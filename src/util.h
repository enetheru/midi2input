//
// Created by enetheru on 9/26/17.
//

#ifndef MIDI2INPUT_LOG_H
#define MIDI2INPUT_LOG_H
#include <iostream>
#include <iomanip>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


#define INFO "[INFO]"
#define WARN "[WARN]"
#define ERROR "[ERROR]"
#define FATAL "[FATAL]"
#define LOG( TYPE ) std::cout << TYPE

namespace m2i {
    enum class ECODE {
        SUCCESS,
        FAILURE,
    };

    fs::path getPath( const fs::path &path );

}//namespace m2i

#endif //MIDI2INPUT_LOG_H
