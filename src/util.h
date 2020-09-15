//
// Created by enetheru on 9/26/17.
//

#ifndef M2I_UTIL_H
#define M2I_UTIL_H
#include <iostream>
#include <iomanip>
#include <memory>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace m2i {

fs::path getPath( fs::path path );

}//namespace m2i

#endif //M2I_UTIL_H
