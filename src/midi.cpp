#include "util.h"
#include "midi.h"
#include <ostream>

std::string
midi_event::str() const
{
    return fmt::format("{:#02x}, {:#02x}, {:3d}", status, data1, data2 );
}


std::ostream& operator<<( std::ostream &os, const midi_event &event ){
    return os << event.str();
} 
