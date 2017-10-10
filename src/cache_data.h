#ifndef CACHE_DATA_H
#define CACHE_DATA_H

#include <vector>
#include <string>

// function to get cached data as a key and values pair
// for now i'm going t use the siimplest possible imaginable method of using a
// separate file per varible with per line data stored in a vector of strings.
std::vector< std::string >
cacheGet( const std::string &key );

int32_t
cacheSet( const std::string &key, const std::string &value );

int32_t
cacheAppend( const std::string &key, const std::string &value );

bool
cacheExists( const std::string &key );

int32_t
cacheClear();


#endif//CACHE_DATA_H
