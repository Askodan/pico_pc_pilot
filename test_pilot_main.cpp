

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/gpio.h"
#include "irdecoders/necdecoder.hpp"

uint ReceivePin = 5;

queue_t signal_queue;
void irq_NEC_Listener(uint gpio, uint32_t events) {
    uint64_t t = to_us_since_boot(get_absolute_time());
    queue_try_add(&signal_queue, &t);
}

void init_receive_pin(uint pin)
{
    gpio_init(pin);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &irq_NEC_Listener);
}

int main()
{
    // important line if want to debug usb
    stdio_init_all();

    queue_init(&signal_queue, sizeof(uint64_t), 128);
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    init_receive_pin(ReceivePin);
    NECAnalyzer analyzer;
    while(true)
    {
        uint64_t new_time;
        bool got_value = queue_try_remove(&signal_queue, &new_time);
        if(got_value)
        {
            bool success = analyzer.analyze_signal(new_time);
            if(success)
            {
                printf("read %i with %i\n", analyzer.get_address(), analyzer.get_data());
                gpio_put(PICO_DEFAULT_LED_PIN, 1);
                sleep_ms(500);
                gpio_put(PICO_DEFAULT_LED_PIN, 0);
            }
        }
        sleep_ms(10);
    }
    return 0;
}