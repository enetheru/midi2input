#include <sstream>
#include <spdlog/spdlog.h>

#include "x11.h"

namespace m2i {

/* NOTE this error call back is to avoid an assertion when X11 cant find the
 * window properties, which is hardly important for our use case */
int XErrorCatcher( Display *disp, XErrorEvent *xe )
{
    (void)disp; (void)xe;
    spdlog::error( FMT_STRING( "X11: Generic XError Catcher that does nothing" ) );
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
        else return None;
    }
    else {
        spdlog::error( FMT_STRING( "X11: Failed XQueryTree" ) );
    }
    return None;
}

std::string
XDetectWindow( Display* xdp )
{
    static Window w_current;

    if(! xdp ){
        spdlog::error( FMT_STRING( "X11: invalid handle to X display" ) );
        return "None";
    }

    Window w;
    int revert_to;
    XGetInputFocus( xdp, &w, &revert_to );
    if( w == None ) return "None";
    if( w == w_current ) return std::string();
    else w_current = w;

    while( true ){
        Atom property = XInternAtom( xdp, "WM_CLASS", False );
        if( property == None ){
            spdlog::error( FMT_STRING( "X11: Failed XInternAtom for WM_CLASS" ) );
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
                spdlog::info( FMT_STRING( "X11: WM_CLASS: {}" ), retval );
                return retval;
            }
            if( actual_format_return == None )
                spdlog::warn( FMT_STRING( "X11: property: WM_CLASS not present" ) );
        }
        else{
            spdlog::error( FMT_STRING( "X11: XGetWindowProperty did not return Success" ) );
            return "None";
        }

        w = XGetParent( xdp, w );
    }
    return "None";
}

}// end namespace m2i


