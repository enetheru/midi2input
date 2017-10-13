#ifndef X11_H
#define X11_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>

namespace m2i {
    int XErrorCatcher( Display *disp, XErrorEvent *xe );
    Window XGetParent( Display *xdp, Window w );
    std::string XDetectWindow( Display *xdp );
}

#endif //X11_H
