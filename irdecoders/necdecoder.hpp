#pragma once

#include "pico/stdlib.h"
#include <bitset>

class NECAnalyzer
{
public:
    bool analyze_signal(uint64_t new_time);
    uint8_t get_address() const {return address;}
    uint8_t get_data() const {return data;}
private:
    bool is_close(uint current, uint value) const;

    void reset_state();

    void analyze_init(uint delta);

    enum class AnalyzerState
    {
        Init,
        Address,
        Data,
    };

    bool analyze_value(uint delta, std::bitset<8>& used_set);
// settings
    float Tolerance = 0.2; // in microseconds

// consts
    const uint BaseInterval = 562;
    const uint OneInterval = 1687;
    const uint FirstInterval = 9000;
    const uint SecondInterval = 4500;

// internal state
    AnalyzerState State = AnalyzerState::Init;
    int StateStep = 0;
    uint64_t last_time = 0;
    std::bitset<8> add_set;
    std::bitset<8> data_set;
    uint8_t address;
    uint8_t data;
};