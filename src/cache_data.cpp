#include "cache_data.h"
#include "util.h"

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

//helper method to get the path of the cache
static std::string
cachePath()
{
    if( getenv( "XDG_CACHE_HOME" ) )
        return std::string( getenv( "XDG_CACHE_HOME" ) ) + "/m2i/";

    std::string home;
    if(! getenv( "HOME" ) )home = "~";
    else home = getenv( "HOME" );

    return home + "/.cache/m2i/";
}

//get list of values from cache key
std::vector< std::string >
cacheGet( const std::string &key )
{
    std::vector< std::string > values;

    /* read file and pull values */
    std::ifstream cache_file( cachePath() + key );
    LOG( INFO ) << cachePath() + key << "\n";
    if(! cache_file.is_open() ) return values;

    for( std::string line; std::getline( cache_file, line ); ){
        values.push_back( line );
    }
    return values;
}

//clobber cache value
int32_t
cacheSet( const std::string &key, const std::string &value )
{
    if(! fs::exists( cachePath() ) )
        fs::create_directories( cachePath() );

    std::ofstream cache_file( cachePath() + key );
    if(! cache_file.is_open() ) return -1;
    else cache_file << value;
    /* clobber value in cache */
    return 0;
}

//add to cache value
int32_t
cacheAppend( const std::string &key, const std::string &value )
{
    if(! fs::exists( cachePath() ) )
        fs::create_directories( cachePath() );

    std::ofstream cache_file( cachePath() + key, std::ofstream::app );
    if(! cache_file.is_open() ) return -1;
    else cache_file << "\n" << value;
    return 0;
}

// access for boolean values;
bool
cacheExists( const std::string& key )
{
    /* read file and pull values */
    std::ifstream cache_file( cachePath() + key );
    if(! cache_file.is_open() ) return false;
    return true;
}

//delete all values in the cache
int32_t
cacheClear()
{
    /* delete all files in $XDG_CACHE_HOME/m2i */
    fs::remove_all( cachePath() );
    return 0;
}
