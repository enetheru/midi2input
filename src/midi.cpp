#include <fmt/format.h>
#include "midi.h"

std::string
midi_event::str() const
{
    return fmt::format("{:#04x}, {:#04x}, {:3d}", status, data1, data2 );
}


std::ostream& operator<<( std::ostream &os, const midi_event &event ){
    return os << event.str();
} 
