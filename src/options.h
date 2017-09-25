#include <cstdio>
#include <iostream>
#include "optionparser.h"

/* Argument tests */
struct Arg: public option::Arg
{
    static void
    printError( const char* msg1, const option::Option& opt, const char* msg2 )
    {
        std::cerr << msg1 << std::endl;
        fwrite( opt.name, opt.namelen, 1, stderr );
        std::cerr << msg2 << std::endl;
    }

    static option::ArgStatus
    Unknown( const option::Option& option, bool msg )
    {
        if( msg ) std::cerr << "Unknown option " << option << "\n";
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus
    Required( const option::Option& option, bool msg )
    {
        if( option.arg != 0 ) return option::ARG_OK;
        if( msg ) std::cerr << "Option '" << option << "' requires an argument\n";
        return option::ARG_ILLEGAL;
    }
};
