//
// Created by enetheru on 9/26/17.
//

#ifndef M2I_UTIL_H
#define M2I_UTIL_H
#include <iostream>
#include <iomanip>
#include "fmt/format.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

//TODO replace this with actual logging facility
#define INFO "[INFO]"
#define WARN "[WARN]"
#define ERROR "[ERROR]"
#define FATAL "[FATAL]"
#define NONE ""
#define LOG( TYPE ) std::cout << TYPE

namespace m2i {
    enum class ECODE {
        SUCCESS,
        FAILURE,
    };

    fs::path getPath( const fs::path &path );

}//namespace m2i

#endif //M2I_UTIL_H
