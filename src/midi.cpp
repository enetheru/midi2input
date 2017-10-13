#include "midi.h"

#include <sstream>
#include <iomanip>

std::string
midi2string( const midi_event& event ){
    std::stringstream string;
    string << std::hex << std::setfill( '0' ) << std::uppercase
           << "0x" << std::setw( 2 ) << (int)event.status << ", "
           << "0x" << std::setw( 2 ) << (int)event.data1 << ", "
           << std::dec << std::setfill( ' ' ) << std::setw( 3 ) << (int)event.data2;
    return string.str();
}
