#include "util.h"
#include "x11.h"
#include <sstream>

namespace m2i {

int XErrorCatcher( Display *disp, XErrorEvent *xe )
{
    (void)disp; (void)xe;
    //FIXME this is to shut things up; we might want to
    // handle this at some point in the future though
    LOG( ERROR ) << "Something had happened, bruh\n";
    return 0;
}

Window
XGetParent( Display *xdp, Window w )
{
    Window root_return;
    Window parent_return;
    Window *children_return = nullptr;
    unsigned int nchildren_return = 0;

    if( XQueryTree( xdp, w,
                &root_return,
                &parent_return,
                &children_return,
                &nchildren_return ) ){
        if( children_return != nullptr ) XFree( children_return );
        if( parent_return != DefaultRootWindow( xdp ) )
            return parent_return;
        else return -1;
    }
    else {
        LOG( ERROR ) << "Failed XQueryTree\n";
    }
    return -1;
}

std::string
XDetectWindow( Display* xdp )
{
    static Window w_current;

    if(! xdp ){
        LOG( ERROR ) << "invalid handle to X display\n";
        return "None";
    }

    Window w;
    int revert_to;
    XGetInputFocus( xdp, &w, &revert_to );
    if( w == None || w == w_current ) return "None";
    else w_current = w;

    while( true ){
        Atom property = XInternAtom( xdp, "WM_CLASS", False );
        if( property == None ){
            LOG( ERROR ) << "Failed XInternAtom for WM_CLASS\n";
            break;
        }

        Atom actual_type_return;
        int actual_format_return = 0;
        unsigned long nitems_return = 0L;
        unsigned long bytes_after_return = 0L;
        unsigned char *prop_return = nullptr;

        if( XGetWindowProperty(
                    xdp, w, property, 0L, 1024L, False, XA_STRING,
                    &actual_type_return,
                    &actual_format_return,
                    &nitems_return,
                    &bytes_after_return,
                    &prop_return ) == Success )
        {
            if( actual_type_return == XA_STRING
             && actual_format_return == 8 ){
                std::string retval( reinterpret_cast< const char*>( prop_return ) );
                XFree( prop_return );
                LOG( INFO ) << "WM_CLASS: " << retval << "\n";
                return retval;
            }
            if( actual_format_return == None )
                LOG( INFO ) << "property: WM_CLASS not present\n";
        }
        else{
            LOG( ERROR ) << "XGetWindowProperty did not return Success\n";
            return "None";
        }

        w = XGetParent( xdp, w );
    }
    return "None";
}

}// end namespace m2i


