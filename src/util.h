//
// Created by enetheru on 9/26/17.
//

#ifndef M2I_UTIL_H
#define M2I_UTIL_H
#include <iostream>
#include <iomanip>
#include <memory>
#include "fmt/format.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace m2i {

//FIXME I dont like this anonymous enum
enum { //log level
    INFO = 4,
    WARN = 3,
    ERROR = 2,
    FATAL = 1,
    NONE = 0,
};

class logger{
public:
    template< typename T>
    logger &operator<<( const T& input ){
        if( log ) std::cout << input;
        //TODO give this stderr too
        return *this;
    }

    bool log = true;
    friend logger LOG( const std::string &input );
};

logger LOG( int );

fs::path getPath( const fs::path &path );

}//namespace m2i

#endif //M2I_UTIL_H
