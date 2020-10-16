--[[ Imported c++ Functions ]]--
--midi_send( { status, data1, data2 } )
--keypress( XK_keycode )
--keydown( XK_keycode )
--keyup( XK_keycode )
--buttonpress( n )
--buttondown( n )
--buttonup( n )
--mousemove( x, y )
--mousepos( x, y )
--exec( 'command' )
--
--[[ Imported Global Variables ]]--
--WM_CLASS
--
--[[ Functions you want to define yourself]]--
--midi_recv( status, data1, data2 )
--pre_loop()
--loop()

--[[ helper functions]]--
--[[=================]]--
--
--[[ MSB LSB ]]--
-- asign the MSB function to the MSB event in the event map
-- assign your function to the LSB event
-- use the LSB function in your function to get the final value
msb = nil
function MSB( event )
    msb = event[3]
end

function LSB( event )
    if( not msb ) then
        print( 'no MSB to gather' )
        return
    end
    local final = (msb * 256) + event[3]
    print( final )
    msb = nil
    return final
end

function reset_state( state )
    --TODO loop through the midi state map and set all the lights to their default
end

-- because i always give the event to functions i needed a wrapper for keypress
function kpress( event, keys )
    if( type( keys ) == "number" ) then
        print("single key event")
        keypress( keys )
        return
    end
    --FIXME needs one shot combo shortcut implementation
    if( type( keys ) == "table" ) then
        print( "multi key event" )
        kdown( event, keys )
        -- FIXME need to reverse the list here
        kup( event, keys )
        return
    end
    print( "Unhandled type: " .. type( key ) )
end


function kdown( event, keys )
    print( "kdown function" )
    if( type( keys ) == "number" ) then
        print("single key event")
        keydown( keys )
        return
    end
    if( type( keys ) == "table" ) then
        print( "multi key event" )
        for i,key in pairs(keys) do
             keydown( key )
        end
        return
    end
    print( "Unhandled type: " .. type( key ) )
end

function kup( event, keys )
    print( "kup function" )
    if( type( keys ) == "number" ) then
        print("single key event")
        keyup( keys )
        return
    end
    if( type( keys ) == "table" ) then
        print( "multi key event" )
        for i,key in pairs(keys) do
            keyup( key )
        end
        return
    end
    print( "Unhandled type: " .. type( key ) )
end

function event_to_string( event )
    return string.format("0x%2X, 0x%2X, %3d", event[1], event[2], event[3] )
end

--[[ keystrokes defined by linux\input-event-codes.h]]--
KEY_RESERVED            = 0
KEY_ESC                 = 1
KEY_1                   = 2
KEY_2                   = 3
KEY_3                   = 4
KEY_4                   = 5
KEY_5                   = 6
KEY_6                   = 7
KEY_7                   = 8
KEY_8                   = 9
KEY_9                   = 10
KEY_0                   = 11
KEY_MINUS               = 12
KEY_EQUAL               = 13
KEY_BACKSPACE           = 14
KEY_TAB                 = 15
KEY_Q                   = 16
KEY_W                   = 17
KEY_E                   = 18
KEY_R                   = 19
KEY_T                   = 20
KEY_Y                   = 21
KEY_U                   = 22
KEY_I                   = 23
KEY_O                   = 24
KEY_P                   = 25
KEY_LEFTBRACE           = 26
KEY_RIGHTBRACE          = 27
KEY_ENTER               = 28
KEY_LEFTCTRL            = 29
KEY_A                   = 30
KEY_S                   = 31
KEY_D                   = 32
KEY_F                   = 33
KEY_G                   = 34
KEY_H                   = 35
KEY_J                   = 36
KEY_K                   = 37
KEY_L                   = 38
KEY_SEMICOLON           = 39
KEY_APOSTROPHE          = 40
KEY_GRAVE               = 41
KEY_LEFTSHIFT           = 42
KEY_BACKSLASH           = 43
KEY_Z                   = 44
KEY_X                   = 45
KEY_C                   = 46
KEY_V                   = 47
KEY_B                   = 48
KEY_N                   = 49
KEY_M                   = 50
KEY_COMMA               = 51
KEY_DOT                 = 52
KEY_SLASH               = 53
KEY_RIGHTSHIFT          = 54
KEY_KPASTERISK          = 55
KEY_LEFTALT             = 56
KEY_SPACE               = 57
KEY_CAPSLOCK            = 58
KEY_F1                  = 59
KEY_F2                  = 60
KEY_F3                  = 61
KEY_F4                  = 62
KEY_F5                  = 63
KEY_F6                  = 64
KEY_F7                  = 65
KEY_F8                  = 66
KEY_F9                  = 67
KEY_F10                 = 68
KEY_NUMLOCK             = 69
KEY_SCROLLLOCK          = 70
KEY_KP7                 = 71
KEY_KP8                 = 72
KEY_KP9                 = 73
KEY_KPMINUS             = 74
KEY_KP4                 = 75
KEY_KP5                 = 76
KEY_KP6                 = 77
KEY_KPPLUS              = 78
KEY_KP1                 = 79
KEY_KP2                 = 80
KEY_KP3                 = 81
KEY_KP0                 = 82
KEY_KPDOT               = 83

KEY_ZENKAKUHANKAKU      = 85
KEY_102ND               = 86
KEY_F11                 = 87
KEY_F12                 = 88
KEY_RO                  = 89
KEY_KATAKANA            = 90
KEY_HIRAGANA            = 91
KEY_HENKAN              = 92
KEY_KATAKANAHIRAGANA    = 93
KEY_MUHENKAN            = 94
KEY_KPJPCOMMA           = 95
KEY_KPENTER             = 96
KEY_RIGHTCTRL           = 97
KEY_KPSLASH             = 98
KEY_SYSRQ               = 99
KEY_RIGHTALT            = 100
KEY_LINEFEED            = 101
KEY_HOME                = 102
KEY_UP                  = 103
KEY_PAGEUP              = 104
KEY_LEFT                = 105
KEY_RIGHT               = 106
KEY_END                 = 107
KEY_DOWN                = 108
KEY_PAGEDOWN            = 109
KEY_INSERT              = 110
KEY_DELETE              = 111
KEY_MACRO               = 112
KEY_MUTE                = 113
KEY_VOLUMEDOWN          = 114
KEY_VOLUMEUP            = 115
KEY_POWER               = 116	--[ SC System Power Down ]--
KEY_KPEQUAL             = 117
KEY_KPPLUSMINUS         = 118
KEY_PAUSE               = 119
KEY_SCALE               = 120	--[ AL Compiz Scale (Expose)
KEY_KPCOMMA             = 121
KEY_HANGEUL             = 122
KEY_HANGUEL             = KEY_HANGEUL
KEY_HANJA               = 123
KEY_YEN                 = 124
KEY_LEFTMETA            = 125
KEY_RIGHTMETA           = 126
KEY_COMPOSE             = 127
KEY_STOP                = 128	--[ AC Stop ]--
KEY_AGAIN               = 129
KEY_PROPS               = 130	--[ AC Properties ]--
KEY_UNDO                = 131	--[ AC Undo ]--
KEY_FRONT               = 132
KEY_COPY                = 133	--[ AC Copy ]--
KEY_OPEN                = 134	--[ AC Open ]--
KEY_PASTE               = 135	--[ AC Paste ]--
KEY_FIND                = 136	--[ AC Search ]--
KEY_CUT                 = 137	--[ AC Cut ]--
KEY_HELP                = 138	--[ AL Integrated Help Center ]--
KEY_MENU                = 139	--[ Menu (show menu) ]--
KEY_CALC                = 140	--[ AL Calculator ]--
KEY_SETUP               = 141
KEY_SLEEP               = 142	--[ SC System Sleep ]--
KEY_WAKEUP              = 143	--[ System Wake Up ]--
KEY_FILE                = 144	--[ AL Local Machine Browser ]--
KEY_SENDFILE            = 145
KEY_DELETEFILE          = 146
KEY_XFER                = 147
KEY_PROG1               = 148
KEY_PROG2               = 149
KEY_WWW                 = 150	--[ AL Internet Browser ]--
KEY_MSDOS               = 151
KEY_COFFEE              = 152	--[ AL Terminal Lock/Screensaver ]--
KEY_SCREENLOCK          = KEY_COFFEE
KEY_ROTATE_DISPLAY      = 153	--[ Display orientation for e.g. tablets ]--
KEY_DIRECTION           = KEY_ROTATE_DISPLAY
KEY_CYCLEWINDOWS        = 154
KEY_MAIL                = 155
KEY_BOOKMARKS           = 156	--[ AC Bookmarks ]--
KEY_COMPUTER            = 157
KEY_BACK                = 158	--[ AC Back ]--
KEY_FORWARD             = 159	--[ AC Forward ]--
KEY_CLOSECD             = 160
KEY_EJECTCD             = 161
KEY_EJECTCLOSECD        = 162
KEY_NEXTSONG            = 163
KEY_PLAYPAUSE           = 164
KEY_PREVIOUSSONG        = 165
KEY_STOPCD              = 166
KEY_RECORD              = 167
KEY_REWIND              = 168
KEY_PHONE               = 169	--[ Media Select Telephone ]--
KEY_ISO                 = 170
KEY_CONFIG              = 171	--[ AL Consumer Control Configuration ]--
KEY_HOMEPAGE            = 172	--[ AC Home ]--
KEY_REFRESH             = 173	--[ AC Refresh ]--
KEY_EXIT                = 174	--[ AC Exit ]--
KEY_MOVE                = 175
KEY_EDIT                = 176
KEY_SCROLLUP            = 177
KEY_SCROLLDOWN          = 178
KEY_KPLEFTPAREN         = 179
KEY_KPRIGHTPAREN        = 180
KEY_NEW                 = 181	--[ AC New ]--
KEY_REDO                = 182	--[ AC Redo/Repeat ]--

KEY_F13                 = 183
KEY_F14                 = 184
KEY_F15                 = 185
KEY_F16                 = 186
KEY_F17                 = 187
KEY_F18                 = 188
KEY_F19                 = 189
KEY_F20                 = 190
KEY_F21                 = 191
KEY_F22                 = 192
KEY_F23                 = 193
KEY_F24                 = 194

BTN_GAMEPAD		        = 0x130
BTN_SOUTH		        = 0x130
BTN_A			        = BTN_SOUTH
BTN_EAST		        = 0x131
BTN_B			        = BTN_EAST
BTN_C			        = 0x132
BTN_NORTH		        = 0x133
BTN_X			        = BTN_NORTH
BTN_WEST		        = 0x134
BTN_Y			        = BTN_WEST
BTN_Z			        = 0x135
BTN_TL			        = 0x136
BTN_TR			        = 0x137
BTN_TL2			        = 0x138
BTN_TR2			        = 0x139
BTN_SELECT		        = 0x13a
BTN_START		        = 0x13b
BTN_MODE		        = 0x13c
BTN_THUMBL		        = 0x13d
BTN_THUMBR		        = 0x13e


--[[ DENON DN-SC2000 ]]--
-------------------------
controller = {}

--The controller can be configure to use different channels for decks A and B
-- consult the official instructions on how to change the channels of the
-- controller. and then change these variables to reflect
controller.DECKA = { channel = 0x00 }
controller.DECKB = { channel = 0x01 }

--[[ Controller Codes ]]--
-- All of the Note On codes for the controller
-- n = MIDI CH = 0-7
-- packet {
--      SW ON: 0x9n SW OFF: 0x8n,
--      <below code>,
--      SW ON: 0x40 SW OFF: 0x00
-- }
XC_AUTO_LOOP                = 0x1D
XC_BEAT_MINUS               = 0x69
XC_BEAT_PLUS                = 0x6A
XC_BEND_MINUS               = 0x0D
XC_BEND_PLUS                = 0x0C
XC_BROWSER                  = 0x64
XC_CUE                      = 0x42
XC_CUE1                     = 0x17
XC_CUE2                     = 0x18
XC_CUE3                     = 0x19
XC_CUE4                     = 0x20
XC_CUE5                     = 0x21
XC_CUE6                     = 0x22
XC_CUE7                     = 0x23
XC_CUE8                     = 0x24
XC_CUE_LOOP_DELETE_SHIFT    = 0x60
XC_DECK_CHG                 = 0x03
XC_DRY_WET_SW_FX1           = 0x15
XC_DRY_WET_SW_FX2           = 0x55
XC_DUPLICATE                = 0x65
XC_FAST_SEARCH_MINUS        = 0x11
XC_FAST_SEARCH_PLUS         = 0x10
XC_FILTER_SW                = 0x68
XC_FX_ON_1                  = 0x56
XC_FX_ON_2                  = 0x57
XC_FX_UNIT                  = 0x58
XC_JOGWHEEL_TOUCH           = 0x51
XC_KEY_LOCK                 = 0x06
XC_LOOP_IN                  = 0x37
XC_LOOP_OUT                 = 0x39
XC_NEXT                     = 0x29
XC_PARAM1_SW_FX1            = 0x12
XC_PARAM1_SW_FX2            = 0x52
XC_PARAM2_SW_FX1            = 0x13
XC_PARAM2_SW_FX2            = 0x53
XC_PARAM3_SW_FX1            = 0x14
XC_PARAM3_SW_FX2            = 0x54
XC_PLAY                     = 0x43
XC_PREVIOUS                 = 0x30
XC_SEL                      = 0x1A
XC_SYNC                     = 0x6B
XC_TRACK_SELECT_SW          = 0x28

-- Control Signal codes
-- packet {
--      0xBn,
--      <below code>,
--      Increment: 00
--      Decrement: 0x7F
-- }
XC_DRY_WET_KNOB_FX1         = 0x55
XC_DRY_WET_KNOB_FX2         = 0x59
XC_PARAM1_KNOB_FX1          = 0x56
XC_PARAM1_KNOB_FX2          = 0x5A
XC_PARAM2_KNOB_FX1          = 0x57
XC_PARAM2_KNOB_FX2          = 0x5B
XC_PARAM3_KNOB_FX1          = 0x58
XC_PARAM3_KNOB_FX2          = 0x5C
XC_TRACK_SELECT_KNOB        = 0x54
XC_FILTER_KNOB              = 0x5D

-- packet {
--      0xBn,
--      <below code>,
--      Reverse: 0x3F-0x01
--      Forward: 0x41~0x7F
--      Slow->Fast
--      * relative data 2048Pulse/cycle
-- }
XC_JOGWHEEL_FWD_REV         = 0x51

--PITCH_SLIDER is a bit different, with packets like this
--{0xEn, 0xllx(LSB), 0xmm (MSB) }

--The lights are controlled by sending a control signal with the first byte
--indicating the signal to send, and the second byte indicating the light
-- signals:
-- * 0x4A = On
-- * 0x4B = Off
-- * 0x4C = Blink
-- ie. {0xBn, <signal>, <light>}, where n is the channel number
-- eg. {0xB0, 0x4A, 0x5C}  - switches the play light on.
-- Some buttons have two light values associated with them, one light is normal
-- brightness, and the other is a dim version, both the normal and the dim
-- light use the same control signals of on, off, blink.

-- Confusingly the control signals to change the light value have no
-- relationship with the note on and off values that are sent by the controller
-- which makes sense only in the case of the normal and dim light buttons,
-- because there are two lights for the same button.
XL_DRY_WET_KNOB_FX1         = 0x5C
XL_DRY_WET_KNOB_FX2         = 0x60
XL_PARAM_1_KNOB_FX1         = 0x5D
XL_PARAM_1_KNOB_FX2         = 0x61
XL_PARAM_2_KNOB_FX1         = 0x5E
XL_PARAM_2_KNOB_FX2         = 0x62
XL_PARAM_3_KNOB_FX1         = 0x5F
XL_PARAM_3_KNOB_FX2         = 0x63
XL_FX_ON_1                  = 0x5A
XL_FX_ON_2                  = 0x5B
XL_KEY_LOCK                 = 0x08
XL_SYNC                     = 0x09
XL_CUE1                     = 0x11
XL_CUE1_Dimmer              = 0x12
XL_CUE2                     = 0x13
XL_CUE2_Dimmer              = 0x14
XL_CUE3                     = 0x15
XL_CUE3_Dimmer              = 0x16
XL_CUE4                     = 0x17
XL_CUE4_Dimmer              = 0x18
XL_CUE5                     = 0x19
XL_CUE5_Dimmer              = 0x1A
XL_CUE6                     = 0x1B
XL_CUE6_Dimmer              = 0x1C
XL_CUE7                     = 0x1D
XL_CUE7_Dimmer              = 0x1F
XL_CUE8                     = 0x20
XL_CUE8_Dimmer              = 0x21
XL_PLAY                     = 0x27
XL_CUE                      = 0x26
XL_LOOP_IN                  = 0x24
XL_LOOP_IN_Dimmer           = 0x3E
XL_LOOP_OUT                 = 0x40
XL_LOOP_OUT_Dimmer          = 0x2A
XL_AUTO_LOOP                = 0x2B
XL_AUTO_LOOP_Dimmer         = 0x53


--[[ Application Functions ]]--
--[[=======================]]--

-- use deck a as scroll wheel
jogdial = {
    counter = 0,
    turn = function( event )
        if( jogdial.counter < 10 ) then
            jogdial.counter = jogdial.counter + 1
            return
        else
            jogdial.counter = 0
        end

        if event[3] < 64 then
            mousescroll( 120 )
        end
        if event[3] > 64 then
            mousescroll( -120 )
        end
    end
}

--[[ Clementine ]]--
clementine = {}
clementine.next_prev = function( event )
    if( event[3] > 65 ) then
        exec( "clementine -r" )
    else
        exec( "clementine -f" )
    end
end

clementine.play_pause = function( event )
    exec( "clementine -t" )
end

--[[ Pulse Audio ]]--
pa = {
    volume = 0,
    volume_ms = milliseconds(),
    set_volume = function( event )
        if( milliseconds() - pa.volume_ms < 25 )then
            return
        end
        pa.volume_ms = milliseconds()

        temp = math.floor( (event[3] / 2.550) )
        if( pa.volume == temp) then
            return
        end
        pa.volume = temp
        -- old way exec("amixer -D pulse sset Master " .. temp .. "%" )
        exec( "pactl set-sink-volume @DEFAULT_SINK@ " .. temp .. "%" )
    end
}

--[[ Default Keymap ]]--
------------------------
default = {}

default.map = {}

--DECK A:BLUE Note-On functions
default.map[0x90 + controller.DECKA.channel ]= {
    [ XC_PLAY                  ] = { ['*'] = clementine.play_pause },
    [ XC_CUE                   ] = {
        [ 64 ] = function( event ) keydown( BTN_A ) end,
        [  0 ] = function( event ) keyup( BTN_A ) end
    },
}

--DECK A:BLUE control signals
default.map[0xB0 + controller.DECKA.channel ] = {
    [ XC_TRACK_SELECT_KNOB ] = { ['*'] = clementine.next_prev },
    [ XC_JOGWHEEL_FWD_REV  ] = { ['*'] = jogdial.turn },
}

--DECK A:BLUE Pitch Bend
default.map[0xE0 + controller.DECKA.channel] = {
    [0x00] = { ['*'] = pa.set_volume },
}

--[[ application specific maps ]]--
--[[===========================]]--

--[[ ffplay ]]--
ffplay = {}
ffplay.map = {}

-- play pause when play button is pressed
ffplay.map[ 0x90 + controller.DECKB.channel ]= {
    [ XC_PLAY ]= { [127]= function( event )
        keypress( XK_space )
    end },
}
-- scrub through footage with jog wheel
ffplay.map[0xB0 + controller.DECKB.channel ]= {
    [ XC_JOGWHEEL_FWD_REV ] = { ['*'] =
        function( event )
            if event[3] > 64 then
                keypress( XK_Right )
            end
            if event[3] < 64 then
                keypress( XK_Left )
            end
        end },
}

--[[ Blender ]]--
--[[ ======= ]]--

Blender = {}
Blender.func = {}

function Blender.func.scrub( event )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

Blender.map = {}

Blender.map[0x90 + controller.DECKB.channel ] = {
    [ XC_CUE      ] = { ['*'] = function( event ) keypress( XK_m ) end },
    [ XC_PLAY     ] = { ['*'] = function( event ) keypress( XK_space ) end },
    [ XC_NEXT     ] = { ['*'] = function( event ) keypress( XK_Page_Up ) end },
    [ XC_PREVIOUS ] = { ['*'] = function( event ) keypress( XK_Page_Down ) end },
    [ XC_FAST_SEARCH_PLUS ] = { ['*'] =
        function( event ) kpress( event, { XK_Shift_L, XK_period } ) end },
    [ XC_FAST_SEARCH_MINUS ] = { ['*'] =
        function( event ) kpress( event, { XK_Shift_L, XK_comma }) end },
}
Blender.map[0xB0 +  controller.DECKB.channel ] = {
    [ XC_JOGWHEEL_FWD_REV ] = { ['*'] = Blender.func.scrub },
    [ XC_TRACK_SELECT_KNOB ] = {
        [  0] = function( event ) keypress( XK_Page_Up ) end,
        [127] = function( event ) keypress( XK_Page_Down ) end
    },
}

--[[ Built in midi2input control functions ]]--
--[[=======================================]]--
function script_init()
    print( "beans nothing to do" )
    loopenable()
end

function loop()
    detectwindow();
    --alsaconnect( "DN-SC2000", "*" );
    return 0;
end

--[[ receive and react ]]--
function midi_recv( channel, control, value )
    event = {channel, control, value}
    local app = _G[ WM_CLASS ]
    if not app then app = default end

    local current = app.map
    --check for channel and control first.
    if current[channel] and current[channel][control]
    then
        local control = current[channel][control]
        -- we have the control, now look for the value or wildcard
        if control[ value ] then
            control[ value ]( event )
        elseif control[ '*' ] then
            control[ '*' ]( event )
        else
            --unable to find action for event
            return
        end
    end
end


