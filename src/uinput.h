//
// Created by enetheru on 22/9/20.
//

#ifndef M2I_UINPUT_H
#define M2I_UINPUT_H

// the event codes are derived from /usr/include/linux/input-event-codes.h

namespace m2i {
// This class  exists to manage the virtual uinput device
    class Uinput {
        int fd;
        struct libevdev_uinput *uidev{};
    public:
        Uinput();
        ~Uinput();

        //emulating input devices
        void keypress( int input_event_code );
        void keydown( int input_event_code );
        void keyup( int input_event_code );
        void mousemove( int rel_x, int rel_y );
        void mousewarp( int abs_x, int abs_y );
        void mousescroll( int distance );
        void mousehscroll( int distance );
    };

}// namespace m2i
#endif //M2I_UINPUT_H