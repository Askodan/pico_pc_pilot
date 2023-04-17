#include "necdecoder.hpp"


bool NECAnalyzer::analyze_signal(uint64_t new_time)
{
    update_time(new_time);
    // switch would be ok... maybe?
    if (State == AnalyzerState::Init)
    {
        analyze_init(delta);
    }
    else if (State == AnalyzerState::Address)
    {
        bool finished = analyze_value(delta, add_set);
        if(finished)
        {
            address = add_set.to_ulong();
            State = AnalyzerState::Data;
            StateStep = 0;
        }
    }
    else if (State == AnalyzerState::Data)
    {
        bool finished = analyze_value(delta, data_set);
        if(finished)
        {
            data = data_set.to_ulong();
            reset_state();
            return true;
        }
    }
    return false;
}

void NECAnalyzer::reset_state()
{
    State = AnalyzerState::Init;
    StateStep = 0;
}

void NECAnalyzer::analyze_init(uint delta)
{
    switch(StateStep)
    {
        case 0:
            if(is_close(delta, FirstInterval))
            {
                StateStep++;
            }
            break;
        case 1:
            if(is_close(delta, SecondInterval))
            {
                StateStep = 0;
                State = AnalyzerState::Address;
            }else
            {
                reset_state();
            }
            break;
    }
}

bool NECAnalyzer::analyze_value(uint delta, std::bitset<8>& used_set)
{
    StateStep++;
    if(StateStep%2)
    {
        // this is break that is present between bits
        if(!is_close(delta, BaseInterval))
        {
            reset_state(); 
        }
        return false;
    }

    // find value
    bool Value;
    if(is_close(delta, BaseInterval))
    {
        Value = 0;
    }else if(is_close(delta, OneInterval))
    {
        Value = 1;
    }else
    {
        reset_state();
        return false;
    }
    // decide on bit to fill
    uint8_t Bit = (StateStep-1)/2;
    if(Bit < 8)
        used_set[Bit] = Value;
    else
    { 
        // mirrored check
        if(used_set[Bit-8] == Value){
            reset_state();
        }
    }
    return Bit == 15;
}