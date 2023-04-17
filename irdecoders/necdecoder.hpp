#pragma once

#include "basedecoder.hpp"
#include "pico/stdlib.h"
#include <bitset>

class NECAnalyzer : public IRAnalyzer
{
public:
    virtual bool analyze_signal(uint64_t new_time) override;
private:

    void reset_state();

    void analyze_init(uint delta);

    enum class AnalyzerState
    {
        Init,
        Address,
        Data,
    };

    bool analyze_value(uint delta, std::bitset<8>& used_set);

// consts
    const uint BaseInterval = 562;
    const uint OneInterval = 1687;
    const uint FirstInterval = 9000;
    const uint SecondInterval = 4500;

// internal state
    AnalyzerState State = AnalyzerState::Init;
    int StateStep = 0;
    std::bitset<8> add_set;
    std::bitset<8> data_set;
};