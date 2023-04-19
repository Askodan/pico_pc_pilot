#pragma once

#include "basedecoder.hpp"
#include "pico/stdlib.h"
#include <bitset>

class RC5Analyzer : public IRAnalyzer
{
public:
    virtual bool analyze_signal(uint64_t new_time) override;
    bool get_up_bit() const;
private:
    void init_message_analysis();
    void end_analysis();
    void analyze_base_interval();
    void analyze_double_interval();
    void parse_message_to_data();
    void parse_address();
    void parse_command();
    const uint base_tick = 874;
    bool messageStarted = false;
    bool halfbit = true;
    int messageIndex = 0;

    const int length = 14;
    std::bitset<14> message;
    const int command_size = 7;
    std::bitset<7> com_set;
    const int address_size = 5;
    std::bitset<5> add_set;

    bool up_bit = false;
};