#pragma once

#include <map>
//#include <pair>
#include "tusb.h"
#include "usb_descriptors.h"

struct BasicInput
{
    uint8_t Address;
    uint8_t Command;
    bool operator <(const BasicInput& Another) const
    {
        if(Address!= Another.Address)
            return Address < Another.Address;
        return Command < Another.Command;
    }
};

struct BasicAction
{
    uint8_t ReportID;
    uint32_t Action;
    uint8_t Modifier;
};

std::map<BasicInput, BasicAction> Actions = {
{{53, 86}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT, 0}},// increase volume
{{53, 87}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT, 0}},// decrease volume
{{53, 16}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, 0}},
{{53, 27}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, 0}},
{{53, 26}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_STOP, 0}},
//{{53, 27}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE}},
{{53, 9}, {REPORT_ID_KEYBOARD, HID_KEY_9, 0}},// key 9
{{53, 8}, {REPORT_ID_KEYBOARD, HID_KEY_8, 0}},// key 9
{{53, 7}, {REPORT_ID_KEYBOARD, HID_KEY_7, 0}},// key 9
{{53, 6}, {REPORT_ID_KEYBOARD, HID_KEY_6, 0}},// key 9
{{53, 5}, {REPORT_ID_KEYBOARD, HID_KEY_5, 0}},// key 9
{{53, 4}, {REPORT_ID_KEYBOARD, HID_KEY_4, 0}},// key 9
{{53, 3}, {REPORT_ID_KEYBOARD, HID_KEY_3, 0}},// key 9
{{53, 2}, {REPORT_ID_KEYBOARD, HID_KEY_2, 0}},// key 9
{{53, 1}, {REPORT_ID_KEYBOARD, HID_KEY_1, 0}},// key 9
{{53, 0}, {REPORT_ID_KEYBOARD, HID_KEY_0, 0}},// key 9
};

struct RC5Action: public BasicAction
{
    bool BlockRepeats;
};

std::map<BasicInput, RC5Action> ActionsRC5 = {
{{0, 16}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT, 0, false}},// increase volume
{{0, 17}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT, 0, false}},// decrease volume
{{0, 60}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, 0, true}},
{{0, 35}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, 0, true}},
{{0, 126}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_STOP, 0, true}},
{{0, 13}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE, 0, true}},
{{0, 117}, {REPORT_ID_KEYBOARD, HID_KEY_P, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTALT, true}},// key 9
{{0, 88}, {REPORT_ID_KEYBOARD, HID_KEY_O, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTALT, true}},// key 9
{{0, 9}, {REPORT_ID_KEYBOARD, HID_KEY_9, 0, true}},// key 9
{{0, 8}, {REPORT_ID_KEYBOARD, HID_KEY_8, 0, true}},// key 9
{{0, 7}, {REPORT_ID_KEYBOARD, HID_KEY_7, 0, true}},// key 9
{{0, 6}, {REPORT_ID_KEYBOARD, HID_KEY_6, 0, true}},// key 9
{{0, 5}, {REPORT_ID_KEYBOARD, HID_KEY_5, 0, true}},// key 9
{{0, 4}, {REPORT_ID_KEYBOARD, HID_KEY_4, 0, true}},// key 9
{{0, 3}, {REPORT_ID_KEYBOARD, HID_KEY_3, 0, true}},// key 9
{{0, 2}, {REPORT_ID_KEYBOARD, HID_KEY_2, 0, true}},// key 9
{{0, 1}, {REPORT_ID_KEYBOARD, HID_KEY_1, 0, true}},// key 9
{{0, 0}, {REPORT_ID_KEYBOARD, HID_KEY_0, 0, true}},// key 9
};