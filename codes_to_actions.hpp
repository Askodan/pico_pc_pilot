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
};

std::map<BasicInput, BasicAction> Actions = {
{{53, 86}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT}},// increase volume
{{53, 87}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT}},// decrease volume
{{53, 16}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE}},
{{53, 27}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE}},
{{53, 26}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_STOP}},
//{{53, 27}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE}},
{{53, 9}, {REPORT_ID_KEYBOARD, HID_KEY_9}},// key 9
{{53, 8}, {REPORT_ID_KEYBOARD, HID_KEY_8}},// key 9
{{53, 7}, {REPORT_ID_KEYBOARD, HID_KEY_7}},// key 9
{{53, 6}, {REPORT_ID_KEYBOARD, HID_KEY_6}},// key 9
{{53, 5}, {REPORT_ID_KEYBOARD, HID_KEY_5}},// key 9
{{53, 4}, {REPORT_ID_KEYBOARD, HID_KEY_4}},// key 9
{{53, 3}, {REPORT_ID_KEYBOARD, HID_KEY_3}},// key 9
{{53, 2}, {REPORT_ID_KEYBOARD, HID_KEY_2}},// key 9
{{53, 1}, {REPORT_ID_KEYBOARD, HID_KEY_1}},// key 9
{{53, 0}, {REPORT_ID_KEYBOARD, HID_KEY_0}},// key 9
};

struct RC5Action: public BasicAction
{
    bool BlockRepeats;
};

std::map<BasicInput, RC5Action> ActionsRC5 = {
{{0, 16}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT, false}},// increase volume
{{0, 17}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT, false}},// decrease volume
{{0, 60}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, true}},
{{0, 35}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE, true}},
{{0, 126}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_STOP, true}},
{{0, 13}, {REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE, true}},
{{0, 9}, {REPORT_ID_KEYBOARD, HID_KEY_9, true}},// key 9
{{0, 8}, {REPORT_ID_KEYBOARD, HID_KEY_8, true}},// key 9
{{0, 7}, {REPORT_ID_KEYBOARD, HID_KEY_7, true}},// key 9
{{0, 6}, {REPORT_ID_KEYBOARD, HID_KEY_6, true}},// key 9
{{0, 5}, {REPORT_ID_KEYBOARD, HID_KEY_5, true}},// key 9
{{0, 4}, {REPORT_ID_KEYBOARD, HID_KEY_4, true}},// key 9
{{0, 3}, {REPORT_ID_KEYBOARD, HID_KEY_3, true}},// key 9
{{0, 2}, {REPORT_ID_KEYBOARD, HID_KEY_2, true}},// key 9
{{0, 1}, {REPORT_ID_KEYBOARD, HID_KEY_1, true}},// key 9
{{0, 0}, {REPORT_ID_KEYBOARD, HID_KEY_0, true}},// key 9
};