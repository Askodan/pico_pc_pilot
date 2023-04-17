#include "basedecoder.hpp"

#include <stdio.h>
#include "math.h"
bool IRAnalyzer::analyze_signal(uint64_t new_time)
{
    update_time(new_time);
    printf("read delta %i %b\n", delta, up);
    return false;
}

void IRAnalyzer::update_time(uint64_t new_time)
{
    uint64_t delta64 = new_time - last_time;
    delta = delta64;
    last_time = new_time;
    up = !up;
}

bool IRAnalyzer::is_close(uint current, uint value) const
{
    int diff = std::abs((int)current - (int)value);
    int tol = (int)(Tolerance*value);
    return  diff < tol;
}