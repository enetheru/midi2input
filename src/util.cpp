#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "util.h"

namespace m2i {

fs::path
getPath( fs::path path )
{
    //file doesnt exist but the path is relative then it might exist in .config
    if( not fs::exists( path ) and path.is_relative() ){
        if( getenv( "XDG_CONFIG_HOME" ) )
            path = std::string( getenv( "XDG_CONFIG_HOME") ) + "/m2i/" + path.string();
        else
            path = std::string( getenv( "HOME") ) + "/.config/m2i/" + path.string();
    }

    //possible it could be a symlink
    if( fs::exists( path ) and fs::is_symlink( path ) )
        path = fs::read_symlink( path );
    //Note: this only gives one deference, might want more

    if( !fs::exists( path ) )return fs::path();//file simply doesnt exist

    return path;
}

}//namespace m2i
