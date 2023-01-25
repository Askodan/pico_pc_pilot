#pragma once

#include <map>
//#include <pair>
#include "tusb.h"

std::map<std::pair<uint8_t, uint8_t>, std::pair<uint8_t, uint32_t>> Actions = {
{{53, 86}, {3, HID_USAGE_CONSUMER_VOLUME_INCREMENT}},// increase volume
{{53, 87}, {3, HID_USAGE_CONSUMER_VOLUME_DECREMENT}},// decrease volume
{{53, 16}, {3, HID_USAGE_CONSUMER_PLAY_PAUSE}},
{{53, 27}, {3, HID_USAGE_CONSUMER_PLAY_PAUSE}},
{{53, 26}, {3, HID_USAGE_CONSUMER_STOP}},
//{{53, 27}, {3, HID_USAGE_CONSUMER_MUTE}},
{{53, 9}, {1, HID_KEY_9}},// key 9
{{53, 8}, {1, HID_KEY_8}},// key 9
{{53, 7}, {1, HID_KEY_7}},// key 9
{{53, 6}, {1, HID_KEY_6}},// key 9
{{53, 5}, {1, HID_KEY_5}},// key 9
{{53, 4}, {1, HID_KEY_4}},// key 9
{{53, 3}, {1, HID_KEY_3}},// key 9
{{53, 2}, {1, HID_KEY_2}},// key 9
{{53, 1}, {1, HID_KEY_1}},// key 9
{{53, 0}, {1, HID_KEY_0}},// key 9
};