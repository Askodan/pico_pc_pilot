
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/gpio.h"

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "irdecoders/necdecoder.hpp"
#include "irdecoders/rc5decoder.hpp"
#include "codes_to_actions.hpp"

uint ReceivePin = 5;
uint TimeOfBlinking = 500;
uint DiodePin = 10;

uint64_t EndOfBlink = 0;
queue_t signal_queue;
void irq_Listener(uint gpio, uint32_t events) {
    uint64_t t = to_us_since_boot(get_absolute_time());
    queue_try_add(&signal_queue, &t);
}

void init_receive_pin(uint pin)
{
    gpio_init(pin);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &irq_Listener);
}

void init_diode_pin(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, false);
}

void led_blinking_task(void);
void hid_task(NECAnalyzer& analyzer, RC5Analyzer& analyzerRC5);

int main()
{
    queue_init(&signal_queue, sizeof(uint64_t), 128);

    init_receive_pin(ReceivePin);
    init_diode_pin(DiodePin);
    NECAnalyzer analyzerNEC;
    RC5Analyzer analyzerRC5;

    board_init();
    tusb_init();
    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();

        hid_task(analyzerNEC, analyzerRC5);
    }

    return 0;
}

static void send_hid_report(uint8_t report_id, uint32_t btn, uint8_t modifier)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = btn;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
      int8_t const delta = 5;

      if(btn)
      {// no button, right + down, no scroll, no pan
        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
      }
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        uint16_t order = btn;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &order, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(NECAnalyzer& analyzerNEC, RC5Analyzer& analyzerRC5)
{
    static uint8_t last_type = 1;
    static uint32_t start_ms = 0; 
    uint64_t new_time;
    bool got_value = queue_try_remove(&signal_queue, &new_time);
    if(got_value)
    {
        bool success = analyzerNEC.analyze_signal(new_time);
        bool successrc5 = analyzerRC5.analyze_signal(new_time);
        if(success || successrc5)
        {

            // Remote wakeup
            if ( tud_suspended())
            {
                // Wake up host if we are in suspend mode
                // and REMOTE_WAKEUP feature is enabled by host
                tud_remote_wakeup();
            }else
            {
              if(success)
              {  
                auto res = Actions.find({analyzerNEC.get_address(), analyzerNEC.get_data()});
                if(res != Actions.end()) 
                {
                    send_hid_report(res->second.ReportID, res->second.Action, res->second.Modifier);
                    last_type = res->second.ReportID;

                    start_ms = board_millis();
                    EndOfBlink = board_millis()+TimeOfBlinking;
                }
              }
              if(successrc5)
              {  
                static uint8_t last_command = 255;
                static bool last_bit = false;
                auto res = ActionsRC5.find({analyzerRC5.get_address(), analyzerRC5.get_data()});
                const bool found_command = res != ActionsRC5.end();
                const bool new_command = last_command != analyzerRC5.get_data();
                const bool new_button_press = last_bit != analyzerRC5.get_up_bit();
                if(found_command && (new_command || !res->second.BlockRepeats || new_button_press)) 
                {
                  last_bit = analyzerRC5.get_up_bit();
                  last_command = analyzerRC5.get_data();
                  send_hid_report(res->second.ReportID, res->second.Action, res->second.Modifier);
                  last_type = res->second.ReportID;
                  
                  start_ms = board_millis();
                  EndOfBlink = board_millis()+TimeOfBlinking;
                }
              }
            }
        }
    }else
    {
        // Poll every 10ms
        const uint32_t interval_ms = 10;
        static uint32_t start_ms = 0;

        if ( board_millis() - start_ms < interval_ms) return; // not enough time
        start_ms += interval_ms;

        send_hid_report(last_type, 0, 0);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
  (void) instance;
  (void) len;
  // make one shot
  // send_hid_report(report[0], 0);
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  bool diode_light = EndOfBlink > board_millis();
  board_led_write(diode_light);
  gpio_put(DiodePin, diode_light);
} 
