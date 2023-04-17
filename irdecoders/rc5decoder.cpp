#include "rc5decoder.hpp"


bool RC5Analyzer::analyze_signal(uint64_t new_time)
{
    update_time(new_time);
    bool base = is_close(delta, base_tick);
    bool double_base = is_close(delta, 2*base_tick);
    if(!base && !double_base)
    {
        end_analysis();
        return false;
    }

    if(!messageStarted)
    {
        init_message_analysis();
    }

    if(base)
    {
        analyze_base_interval();
    }else
    {
        analyze_double_interval();
    }
    // analysis can be cancelled by setting messageStarted to false
    if(messageStarted && messageIndex == length-1)
    {
        parse_message_to_data();
        end_analysis();
        return true;
    }
    return false;
}

void RC5Analyzer::init_message_analysis()
{
    messageStarted = true;
    messageIndex = 0;
    halfbit = true;
    message[messageIndex] = 1;
}

void RC5Analyzer::end_analysis()
{
    messageStarted = false;
}

void RC5Analyzer::analyze_base_interval()
{
    halfbit = !halfbit;
    if(halfbit)
    {
        messageIndex++;
        message[messageIndex] = message[messageIndex-1];
    }
}

void RC5Analyzer::analyze_double_interval()
{
    if(!halfbit)
    {
        end_analysis();
    }
    messageIndex++;
    message[messageIndex] = !message[messageIndex-1];
}

void RC5Analyzer::parse_message_to_data()
{
    parse_address();
    parse_command();
}

void RC5Analyzer::parse_address()
{
    for(int i = 0; i<address_size; i++)
    {
        add_set[address_size-1-i] = message[i+3];
    }
    address = add_set.to_ulong();
}

void RC5Analyzer::parse_command()
{
    // command last bit is encoded in reverse in second bit of message
    com_set[command_size-1] = !message[1];
    for(int i = 0; i<command_size-1; i++)
    {
        com_set[command_size-2-i] = message[i+8];
    }
    data = com_set.to_ulong();
}