#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "util.h"

namespace m2i {

fs::path
getPath( const fs::path &path )
{
    if( fs::exists( path ) && fs::is_regular_file( path ) )
        return fs::absolute( path );

    // if the path doesnt exist, and it is absolute, stop searching
    if( path.is_absolute() ) return fs::path();

    //otherwise search for a relate path in the config directory
    fs::path temp;
    if( getenv( "XDG_CONFIG_HOME" ) )
        temp = std::string( getenv( "XDG_CONFIG_HOME") ) + "/m2i/" + path.string();
    else
        temp = std::string( getenv( "HOME") ) + "/.config/m2i/" + path.string();

    if( fs::exists( temp ) && fs::is_regular_file( temp ) ){
        return fs::absolute( temp );
    }
    return fs::path();
}

}//namespace m2i
