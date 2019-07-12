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

--[[ keystrokes defined by X11\kesymdef.h]]--
XK_BackSpace                   = 0xff08  --/* Back space, back char */
XK_Tab                         = 0xff09
XK_Linefeed                    = 0xff0a  --/* Linefeed, LF */
XK_Clear                       = 0xff0b
XK_Return                      = 0xff0d  --/* Return, enter */
XK_Pause                       = 0xff13  --/* Pause, hold */
XK_Scroll_Lock                 = 0xff14
XK_Sys_Req                     = 0xff15
XK_Escape                      = 0xff1b
XK_Delete                      = 0xffff  --/* Delete, rubout */
XK_Home                        = 0xff50
XK_Left                        = 0xff51  --/* Move left, left arrow */
XK_Up                          = 0xff52  --/* Move up, up arrow */
XK_Right                       = 0xff53  --/* Move right, right arrow */
XK_Down                        = 0xff54  --/* Move down, down arrow */
XK_Prior                       = 0xff55  --/* Prior, previous */
XK_Page_Up                     = 0xff55
XK_Next                        = 0xff56  --/* Next */
XK_Page_Down                   = 0xff56
XK_End                         = 0xff57  --/* EOL */
XK_Begin                       = 0xff58  --/* BOL */
XK_F1                          = 0xffbe
XK_F2                          = 0xffbf
XK_F3                          = 0xffc0
XK_F4                          = 0xffc1
XK_F5                          = 0xffc2
XK_F6                          = 0xffc3
XK_F7                          = 0xffc4
XK_F8                          = 0xffc5
XK_F9                          = 0xffc6
XK_F10                         = 0xffc7
XK_F11                         = 0xffc8
XK_L1                          = 0xffc8
XK_F12                         = 0xffc9
XK_L2                          = 0xffc9
XK_F13                         = 0xffca
XK_L3                          = 0xffca
XK_F14                         = 0xffcb
XK_L4                          = 0xffcb
XK_F15                         = 0xffcc
XK_L5                          = 0xffcc
XK_F16                         = 0xffcd
XK_L6                          = 0xffcd
XK_F17                         = 0xffce
XK_L7                          = 0xffce
XK_F18                         = 0xffcf
XK_L8                          = 0xffcf
XK_F19                         = 0xffd0
XK_L9                          = 0xffd0
XK_F20                         = 0xffd1
XK_L10                         = 0xffd1
XK_F21                         = 0xffd2
XK_R1                          = 0xffd2
XK_F22                         = 0xffd3
XK_R2                          = 0xffd3
XK_F23                         = 0xffd4
XK_R3                          = 0xffd4
XK_F24                         = 0xffd5
XK_R4                          = 0xffd5
XK_F25                         = 0xffd6
XK_R5                          = 0xffd6
XK_F26                         = 0xffd7
XK_R6                          = 0xffd7
XK_F27                         = 0xffd8
XK_R7                          = 0xffd8
XK_F28                         = 0xffd9
XK_R8                          = 0xffd9
XK_F29                         = 0xffda
XK_R9                          = 0xffda
XK_F30                         = 0xffdb
XK_R10                         = 0xffdb
XK_F31                         = 0xffdc
XK_R11                         = 0xffdc
XK_F32                         = 0xffdd
XK_R12                         = 0xffdd
XK_F33                         = 0xffde
XK_R13                         = 0xffde
XK_F34                         = 0xffdf
XK_R14                         = 0xffdf
XK_F35                         = 0xffe0
XK_R15                         = 0xffe0
XK_Shift_L                     = 0xffe1  --/* Left shift */
XK_Shift_R                     = 0xffe2  --/* Right shift */
XK_Control_L                   = 0xffe3  --/* Left control */
XK_Control_R                   = 0xffe4  --/* Right control */
XK_Caps_Lock                   = 0xffe5  --/* Caps lock */
XK_Shift_Lock                  = 0xffe6  --/* Shift lock */
XK_Meta_L                      = 0xffe7  --/* Left meta */
XK_Meta_R                      = 0xffe8  --/* Right meta */
XK_Alt_L                       = 0xffe9  --/* Left alt */
XK_Alt_R                       = 0xffea  --/* Right alt */
XK_Super_L                     = 0xffeb  --/* Left super */
XK_Super_R                     = 0xffec  --/* Right super */
XK_Hyper_L                     = 0xffed  --/* Left hyper */
XK_Hyper_R                     = 0xffee  --/* Right hyper */
XK_space                       = 0x0020  --/* U+0020 SPACE */
XK_exclam                      = 0x0021  --/* U+0021 EXCLAMATION MARK */
XK_quotedbl                    = 0x0022  --/* U+0022 QUOTATION MARK */
XK_numbersign                  = 0x0023  --/* U+0023 NUMBER SIGN */
XK_dollar                      = 0x0024  --/* U+0024 DOLLAR SIGN */
XK_percent                     = 0x0025  --/* U+0025 PERCENT SIGN */
XK_ampersand                   = 0x0026  --/* U+0026 AMPERSAND */
XK_apostrophe                  = 0x0027  --/* U+0027 APOSTROPHE */
XK_quoteright                  = 0x0027  --/* deprecated */
XK_parenleft                   = 0x0028  --/* U+0028 LEFT PARENTHESIS */
XK_parenright                  = 0x0029  --/* U+0029 RIGHT PARENTHESIS */
XK_asterisk                    = 0x002a  --/* U+002A ASTERISK */
XK_plus                        = 0x002b  --/* U+002B PLUS SIGN */
XK_comma                       = 0x002c  --/* U+002C COMMA */
XK_minus                       = 0x002d  --/* U+002D HYPHEN-MINUS */
XK_period                      = 0x002e  --/* U+002E FULL STOP */
XK_slash                       = 0x002f  --/* U+002F SOLIDUS */
XK_0                           = 0x0030  --/* U+0030 DIGIT ZERO */
XK_1                           = 0x0031  --/* U+0031 DIGIT ONE */
XK_2                           = 0x0032  --/* U+0032 DIGIT TWO */
XK_3                           = 0x0033  --/* U+0033 DIGIT THREE */
XK_4                           = 0x0034  --/* U+0034 DIGIT FOUR */
XK_5                           = 0x0035  --/* U+0035 DIGIT FIVE */
XK_6                           = 0x0036  --/* U+0036 DIGIT SIX */
XK_7                           = 0x0037  --/* U+0037 DIGIT SEVEN */
XK_8                           = 0x0038  --/* U+0038 DIGIT EIGHT */
XK_9                           = 0x0039  --/* U+0039 DIGIT NINE */
XK_colon                       = 0x003a  --/* U+003A COLON */
XK_semicolon                   = 0x003b  --/* U+003B SEMICOLON */
XK_less                        = 0x003c  --/* U+003C LESS-THAN SIGN */
XK_equal                       = 0x003d  --/* U+003D EQUALS SIGN */
XK_greater                     = 0x003e  --/* U+003E GREATER-THAN SIGN */
XK_question                    = 0x003f  --/* U+003F QUESTION MARK */
XK_at                          = 0x0040  --/* U+0040 COMMERCIAL AT */
XK_A                           = 0x0041  --/* U+0041 LATIN CAPITAL LETTER A */
XK_B                           = 0x0042  --/* U+0042 LATIN CAPITAL LETTER B */
XK_C                           = 0x0043  --/* U+0043 LATIN CAPITAL LETTER C */
XK_D                           = 0x0044  --/* U+0044 LATIN CAPITAL LETTER D */
XK_E                           = 0x0045  --/* U+0045 LATIN CAPITAL LETTER E */
XK_F                           = 0x0046  --/* U+0046 LATIN CAPITAL LETTER F */
XK_G                           = 0x0047  --/* U+0047 LATIN CAPITAL LETTER G */
XK_H                           = 0x0048  --/* U+0048 LATIN CAPITAL LETTER H */
XK_I                           = 0x0049  --/* U+0049 LATIN CAPITAL LETTER I */
XK_J                           = 0x004a  --/* U+004A LATIN CAPITAL LETTER J */
XK_K                           = 0x004b  --/* U+004B LATIN CAPITAL LETTER K */
XK_L                           = 0x004c  --/* U+004C LATIN CAPITAL LETTER L */
XK_Shift_L                     = 0xffe1  --/* Left shift */
XK_Shift_R                     = 0xffe2  --/* Right shift */
XK_Control_L                   = 0xffe3  --/* Left control */
XK_Control_R                   = 0xffe4  --/* Right control */
XK_Caps_Lock                   = 0xffe5  --/* Caps lock */
XK_Shift_Lock                  = 0xffe6  --/* Shift lock */
XK_Meta_L                      = 0xffe7  --/* Left meta */
XK_Meta_R                      = 0xffe8  --/* Right meta */
XK_Alt_L                       = 0xffe9  --/* Left alt */
XK_Alt_R                       = 0xffea  --/* Right alt */
XK_Super_L                     = 0xffeb  --/* Left super */
XK_Super_R                     = 0xffec  --/* Right super */
XK_Hyper_L                     = 0xffed  --/* Left hyper */
XK_Hyper_R                     = 0xffee  --/* Right hyper */
XK_space                       = 0x0020  --/* U+0020 SPACE */
XK_exclam                      = 0x0021  --/* U+0021 EXCLAMATION MARK */
XK_quotedbl                    = 0x0022  --/* U+0022 QUOTATION MARK */
XK_numbersign                  = 0x0023  --/* U+0023 NUMBER SIGN */
XK_dollar                      = 0x0024  --/* U+0024 DOLLAR SIGN */
XK_percent                     = 0x0025  --/* U+0025 PERCENT SIGN */
XK_ampersand                   = 0x0026  --/* U+0026 AMPERSAND */
XK_apostrophe                  = 0x0027  --/* U+0027 APOSTROPHE */
XK_quoteright                  = 0x0027  --/* deprecated */
XK_parenleft                   = 0x0028  --/* U+0028 LEFT PARENTHESIS */
XK_parenright                  = 0x0029  --/* U+0029 RIGHT PARENTHESIS */
XK_asterisk                    = 0x002a  --/* U+002A ASTERISK */
XK_plus                        = 0x002b  --/* U+002B PLUS SIGN */
XK_comma                       = 0x002c  --/* U+002C COMMA */
XK_minus                       = 0x002d  --/* U+002D HYPHEN-MINUS */
XK_period                      = 0x002e  --/* U+002E FULL STOP */
XK_slash                       = 0x002f  --/* U+002F SOLIDUS */
XK_0                           = 0x0030  --/* U+0030 DIGIT ZERO */
XK_1                           = 0x0031  --/* U+0031 DIGIT ONE */
XK_2                           = 0x0032  --/* U+0032 DIGIT TWO */
XK_3                           = 0x0033  --/* U+0033 DIGIT THREE */
XK_4                           = 0x0034  --/* U+0034 DIGIT FOUR */
XK_5                           = 0x0035  --/* U+0035 DIGIT FIVE */
XK_6                           = 0x0036  --/* U+0036 DIGIT SIX */
XK_7                           = 0x0037  --/* U+0037 DIGIT SEVEN */
XK_8                           = 0x0038  --/* U+0038 DIGIT EIGHT */
XK_9                           = 0x0039  --/* U+0039 DIGIT NINE */
XK_colon                       = 0x003a  --/* U+003A COLON */
XK_semicolon                   = 0x003b  --/* U+003B SEMICOLON */
XK_less                        = 0x003c  --/* U+003C LESS-THAN SIGN */
XK_equal                       = 0x003d  --/* U+003D EQUALS SIGN */
XK_greater                     = 0x003e  --/* U+003E GREATER-THAN SIGN */
XK_question                    = 0x003f  --/* U+003F QUESTION MARK */
XK_at                          = 0x0040  --/* U+0040 COMMERCIAL AT */
XK_A                           = 0x0041  --/* U+0041 LATIN CAPITAL LETTER A */
XK_B                           = 0x0042  --/* U+0042 LATIN CAPITAL LETTER B */
XK_C                           = 0x0043  --/* U+0043 LATIN CAPITAL LETTER C */
XK_D                           = 0x0044  --/* U+0044 LATIN CAPITAL LETTER D */
XK_E                           = 0x0045  --/* U+0045 LATIN CAPITAL LETTER E */
XK_F                           = 0x0046  --/* U+0046 LATIN CAPITAL LETTER F */
XK_G                           = 0x0047  --/* U+0047 LATIN CAPITAL LETTER G */
XK_H                           = 0x0048  --/* U+0048 LATIN CAPITAL LETTER H */
XK_I                           = 0x0049  --/* U+0049 LATIN CAPITAL LETTER I */
XK_J                           = 0x004a  --/* U+004A LATIN CAPITAL LETTER J */
XK_K                           = 0x004b  --/* U+004B LATIN CAPITAL LETTER K */
XK_L                           = 0x004c  --/* U+004C LATIN CAPITAL LETTER L */
XK_M                           = 0x004d  --/* U+004D LATIN CAPITAL LETTER M */
XK_N                           = 0x004e  --/* U+004E LATIN CAPITAL LETTER N */
XK_O                           = 0x004f  --/* U+004F LATIN CAPITAL LETTER O */
XK_P                           = 0x0050  --/* U+0050 LATIN CAPITAL LETTER P */
XK_Q                           = 0x0051  --/* U+0051 LATIN CAPITAL LETTER Q */
XK_R                           = 0x0052  --/* U+0052 LATIN CAPITAL LETTER R */
XK_S                           = 0x0053  --/* U+0053 LATIN CAPITAL LETTER S */
XK_T                           = 0x0054  --/* U+0054 LATIN CAPITAL LETTER T */
XK_U                           = 0x0055  --/* U+0055 LATIN CAPITAL LETTER U */
XK_V                           = 0x0056  --/* U+0056 LATIN CAPITAL LETTER V */
XK_W                           = 0x0057  --/* U+0057 LATIN CAPITAL LETTER W */
XK_X                           = 0x0058  --/* U+0058 LATIN CAPITAL LETTER X */
XK_Y                           = 0x0059  --/* U+0059 LATIN CAPITAL LETTER Y */
XK_Z                           = 0x005a  --/* U+005A LATIN CAPITAL LETTER Z */
XK_bracketleft                 = 0x005b  --/* U+005B LEFT SQUARE BRACKET */
XK_backslash                   = 0x005c  --/* U+005C REVERSE SOLIDUS */
XK_bracketright                = 0x005d  --/* U+005D RIGHT SQUARE BRACKET */
XK_asciicircum                 = 0x005e  --/* U+005E CIRCUMFLEX ACCENT */
XK_underscore                  = 0x005f  --/* U+005F LOW LINE */
XK_grave                       = 0x0060  --/* U+0060 GRAVE ACCENT */
XK_quoteleft                   = 0x0060  --/* deprecated */
XK_a                           = 0x0061  --/* U+0061 LATIN SMALL LETTER A */
XK_b                           = 0x0062  --/* U+0062 LATIN SMALL LETTER B */
XK_c                           = 0x0063  --/* U+0063 LATIN SMALL LETTER C */
XK_d                           = 0x0064  --/* U+0064 LATIN SMALL LETTER D */
XK_e                           = 0x0065  --/* U+0065 LATIN SMALL LETTER E */
XK_f                           = 0x0066  --/* U+0066 LATIN SMALL LETTER F */
XK_g                           = 0x0067  --/* U+0067 LATIN SMALL LETTER G */
XK_h                           = 0x0068  --/* U+0068 LATIN SMALL LETTER H */
XK_i                           = 0x0069  --/* U+0069 LATIN SMALL LETTER I */
XK_j                           = 0x006a  --/* U+006A LATIN SMALL LETTER J */
XK_k                           = 0x006b  --/* U+006B LATIN SMALL LETTER K */
XK_l                           = 0x006c  --/* U+006C LATIN SMALL LETTER L */
XK_m                           = 0x006d  --/* U+006D LATIN SMALL LETTER M */
XK_n                           = 0x006e  --/* U+006E LATIN SMALL LETTER N */
XK_o                           = 0x006f  --/* U+006F LATIN SMALL LETTER O */
XK_p                           = 0x0070  --/* U+0070 LATIN SMALL LETTER P */
XK_q                           = 0x0071  --/* U+0071 LATIN SMALL LETTER Q */
XK_r                           = 0x0072  --/* U+0072 LATIN SMALL LETTER R */
XK_s                           = 0x0073  --/* U+0073 LATIN SMALL LETTER S */
XK_t                           = 0x0074  --/* U+0074 LATIN SMALL LETTER T */
XK_u                           = 0x0075  --/* U+0075 LATIN SMALL LETTER U */
XK_v                           = 0x0076  --/* U+0076 LATIN SMALL LETTER V */
XK_w                           = 0x0077  --/* U+0077 LATIN SMALL LETTER W */
XK_x                           = 0x0078  --/* U+0078 LATIN SMALL LETTER X */
XK_y                           = 0x0079  --/* U+0079 LATIN SMALL LETTER Y */
XK_z                           = 0x007a  --/* U+007A LATIN SMALL LETTER Z */
XK_braceleft                   = 0x007b  --/* U+007B LEFT CURLY BRACKET */
XK_bar                         = 0x007c  --/* U+007C VERTICAL LINE */
XK_braceright                  = 0x007d  --/* U+007D RIGHT CURLY BRACKET */
XK_asciitilde                  = 0x007e  --/* U+007E TILDE */

--[[ Setup the state of the lights ]]--
--The lights are controlled by sending a control signal with the type of
--signal either ON(0x4A), OFF(0x4B), BLINK(0x4C), to a light value.
--eg. {0xBn, signal, light}
--or  {0xB0, 0x4A, 0x5C}  - switches the play light on.
--we'll set the default state for all lights to OFF aka 0x4B
-- for the FIXME's here the dimmers define additional state
-- information for the lights that replace existing state info.
-- so for these specific lights there is:
-- off, dim_flash, dim_on, flash, on = 5 states. how to manage that
-- is not currently clear but may present a solution over time.
controller = {}
-- friendly view
controller = {
    light = {
            DRY_WET_KNOB_FX1 = 0x5C,
            DRY_WET_KNOB_FX2 = 0x60,
            PARAM_1_KNOB_FX1 = 0x5D,
            PARAM_1_KNOB_FX2 = 0x61,
            PARAM_2_KNOB_FX1 = 0x5E,
            PARAM_2_KNOB_FX2 = 0x62,
            PARAM_3_KNOB_FX1 = 0x5F,
            PARAM_3_KNOB_FX2 = 0x63,
                     FX_ON_1 = 0x5A,
                     FX_ON_2 = 0x5B,
                    KEY_LOCK = 0x08,
                        SYNC = 0x09,
                        CUE1 = 0x11,
                 CUE1_Dimmer = 0x12, --FIXME
                        CUE2 = 0x13,
                 CUE2_Dimmer = 0x14, --FIXME
                        CUE3 = 0x15,
                 CUE3_Dimmer = 0x16, --FIXME
                        CUE4 = 0x17,
                 CUE4_Dimmer = 0x18, --FIXME
                        CUE5 = 0x19,
                 CUE5_Dimmer = 0x1A, --FIXME
                        CUE6 = 0x1B,
                 CUE6_Dimmer = 0x1C, --FIXME
                        CUE7 = 0x1D,
                 CUE7_Dimmer = 0x1F, --FIXME
                        CUE8 = 0x20,
                 CUE8_Dimmer = 0x21, --FIXME
                        PLAY = 0x27,
                         CUE = 0x26,
                     LOOP_IN = 0x24,
              LOOP_IN_Dimmer = 0x3E, --FIXME
                    LOOP_OUT = 0x40,
             LOOP_OUT_Dimmer = 0x2A, --FIXME
                   AUTO_LOOP = 0x2B,
            AUTO_LOOP_Dimmer = 0x53, --FIXME
             },

    state = {
        deckA = {
            DRY_WET_KNOB_FX1 = { value = 0x4B }, -- light = 0x5C
            DRY_WET_KNOB_FX2 = { value = 0x4B }, -- light = 0x60
            PARAM_1_KNOB_FX1 = { value = 0x4B }, -- light = 0x5D
            PARAM_1_KNOB_FX2 = { value = 0x4B }, -- light = 0x61
            PARAM_2_KNOB_FX1 = { value = 0x4B }, -- light = 0x5E
            PARAM_2_KNOB_FX2 = { value = 0x4B }, -- light = 0x62
            PARAM_3_KNOB_FX1 = { value = 0x4B }, -- light = 0x5F
            PARAM_3_KNOB_FX2 = { value = 0x4B }, -- light = 0x63
                     FX_ON_1 = { value = 0x4B }, -- light = 0x5A
                     FX_ON_2 = { value = 0x4B }, -- light = 0x5B
                    KEY_LOCK = { value = 0x4B }, -- light = 0x08
                        SYNC = { value = 0x4B }, -- light = 0x09
                     LOOP_IN = { value = 0x4B }, -- light = 0x24
                    LOOP_OUT = { value = 0x4B }, -- light = 0x40
                   AUTO_LOOP = { value = 0x4B }, -- light = 0x2B
                        CUE1 = { value = 0x4B }, -- light = 0x11
                        CUE2 = { value = 0x4B }, -- light = 0x13
                        CUE3 = { value = 0x4B }, -- light = 0x15
                        CUE4 = { value = 0x4B }, -- light = 0x17
                        CUE5 = { value = 0x4B }, -- light = 0x19
                        CUE6 = { value = 0x4B }, -- light = 0x1B
                        CUE7 = { value = 0x4B }, -- light = 0x1D
                        CUE8 = { value = 0x4B }, -- light = 0x20
                         CUE = { value = 0x4B }, -- light = 0x26
                        PLAY = { value = 0x4B }, -- light = 0x27
        },
        deckB = {
            DRY_WET_KNOB_FX1 = { value = 0x4B }, -- light = 0x5C
            DRY_WET_KNOB_FX2 = { value = 0x4B }, -- light = 0x60
            PARAM_1_KNOB_FX1 = { value = 0x4B }, -- light = 0x5D
            PARAM_1_KNOB_FX2 = { value = 0x4B }, -- light = 0x61
            PARAM_2_KNOB_FX1 = { value = 0x4B }, -- light = 0x5E
            PARAM_2_KNOB_FX2 = { value = 0x4B }, -- light = 0x62
            PARAM_3_KNOB_FX1 = { value = 0x4B }, -- light = 0x5F
            PARAM_3_KNOB_FX2 = { value = 0x4B }, -- light = 0x63
                     FX_ON_1 = { value = 0x4B }, -- light = 0x5A
                     FX_ON_2 = { value = 0x4B }, -- light = 0x5B
                    KEY_LOCK = { value = 0x4B }, -- light = 0x08
                        SYNC = { value = 0x4B }, -- light = 0x09
                     LOOP_IN = { value = 0x4B }, -- light = 0x24
                    LOOP_OUT = { value = 0x4B }, -- light = 0x40
                   AUTO_LOOP = { value = 0x4B }, -- light = 0x2B
                        CUE1 = { value = 0x4B }, -- light = 0x11
                        CUE2 = { value = 0x4B }, -- light = 0x13
                        CUE3 = { value = 0x4B }, -- light = 0x15
                        CUE4 = { value = 0x4B }, -- light = 0x17
                        CUE5 = { value = 0x4B }, -- light = 0x19
                        CUE6 = { value = 0x4B }, -- light = 0x1B
                        CUE7 = { value = 0x4B }, -- light = 0x1D
                        CUE8 = { value = 0x4B }, -- light = 0x20
                         CUE = { value = 0x4B }, -- light = 0x26
                        PLAY = { value = 0x4B }, -- light = 0x27

        },
    }
}

--midi view
--since the light control sequences bear no relation to the incoming midi event
--this aliasing comes in handt to check the state of a light based on what
--event was received.
controller.state.midi = {}
controller.state.midi[0x90] = { --deckA
    [0x15] = controller.state.deckA.DRY_WET_KNOB_FX1, --DRY_WET_SW_FX1
    [0x55] = controller.state.deckA.DRY_WET_KNOB_FX2, --DRY_WET_SW_FX2
    [0x12] = controller.state.deckA.PARAM_1_KNOB_FX1, --PARAM_1_SW_FX1
    [0x52] = controller.state.deckA.PARAM_1_KNOB_FX2, --PARAM_1_SW_FX2
    [0x13] = controller.state.deckA.PARAM_2_KNOB_FX1, --PARAM_2_SW_FX1
    [0x53] = controller.state.deckA.PARAM_2_KNOB_FX2, --PARAM_2_SW_FX2
    [0x14] = controller.state.deckA.PARAM_3_KNOB_FX1, --PARAM_3_SW_FX1
    [0x54] = controller.state.deckA.PARAM_3_KNOB_FX2, --PARAM_3_SW_FX2
    [0x56] = controller.state.deckA.FX_ON_1, --FX_ON_1
    [0x57] = controller.state.deckA.FX_ON_2, --FX_ON_2
    [0x06] = controller.state.deckA.KEY_LOCK, --KEY_LOCK
    [0x6B] = controller.state.deckA.SYNC, --SYNC
    [0x37] = controller.state.deckA.LOOP_IN, --LOOP_IN
    [0x39] = controller.state.deckA.LOOP_OUT, --LOOP_OUT
    [0x1D] = controller.state.deckA.AUTO_LOOP, --AUTO_LOOP
    [0x17] = controller.state.deckA.CUE1, --CUE1
    [0x18] = controller.state.deckA.CUE2, --CUE2
    [0x19] = controller.state.deckA.CUE3, --CUE3
    [0x20] = controller.state.deckA.CUE4, --CUE4
    [0x21] = controller.state.deckA.CUE5, --CUE5
    [0x22] = controller.state.deckA.CUE6, --CUE6
    [0x23] = controller.state.deckA.CUE7, --CUE7
    [0x24] = controller.state.deckA.CUE8, --CUE8
    [0x42] = controller.state.deckA.CUE, --CUE
    [0x43] = controller.state.deckA.PLAY, --PLAY
--
--
}
controller.state.midi[0x91] = {
    [0x15] = controller.state.deckA.DRY_WET_KNOB_FX1, --DRY_WET_SW_FX1
    [0x55] = controller.state.deckA.DRY_WET_KNOB_FX2, --DRY_WET_SW_FX2
    [0x12] = controller.state.deckA.PARAM_1_KNOB_FX1, --PARAM_1_SW_FX1
    [0x52] = controller.state.deckA.PARAM_1_KNOB_FX2, --PARAM_1_SW_FX2
    [0x13] = controller.state.deckA.PARAM_2_KNOB_FX1, --PARAM_2_SW_FX1
    [0x53] = controller.state.deckA.PARAM_2_KNOB_FX2, --PARAM_2_SW_FX2
    [0x14] = controller.state.deckA.PARAM_3_KNOB_FX1, --PARAM_3_SW_FX1
    [0x54] = controller.state.deckA.PARAM_3_KNOB_FX2, --PARAM_3_SW_FX2
    [0x56] = controller.state.deckA.FX_ON_1, --FX_ON_1
    [0x57] = controller.state.deckA.FX_ON_2, --FX_ON_2
    [0x06] = controller.state.deckA.KEY_LOCK, --KEY_LOCK
    [0x6B] = controller.state.deckA.SYNC, --SYNC
    [0x37] = controller.state.deckA.LOOP_IN, --LOOP_IN
    [0x39] = controller.state.deckA.LOOP_OUT, --LOOP_OUT
    [0x1D] = controller.state.deckA.AUTO_LOOP, --AUTO_LOOP
    [0x17] = controller.state.deckA.CUE1, --CUE1
    [0x18] = controller.state.deckA.CUE2, --CUE2
    [0x19] = controller.state.deckA.CUE3, --CUE3
    [0x20] = controller.state.deckA.CUE4, --CUE4
    [0x21] = controller.state.deckA.CUE5, --CUE5
    [0x22] = controller.state.deckA.CUE6, --CUE6
    [0x23] = controller.state.deckA.CUE7, --CUE7
    [0x24] = controller.state.deckA.CUE8, --CUE8
    [0x42] = controller.state.deckA.CUE, --CUE
    [0x43] = controller.state.deckA.PLAY, --PLAY
}

--[[ Key Maps ]]--
default = {}

-- toggle lights
-- signal either ON(0x4A), OFF(0x4B), BLINK(0x4C), to a light value.
function default.toggle( event, light )
    print( event_to_string( event ) .. " - Toggle" )
    local light_state = controller.state.midi[event[1]][event[2]]
    if( light_state.value == 0x4A )
    then light_state.value = 0x4B
    elseif( light_state.value == 0x4B )
    then light_state.value = 0x4C
    elseif( light_state.value == 0x4C )
    then light_state.value = 0x4A
    end
    --print( 0xB1 & 0xF0 )
    midi_send( {0xB0, light_state.value, light } )
end

function unassigned( event )
    print( event_to_string( event ) .. " - Unassigned" )
end

-- use deck a as scroll wheel
jogdial_counter = 0
function default.jogdial_turn( event )
    if( jogdial_counter < 10 ) then
        jogdial_counter = jogdial_counter + 1
        return
    else
        jogdial_counter = 0
    end

    if event[3] < 64 then
        buttonpress( 4 )
    end
    if event[3] > 64 then
        buttonpress( 5 )
    end
end

function default.pitchbend( event )
    print( event_to_string( event ) .. " - unassigned" )
end

local pulse_master_volume_value = 0
local pulse_master_volume_ms = milliseconds()
function default.pulse_master_volume( event )
    if( milliseconds() - pulse_master_volume_ms < 25 )then
        return
    end
    pulse_master_volume_ms = milliseconds()

    temp = math.floor( (event[3] / 2.550) )
    if( pulse_master_volume_value == temp) then
        return
    end
    pulse_master_volume_value = temp
    exec("amixer -D pulse sset Master " .. temp .. "%" )
end

function default.clementine_np( event )
    if( event[3] > 65 ) then
        exec( "clementine -r" )
    else
        exec( "clementine -f" )
    end
end

function default.clementine_playpause( event )
    exec( "clementine -t" )
end


default.map = {}

--deckA
default.map[0x90] = {
    [0x58] = { ['*'] = { unassigned } }, -- FX_UNIT
    [0x03] = { ['*'] = { unassigned } }, -- DECK_CHG
    [0x15] = { ['*'] = { default.toggle, controller.light.DRY_WET_KNOB_FX1 } }, -- DRY_WET_SW_FX1
    [0x55] = { ['*'] = { default.toggle, controller.light.DRY_WET_KNOB_FX2 } }, -- DRY_WET_SW_FX2
    [0x64] = { ['*'] = { unassigned } }, -- BROWSER
    [0x30] = { ['*'] = { unassigned } }, -- PREVIOUS
    [0x12] = { ['*'] = { default.toggle, controller.light.PARAM_1_KNOB_FX1 } }, -- PARAM_1_SW_FX1
    [0x52] = { ['*'] = { default.toggle, controller.light.PARAM_1_KNOB_FX2 } }, -- PARAM_1_SW_FX2
    [0x28] = { ['*'] = { unassigned } }, -- TRACK_SELECT_KNOB_SW
    [0x13] = { ['*'] = { default.toggle, controller.light.PARAM_2_KNOB_FX1 } }, -- PARAM_2_SW_FX1
    [0x53] = { ['*'] = { default.toggle, controller.light.PARAM_2_KNOB_FX2 } }, -- PARAM_2_SW_FX2
    [0x29] = { ['*'] = { unassigned } }, -- NEXT
    [0x65] = { ['*'] = { unassigned } }, -- DUPLICATE
    [0x14] = { ['*'] = { default.toggle, controller.light.PARAM_3_KNOB_FX1 } }, -- PARAM_3_SW_FX1
    [0x54] = { ['*'] = { default.toggle, controller.light.PARAM_3_KNOB_FX2 } }, -- PARAM_3_SW_FX2
    [0x0D] = { ['*'] = { unassigned } }, -- BEND_MINUS
    [0x56] = { ['*'] = { default.toggle, controller.light.FX_ON_1 } }, -- FX_ON_1
    [0x06] = { ['*'] = { default.toggle, controller.light.KEY_LOCK } }, -- KEY_LOCK
    [0x57] = { ['*'] = { default.toggle, controller.light.FX_ON_2 } }, -- FX_ON_2
    [0x6B] = { ['*'] = { default.toggle, controller.light.SYNC } }, -- SYNC
    [0x0C] = { ['*'] = { unassigned } }, -- BEND_PLUS
    [0x17] = { ['*'] = { default.toggle, controller.light.CUE1 } }, -- CUE1
    [0x18] = { ['*'] = { default.toggle, controller.light.CUE2 } }, -- CUE2
    [0x19] = { ['*'] = { default.toggle, controller.light.CUE3 } }, -- CUE3
    [0x20] = { ['*'] = { default.toggle, controller.light.CUE4 } }, -- CUE4
    [0x21] = { ['*'] = { default.toggle, controller.light.CUE5 } }, -- CUE5
    [0x22] = { ['*'] = { default.toggle, controller.light.CUE6 } }, -- CUE6
    [0x23] = { ['*'] = { default.toggle, controller.light.CUE7 } }, -- CUE7
    [0x24] = { ['*'] = { default.toggle, controller.light.CUE8 } }, -- CUE8
    [0x60] = { ['*'] = { unassigned } }, -- CUE_LOOP_DELETE_SHIFT
    [0x11] = { ['*'] = { unassigned } }, -- FAST_SEARCH_MINUS
    [0x43] = { ['*'] = { default.toggle, controller.light.PLAY } }, -- PLAY
    [0x42] = { ['*'] = { default.toggle, controller.light.CUE } }, -- CUE
    [0x10] = { ['*'] = { unassigned } }, -- FAST_SEARCH_PLUS
    [0x37] = { ['*'] = { default.toggle, controller.light.LOOP_IN } }, -- LOOP_IN
    [0x39] = { ['*'] = { default.toggle, controller.light.LOOP_OUT } }, -- LOOP_OUT
    [0x69] = { ['*'] = { unassigned } }, -- BEAT_MINUS
    [0x1D] = { ['*'] = { default.toggle, controller.light.AUTO_LOOP } }, -- AUTO_LOOP
    [0x6A] = { ['*'] = { unassigned } }, -- BEAT_PLUS
    [0x68] = { ['*'] = { unassigned } }, -- FILTER_SW
    [0x1A] = { ['*'] = { unassigned } }, -- SEL
}
default.map[0x91] = {
    [0x58] = { ['*'] = { unassigned } }, -- FX_UNIT
    [0x03] = { ['*'] = { unassigned } }, -- DECK_CHG
    [0x15] = { ['*'] = { default.toggle, controller.light.DRY_WET_KNOB_FX1 } }, -- DRY_WET_SW_FX1
    [0x55] = { ['*'] = { default.toggle, controller.light.DRY_WET_KNOB_FX2 } }, -- DRY_WET_SW_FX2
    [0x64] = { ['*'] = { unassigned } }, -- BROWSER
    [0x30] = { ['*'] = { unassigned } }, -- PREVIOUS
    [0x12] = { ['*'] = { default.toggle, controller.light.PARAM_1_KNOB_FX1 } }, -- PARAM_1_SW_FX1
    [0x52] = { ['*'] = { default.toggle, controller.light.PARAM_1_KNOB_FX2 } }, -- PARAM_1_SW_FX2
    [0x28] = { ['*'] = { unassigned } }, -- TRACK_SELECT_KNOB_SW
    [0x13] = { ['*'] = { default.toggle, controller.light.PARAM_2_KNOB_FX1 } }, -- PARAM_2_SW_FX1
    [0x53] = { ['*'] = { default.toggle, controller.light.PARAM_2_KNOB_FX2 } }, -- PARAM_2_SW_FX2
    [0x29] = { ['*'] = { unassigned } }, -- NEXT
    [0x65] = { ['*'] = { unassigned } }, -- DUPLICATE
    [0x14] = { ['*'] = { default.toggle, controller.light.PARAM_3_KNOB_FX1 } }, -- PARAM_3_SW_FX1
    [0x54] = { ['*'] = { default.toggle, controller.light.PARAM_3_KNOB_FX2 } }, -- PARAM_3_SW_FX2
    [0x0D] = { ['*'] = { unassigned } }, -- BEND_MINUS
    [0x56] = { ['*'] = { default.toggle, controller.light.FX_ON_1 } }, -- FX_ON_1
    [0x06] = { ['*'] = { default.toggle, controller.light.KEY_LOCK } }, -- KEY_LOCK
    [0x57] = { ['*'] = { default.toggle, controller.light.FX_ON_2 } }, -- FX_ON_2
    [0x6B] = { ['*'] = { default.toggle, controller.light.SYNC } }, -- SYNC
    [0x0C] = { ['*'] = { unassigned } }, -- BEND_PLUS
    [0x17] = { ['*'] = { default.toggle, controller.light.CUE1 } }, -- CUE1
    [0x18] = { ['*'] = { default.toggle, controller.light.CUE2 } }, -- CUE2
    [0x19] = { ['*'] = { default.toggle, controller.light.CUE3 } }, -- CUE3
    [0x20] = { ['*'] = { default.toggle, controller.light.CUE4 } }, -- CUE4
    [0x21] = { ['*'] = { default.toggle, controller.light.CUE5 } }, -- CUE5
    [0x22] = { ['*'] = { default.toggle, controller.light.CUE6 } }, -- CUE6
    [0x23] = { ['*'] = { default.toggle, controller.light.CUE7 } }, -- CUE7
    [0x24] = { ['*'] = { default.toggle, controller.light.CUE8 } }, -- CUE8
    [0x60] = { ['*'] = { unassigned } }, -- CUE_LOOP_DELETE_SHIFT
    [0x11] = { ['*'] = { unassigned } }, -- FAST_SEARCH_MINUS
    [0x43] = { ['*'] = { default.toggle, controller.light.PLAY } }, -- PLAY
    [0x42] = { ['*'] = { default.toggle, controller.light.CUE } }, -- CUE
    [0x10] = { ['*'] = { unassigned } }, -- FAST_SEARCH_PLUS
    [0x37] = { ['*'] = { default.toggle, controller.light.LOOP_IN } }, -- LOOP_IN
    [0x39] = { ['*'] = { default.toggle, controller.light.LOOP_OUT } }, -- LOOP_OUT
    [0x69] = { ['*'] = { unassigned } }, -- BEAT_MINUS
    [0x1D] = { ['*'] = { default.toggle, controller.light.AUTO_LOOP } }, -- AUTO_LOOP
    [0x6A] = { ['*'] = { unassigned } }, -- BEAT_PLUS
    [0x68] = { ['*'] = { unassigned } }, -- FILTER_SW
    [0x1A] = { ['*'] = { unassigned } }, -- SEL
}


default.map[0xB0] = {
    [0x55] = { [  0] = { unassigned },   --DRY_WET_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --DRY_WET_KNOB_FX1 - counter
    [0x56] = { [  0] = { unassigned },   --PARAM_1_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_1_KNOB_FX1 - counter
    [0x57] = { [  0] = { unassigned },   --PARAM_2_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_2_KNOB_FX1 - counter
    [0x58] = { [  0] = { unassigned },   --PARAM_3_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_3_KNOB_FX1 - counter

    [0x59] = { [  0] = { unassigned },   --DRY_WET_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --DRY_WET_KNOB_FX2 - counter
    [0x5a] = { [  0] = { unassigned },   --PARAM_1_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_1_KNOB_FX2 - counter
    [0x5b] = { [  0] = { unassigned },   --PARAM_2_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_2_KNOB_FX2 - counter
    [0x5c] = { [  0] = { unassigned },   --PARAM_3_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_3_KNOB_FX2 - counter

    [0x54] = { [  0] = { default.clementine_np },   --TRACK SELECT KNOB clockwise
               [127] = { default.clementine_np }, }, --TRACK SELECT KNOB counter
    [0x5D] = { [  0] = { unassigned },   --FILTER KNOB - clockwise
               [127] = { unassigned } }, --FILTER KNOB - counter
    [0x51] = { ['*'] = { default.jogdial_turn } }, --JOGWHEEL FWD/REV
}
default.map[0xB1] = {
    [0x55] = { [  0] = { unassigned },   --DRY_WET_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --DRY_WET_KNOB_FX1 - counter
    [0x56] = { [  0] = { unassigned },   --PARAM_1_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_1_KNOB_FX1 - counter
    [0x57] = { [  0] = { unassigned },   --PARAM_2_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_2_KNOB_FX1 - counter
    [0x58] = { [  0] = { unassigned },   --PARAM_3_KNOB_FX1 - clockwise
               [127] = { unassigned } }, --PARAM_3_KNOB_FX1 - counter

    [0x59] = { [  0] = { unassigned },   --DRY_WET_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --DRY_WET_KNOB_FX2 - counter
    [0x5a] = { [  0] = { unassigned },   --PARAM_1_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_1_KNOB_FX2 - counter
    [0x5b] = { [  0] = { unassigned },   --PARAM_2_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_2_KNOB_FX2 - counter
    [0x5c] = { [  0] = { unassigned },   --PARAM_3_KNOB_FX2 - clockwise
               [127] = { unassigned } }, --PARAM_3_KNOB_FX2 - counter

    [0x54] = { [  0] = { unassigned },   --TRACK SELECT KNOB clockwise
               [127] = { unassigned } }, --TRACK SELECT KNOB counter
    [0x5D] = { [  0] = { unassigned },   --FILTER KNOB - clockwise
               [127] = { unassigned } }, --FILTER KNOB - counter
    [0x51] = { ['*'] = { default.jogdial_turn } }, --JOGWHEEL FWD/REV
}

--Pitch Bend
default.map[0xE0] = {
    [0x00] = { ['*'] = { default.pulse_master_volume } },
}
default.map[0xE1] = {
    [  0] = { ['*'] = { default.pitchbend } },
}
-- Pitch bend
 --   [] = { [] = { } }, --PITCH SLIDER 0xEn 0xllx(LSB) 0xmm (MSB)

--[[ application specific maps ]]--
--[[===========================]]--

--[[[ Lightworks ]]--
lightworks = {}
--the WM_CLASS for lightworrks isnt actually 'lightworks', its 'nvcardvt'
--so we'll make it an alias
ntcardvt = lightworks

function lightworks.autoloop_turn( event )
    if event[3] > 64 then
        keypress( XK_a )
    end
    if event[3] < 64 then
        keypress( XK_s )
    end
end

function lightworks.scratch_turn( event )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

lightworks.map = {}
lightworks.map[0x91] = {
    [0x0B] = { [127] = { kpress, XK_space } },
    [0x0C] = { [127] = { kpress, XK_i     } },
    [0x2E] = { [127] = { kpress, XK_x     } },
}
lightworks.map[0xB1] = {
    [0x13] = { ['*'] = { lightworks.autoloop_turn     } },
    [0x22] = { ['*'] = { lightworks.scratch_turn      } },
}

--[[ ffplay ]]--
ffplay = {}

function ffplay.scratch_turn( event )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

ffplay.map = {}
ffplay.map[0x91] = {
    [0x0B] = { [127] = { kpress, XK_space } },
}
ffplay.map[0xB1] = {
    [0x22] = { ['*'] = { ffplay.scratch_turn } },
}

--[[ kdenlive ]]--
kdenlive = {}

function kdenlive.scratch_turn( event )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

kdenlive.map = {}
kdenlive.map[0xB1] = {
    [0x22] = { ['*'] = { kdenlive.scratch_turn } },
    [0x13] = { [127] = { kpress, XK_A          },
               [  1] = { kpress, XK_S          } },
}
kdenlive.map[0x91] = {
    [0x0B] = { [127] = { kpress, XK_space } },
    [0x0C] = { [127] = { kpress, { XK_Shift_L, XK_8 } } },
    [0x14] = { [127] = { kpress, { XK_Alt_L, XK_8 } } },
}

--[[ olive editor ]]--
--
olive = {}
_G[ "olive-editor" ] = olive

olive.map = {}

olive.map[0x90] = {
}

olive.map[0x91] = {
    [0x37] = { ['*'] = { kpress, XK_i }, },
    [0x39] = { ['*'] = { kpress, XK_o }, },
    [0x43] = { ['*'] = { kpress, XK_space }, },
    [0x29] = { ['*'] = { kpress, XK_Down }, },
    [0x30] = { ['*'] = { kpress, XK_Up }, },
    [0x42] = { ['*'] = { kpress, XK_m }, },
    [0x60] = { ['*'] = { kpress, XK_apostrophe }, },
}

olive.map[0xB1] = {
    [0x54] = { [  0] = { kpress, XK_Right },
               [127] = { kpress, XK_Left  }, },
}


Blender = {}
Blender.func = {}

function Blender.func.scrub( value )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

Blender.map = {}

-- Navigate - BLUE
Blender.map[0x90] = {
    [0x42] = { ['*'] = { kpress, XK_m } },
    [0x43] = { ['*'] = { kpress, XK_space } },
    [0x29] = { ['*'] = { kpress, XK_Page_Up }, },
    [0x30] = { ['*'] = { kpress, XK_Page_Down }, },
    [0x10] = { ['*'] = { kpress, { XK_Shift_L, XK_period } }, },
    [0x11] = { ['*'] = { kpress, { XK_Shift_L, XK_comma } }, },
}
Blender.map[0xB0] = {
    [0x51] = { ['*'] = { Blender.func.scrub} },
    [0x54] = { [  0] = { kpress, XK_Page_Up },
               [127] = { kpress, XK_Page_Down  }, },
}
-- edit - RED
Blender.map[0x91] = {
}
Blender.map[0xB1] = {
}
--[[ Missing assigments from previous version ]]--
-- * dbus message sending to mediaplayer advertisement, honestly it was shit
--   house anyway because i would experiment with my mediaplayer too much
-- * disabling controls when mixx is playing, either way or..
-- * vlc controls
-- * mplayer controls
-- * generic alsa volume controls
--[[ Brainstorming things for future]]--
-- * each application having a default state so that when switching between i
--   can light up the buttons which have commands associated with them. or check
--   using dbus to query the state and make the deck show whats up. so that
--   would mean polling functions, or dbus subscriptions or some such.

--[[ control functions ]]--
--[[===================]]--
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
        if control[value] then
            action = control[value]
        elseif control['*'] then
            action = control['*']
        else
            return
        end

        --we have the action, so run it with its optional paremeter
        if action[2] then
            action[1]( event, action[2] )
        else
            action[1]( event )
        end
    end
end


