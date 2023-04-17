#pragma once


#include "pico/stdlib.h"

class IRAnalyzer
{
public:
    virtual bool analyze_signal(uint64_t new_time);
    uint8_t get_address() const {return address;}
    uint8_t get_data() const {return data;}

protected:
    void update_time(uint64_t new_time);
    bool is_close(uint current, uint value) const;
// settings
    float Tolerance = 0.2; // in microseconds
    
    uint64_t last_time = 0;
    bool up = false;
    uint delta = 0;
    uint8_t address;
    uint8_t data;
};