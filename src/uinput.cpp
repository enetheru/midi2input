//
// Created by enetheru on 22/9/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <linux/input-event-codes.h>
#include <spdlog/spdlog.h>

#include "uinput.h"

namespace m2i {
    Uinput::Uinput(){
        struct libevdev *dev;
        dev = libevdev_new();
        libevdev_set_name(dev, "m2i Virtual Input");

        // We need to unmask the input events we wish to generate
        // https://www.kernel.org/doc/html/v4.17/input/event-codes.htm

        //enable relative device events
        libevdev_enable_event_type( dev, EV_REL);
        for( int axis = 0x00; axis < 0x0a; axis++ ){
            libevdev_enable_event_code( dev, EV_REL, axis, nullptr );
        }
        libevdev_enable_event_code( dev, EV_ABS, REL_WHEEL_HI_RES, nullptr );
        libevdev_enable_event_code( dev, EV_ABS, REL_HWHEEL_HI_RES, nullptr );

        //enable absolute device events
        libevdev_enable_event_type( dev, EV_ABS);
        for( int axis = 0x00; axis < 0x29; axis++ ){
            libevdev_enable_event_code( dev, EV_ABS, axis, nullptr );
        }

        // enable button and key event types
        libevdev_enable_event_type(dev, EV_KEY);

        //enable regular keyboard key codes.
        for( int key = 1; key < 249; key++ ){
            libevdev_enable_event_code( dev, EV_KEY, key, nullptr );
        }

        //enable misc buttons
        for( int btn = 0x100; btn < 0x10a; btn++ ){
            libevdev_enable_event_code( dev, EV_KEY, btn, nullptr );
        }

        //enable mouse buttons
        for( int key = 0x110; key < 0x118; key++ ){
            libevdev_enable_event_code( dev, EV_KEY, key, nullptr );
        }

        //enable joystick buttons
        for( int btn = 0x120; btn < 0x130; btn++ ){
            libevdev_enable_event_code( dev, EV_KEY, btn, nullptr );
        }

        // enable gamepad buttons
        for( int btn = 0x130; btn < 0x13f; btn++ ){
            libevdev_enable_event_code( dev, EV_KEY, btn, nullptr );
        }

        //enable wheel buttons
        for( int btn = 0x150; btn < 0x152; btn++ ){
            libevdev_enable_event_code( dev, EV_KEY, btn, nullptr );
        }

        fd = open("/dev/uinput", O_RDWR);
        int rc = libevdev_uinput_create_from_device(dev, fd, &uidev);

        if (rc < 0){
            //no point in continuing if we cannot create a virtual device.
            spdlog::critical( "Failed to create virtual input device: ({})", strerror(-rc));
            close(fd);
            exit(1);
        }

        libevdev_free(dev);
    }

    Uinput::~Uinput(){
        libevdev_uinput_destroy(uidev);
        close(fd);
    }

    void Uinput::keypress(int input_event_code) {
        keydown( input_event_code );
        keyup( input_event_code );
    }

    void Uinput::keydown(int input_event_code) {
        libevdev_uinput_write_event(uidev, EV_KEY, input_event_code, 1);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }

    void Uinput::keyup(int input_event_code) {
        libevdev_uinput_write_event(uidev, EV_KEY, input_event_code, 0);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }

    void Uinput::mousemove(int rel_x, int rel_y) {
        libevdev_uinput_write_event(uidev, EV_REL, REL_X, rel_x);
        libevdev_uinput_write_event(uidev, EV_REL, REL_Y, rel_y);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }

    void Uinput::mousewarp(int abs_x, int abs_y) {
        libevdev_uinput_write_event(uidev, EV_ABS, ABS_X, abs_x);
        libevdev_uinput_write_event(uidev, EV_ABS, ABS_Y, abs_y);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }

    void Uinput::mousescroll( int distance ) {
        libevdev_uinput_write_event(uidev, EV_REL, REL_WHEEL, distance & -1 );
        libevdev_uinput_write_event(uidev, EV_REL, REL_WHEEL_HI_RES, distance );
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }
    void Uinput::mousehscroll( int distance ) {
        libevdev_uinput_write_event(uidev, EV_REL, REL_HWHEEL , distance & -1 );
        libevdev_uinput_write_event(uidev, EV_REL, REL_HWHEEL_HI_RES , distance );
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }
}
