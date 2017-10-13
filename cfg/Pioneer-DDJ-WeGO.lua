--[[ Imported c++ Functions ]]--
--initialise()
--midi_recv( status, data1, data2 )
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
--wm_class
autoconnect = true

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
    --loop through the midi state map and set all the lights to their default
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
controller = {}
-- friendly view
controller = {
    state = {
        deckA = {
             play = { value = 0x00, default = value },
              cue = { value = 0x00 },
             sync = { value = 0x00 },
          hotcue1 = { value = 0x00 },
          hotcue2 = { value = 0x00 },
          hotcue3 = { value = 0x00 },
          hotcue4 = { value = 0x00 },
         sampler1 = { value = 0x00 },
         sampler2 = { value = 0x00 },
         sampler3 = { value = 0x00 },
         sampler4 = { value = 0x00 },
        headphone = { value = 0x00 },
            deckC = { value = 0x00 },
            ctrlA = { value = 0x00 },
            ctrlB = { value = 0x00 },
              fx1 = { value = 0x00 },
              fx2 = { value = 0x00 },
              fx3 = { value = 0x00 },
       ctrlAwheel = { value = 0x40 },
       ctrlBwheel = { value = 0x40 },
         fx1wheel = { value = 0x00 },
         fx2wheel = { value = 0x00 },
         fx3wheel = { value = 0x00 },
        },
        deckB = {
             play = { value = 0x00 },
              cue = { value = 0x00 },
             sync = { value = 0x00 },
          hotcue1 = { value = 0x00 },
          hotcue2 = { value = 0x00 },
          hotcue3 = { value = 0x00 },
          hotcue4 = { value = 0x00 },
         sampler1 = { value = 0x00 },
         sampler2 = { value = 0x00 },
         sampler3 = { value = 0x00 },
         sampler4 = { value = 0x00 },
        headphone = { value = 0x00 },
            deckD = { value = 0x00 },
            ctrlA = { value = 0x00 },
            ctrlB = { value = 0x00 },
              fx1 = { value = 0x00 },
              fx2 = { value = 0x00 },
              fx3 = { value = 0x00 },
       ctrlAwheel = { value = 0x40 },
       ctrlBwheel = { value = 0x40 },
         fx1wheel = { value = 0x00 },
         fx2wheel = { value = 0x00 },
         fx3wheel = { value = 0x00 },
        },
        deckC = {
             play = { value = 0x00 },
              cue = { value = 0x00 },
             sync = { value = 0x00 },
          hotcue1 = { value = 0x00 },
          hotcue2 = { value = 0x00 },
          hotcue3 = { value = 0x00 },
          hotcue4 = { value = 0x00 },
         sampler1 = { value = 0x00 },
         sampler2 = { value = 0x00 },
         sampler3 = { value = 0x00 },
         sampler4 = { value = 0x00 },
        headphone = { value = 0x00 },
            deckD = {},
            ctrlA = { value = 0x00 },
            ctrlB = { value = 0x00 },
              fx1 = { value = 0x00 },
              fx2 = { value = 0x00 },
              fx3 = { value = 0x00 },
       ctrlAwheel = { value = 0x40 },
       ctrlBwheel = { value = 0x40 },
         fx1wheel = { value = 0x00 },
         fx2wheel = { value = 0x00 },
         fx3wheel = { value = 0x00 },
        },
        deckD = {
             play = { value = 0x00 },
              cue = { value = 0x00 },
             sync = { value = 0x00 },
          hotcue1 = { value = 0x00 },
          hotcue2 = { value = 0x00 },
          hotcue3 = { value = 0x00 },
          hotcue4 = { value = 0x00 },
         sampler1 = { value = 0x00 },
         sampler2 = { value = 0x00 },
         sampler3 = { value = 0x00 },
         sampler4 = { value = 0x00 },
        headphone = { value = 0x00 },
            deckD = {},
            ctrlA = { value = 0x00 },
            ctrlB = { value = 0x00 },
              fx1 = { value = 0x00 },
              fx2 = { value = 0x00 },
              fx3 = { value = 0x00 },
       ctrlAwheel = { value = 0x40 },
       ctrlBwheel = { value = 0x40 },
         fx1wheel = { value = 0x00 },
         fx2wheel = { value = 0x00 },
         fx3wheel = { value = 0x00 },
        },
    }
}
controller.state.deckC.deckC = controller.state.deckA.deckC
controller.state.deckD.deckD = controller.state.deckB.deckD

--midi view
controller.state.midi = {}
controller.state.midi[0x90] = {
    [0x0B] = controller.state.deckA.play,
    [0x0C] = controller.state.deckA.cue,
    [0x2E] = controller.state.deckA.hotcue1,
    [0x2F] = controller.state.deckA.hotcue2,
    [0x30] = controller.state.deckA.hotcue3,
    [0x31] = controller.state.deckA.hotcue4,
    [0x3C] = controller.state.deckA.sampler1,
    [0x3E] = controller.state.deckA.sampler2,
    [0x40] = controller.state.deckA.sampler3,
    [0x42] = controller.state.deckA.sampler4,
    [0x58] = controller.state.deckA.sync,
}
controller.state.midi[0x91] = {
    [0x0B] = controller.state.deckB.play,
    [0x0C] = controller.state.deckB.cue,
    [0x2E] = controller.state.deckB.hotcue1,
    [0x2F] = controller.state.deckB.hotcue2,
    [0x30] = controller.state.deckB.hotcue3,
    [0x31] = controller.state.deckB.hotcue4,
    [0x3C] = controller.state.deckB.sampler1,
    [0x3E] = controller.state.deckB.sampler2,
    [0x40] = controller.state.deckB.sampler3,
    [0x42] = controller.state.deckB.sampler4,
    [0x58] = controller.state.deckB.sync,
}
controller.state.midi[0x92] = {
    [0x0B] = controller.state.deckC.play,
    [0x0C] = controller.state.deckC.cue,
    [0x2E] = controller.state.deckC.hotcue1,
    [0x2F] = controller.state.deckC.hotcue2,
    [0x30] = controller.state.deckC.hotcue3,
    [0x31] = controller.state.deckC.hotcue4,
    [0x3C] = controller.state.deckC.sampler1,
    [0x3E] = controller.state.deckC.sampler2,
    [0x40] = controller.state.deckC.sampler3,
    [0x42] = controller.state.deckC.sampler4,
    [0x58] = controller.state.deckC.sync,
    [0x72] = controller.state.deckC.deckC,
}
controller.state.midi[0x93] = {
    [0x0B] = controller.state.deckD.play,
    [0x0C] = controller.state.deckD.cue,
    [0x2E] = controller.state.deckD.hotcue1,
    [0x2F] = controller.state.deckD.hotcue2,
    [0x30] = controller.state.deckD.hotcue3,
    [0x31] = controller.state.deckD.hotcue4,
    [0x3C] = controller.state.deckD.sampler1,
    [0x3E] = controller.state.deckD.sampler2,
    [0x40] = controller.state.deckD.sampler3,
    [0x42] = controller.state.deckD.sampler4,
    [0x58] = controller.state.deckD.sync,
    [0x72] = controller.state.deckD.deckD,
}
controller.state.midi[0x94] = {
    [0x00] = controller.state.deckA.ctrlAwheel,
    [0x02] = controller.state.deckA.ctrlBwheel,
    [0x04] = controller.state.deckA.fx1wheel,
    [0x06] = controller.state.deckA.fx2wheel,
    [0x07] = controller.state.deckA.fx3wheel,
    [0x08] = controller.state.deckC.ctrlAwheel,
    [0x0A] = controller.state.deckC.ctrlBwheel,
    [0x0C] = controller.state.deckC.fx1wheel,
    [0x0E] = controller.state.deckC.fx2wheel,
    [0x0F] = controller.state.deckC.fx3wheel,
    [0x42] = controller.state.deckA.ctrlA,
    [0x43] = controller.state.deckA.fx1,
    [0x44] = controller.state.deckA.fx2,
    [0x45] = controller.state.deckA.fx3,
    [0x46] = controller.state.deckA.ctrlB,
    [0x47] = controller.state.deckC.ctrlA,
    [0x48] = controller.state.deckC.fx1,
    [0x49] = controller.state.deckC.fx2,
    [0x4A] = controller.state.deckC.fx3,
    [0x4B] = controller.state.deckC.ctrlB,
}
controller.state.midi[0x95] = {
    [0x00] = controller.state.deckB.ctrlAwheel,
    [0x02] = controller.state.deckB.ctrlBwheel,
    [0x04] = controller.state.deckB.fx1wheel,
    [0x06] = controller.state.deckB.fx2wheel,
    [0x07] = controller.state.deckB.fx3wheel,
    [0x08] = controller.state.deckD.ctrlAwheel,
    [0x0A] = controller.state.deckD.ctrlBwheel,
    [0x0C] = controller.state.deckD.fx1wheel,
    [0x0E] = controller.state.deckD.fx2wheel,
    [0x0F] = controller.state.deckD.fx3wheel,
    [0x42] = controller.state.deckB.ctrlA,
    [0x43] = controller.state.deckB.fx1,
    [0x44] = controller.state.deckB.fx2,
    [0x45] = controller.state.deckB.fx3,
    [0x46] = controller.state.deckB.ctrlB,
    [0x47] = controller.state.deckD.ctrlA,
    [0x48] = controller.state.deckD.fx1,
    [0x49] = controller.state.deckD.fx2,
    [0x4A] = controller.state.deckD.fx3,
    [0x4B] = controller.state.deckD.ctrlB,
}
controller.state.midi[0x96] = {
    [0x54] = controller.state.deckA.headphone,
    [0x55] = controller.state.deckB.headphone,
    [0x56] = controller.state.deckC.headphone,
    [0x57] = controller.state.deckD.headphone
}

--[[ Key Maps ]]--
default = {}

function default.toggle( event )
    local control = controller.state.midi[event[1]][event[2]]
    if( control.value == 0x00 )
    then control.value = 0x01
    else control.value = 0x00
    end
    midi_send( {event[1], event[2], control.value } )
end

function unassigned()
    print("This event is unassigned")
end

default.map = {}
--deckA
default.map[0x90] = {
    [0x0B] = { [127] = { default.toggle } }, --         play
    [0x0C] = { [127] = { default.toggle } }, --         cue
    [0x14] = { [127] = { unassigned     } }, --         autoloop
    [0x2E] = { [127] = { default.toggle } }, --         hotcue1
    [0x2F] = { [127] = { default.toggle } }, --         hotcue2
    [0x30] = { [127] = { default.toggle } }, --         hotcue3
    [0x31] = { [127] = { default.toggle } }, --         hotcue4
    [0x36] = { [127] = { unassigned     } }, --         touch
    [0x3C] = { [127] = { default.toggle } }, --         sampler1
    [0x3D] = { [127] = { unassigned     } }, -- [shift] sampler1
    [0x3E] = { [127] = { default.toggle } }, --         sampler2
    [0x3F] = { [127] = { unassigned     } }, -- [shift] sampler2
    [0x40] = { [127] = { default.toggle } }, --         sampler3
    [0x41] = { [127] = { unassigned     } }, -- [shift] sampler3
    [0x42] = { [127] = { default.toggle } }, --         sampler4
    [0x43] = { [127] = { unassigned     } }, -- [shift] sampler4
    [0x47] = { [127] = { unassigned     } }, -- [shift] play
    [0x48] = { [127] = { unassigned     } }, -- [shift] cue
    [0x50] = { [127] = { unassigned     } }, -- [shift] autoloop
    [0x58] = { [127] = { default.toggle } }, --         sync
    [0x59] = { [127] = { unassigned     } }, --         sampler
    [0x5C] = { [127] = { unassigned     } }, -- [shift] sync
    [0x5F] = { [127] = { unassigned     } }, -- [shift] hotcue1
    [0x60] = { [127] = { unassigned     } }, -- [shift] hotcue2
    [0x61] = { [127] = { unassigned     } }, -- [shift] hotcue3
    [0x62] = { [127] = { unassigned     } }, -- [shift] hotcue4
    [0x67] = { [127] = { unassigned     } }, -- [shift] touch
}
default.map[0xB0] = {
    [0x00] = { ['*'] = { MSB            } }, --         tempo MSB
    [0x05] = { ['*'] = { MSB            } }, -- [shift] tempo MSB
    [0x13] = { ['*'] = { unassigned     } }, --         auto loop turn
    [0x20] = { ['*'] = { unassigned     } }, --         tempo LSB
    [0x21] = { ['*'] = { unassigned     } }, --         jog wheel
    [0x22] = { ['*'] = { unassigned     } }, --         scratch
    [0x25] = { ['*'] = { unassigned     } }, -- [shift] tempo LSB
    [0x26] = { ['*'] = { unassigned     } }, -- [shift] jog wheel
    [0x27] = { ['*'] = { unassigned     } }, -- [shift] scratch
    [0x4F] = { ['*'] = { unassigned     } }, -- [shift] auto loop turn
}
--deckB
default.map[0x91] = {
    [0x0B] = { [127] = { default.toggle } }, --         play
    [0x0C] = { [127] = { default.toggle } }, --         cue
    [0x14] = { [127] = { unassigned     } }, --         autoloop
    [0x2E] = { [127] = { default.toggle } }, --         hotcue1
    [0x2F] = { [127] = { default.toggle } }, --         hotcue2
    [0x30] = { [127] = { default.toggle } }, --         hotcue3
    [0x31] = { [127] = { default.toggle } }, --         hotcue4
    [0x36] = { [127] = { unassigned     } }, --         touch
    [0x3C] = { [127] = { default.toggle } }, --         sampler1
    [0x3D] = { [127] = { unassigned     } }, -- [shift] sampler1
    [0x3E] = { [127] = { default.toggle } }, --         sampler2
    [0x3F] = { [127] = { unassigned     } }, -- [shift] sampler2
    [0x40] = { [127] = { default.toggle } }, --         sampler3
    [0x41] = { [127] = { unassigned     } }, -- [shift] sampler3
    [0x42] = { [127] = { default.toggle } }, --         sampler4
    [0x43] = { [127] = { unassigned     } }, -- [shift] sampler4
    [0x47] = { [127] = { unassigned     } }, -- [shift] play
    [0x48] = { [127] = { unassigned     } }, -- [shift] cue
    [0x50] = { [127] = { unassigned     } }, -- [shift] autoloop
    [0x58] = { [127] = { default.toggle } }, --         sync
    [0x59] = { [127] = { unassigned     } }, --         sampler
    [0x5C] = { [127] = { unassigned     } }, -- [shift] sync
    [0x5F] = { [127] = { unassigned     } }, -- [shift] hotcue1
    [0x60] = { [127] = { unassigned     } }, -- [shift] hotcue2
    [0x61] = { [127] = { unassigned     } }, -- [shift] hotcue3
    [0x62] = { [127] = { unassigned     } }, -- [shift] hotcue4
    [0x67] = { [127] = { unassigned     } }, -- [shift] touch
}
default.map[0xB1] = {
    [0x00] = { ['*'] = { MSB            } }, --         tempo MSB
    [0x05] = { ['*'] = { MSB            } }, -- [shift] tempo MSB
    [0x13] = { ['*'] = { unassigned     } }, --         auto loop turn
    [0x20] = { ['*'] = { unassigned     } }, --         tempo LSB
    [0x21] = { ['*'] = { unassigned     } }, --         jog wheel
    [0x22] = { ['*'] = { unassigned     } }, --         scratch
    [0x25] = { ['*'] = { unassigned     } }, -- [shift] tempo LSB
    [0x26] = { ['*'] = { unassigned     } }, -- [shift] jog wheel
    [0x27] = { ['*'] = { unassigned     } }, -- [shift] scratch
    [0x4F] = { ['*'] = { unassigned     } }, -- [shift] auto loop turn
}
--deckC
default.map[0x92] = {
    [0x0B] = { [127] = { default.toggle } }, --         play
    [0x0C] = { [127] = { default.toggle } }, --         cue
    [0x14] = { [127] = { unassigned     } }, --         autoloop
    [0x2E] = { [127] = { default.toggle } }, --         hotcue1
    [0x2F] = { [127] = { default.toggle } }, --         hotcue2
    [0x30] = { [127] = { default.toggle } }, --         hotcue3
    [0x31] = { [127] = { default.toggle } }, --         hotcue4
    [0x36] = { [127] = { unassigned     } }, --         touch
    [0x3C] = { [127] = { default.toggle } }, --         sampler1
    [0x3D] = { [127] = { unassigned     } }, -- [shift] sampler1
    [0x3E] = { [127] = { default.toggle } }, --         sampler2
    [0x3F] = { [127] = { unassigned     } }, -- [shift] sampler2
    [0x40] = { [127] = { default.toggle } }, --         sampler3
    [0x41] = { [127] = { unassigned     } }, -- [shift] sampler3
    [0x42] = { [127] = { default.toggle } }, --         sampler4
    [0x43] = { [127] = { unassigned     } }, -- [shift] sampler4
    [0x47] = { [127] = { unassigned     } }, -- [shift] play
    [0x48] = { [127] = { unassigned     } }, -- [shift] cue
    [0x50] = { [127] = { unassigned     } }, -- [shift] autoloop
    [0x58] = { [127] = { default.toggle } }, --         sync
    [0x59] = { [127] = { unassigned     } }, --         sampler
    [0x5C] = { [127] = { unassigned     } }, -- [shift] sync
    [0x5F] = { [127] = { unassigned     } }, -- [shift] hotcue1
    [0x60] = { [127] = { unassigned     } }, -- [shift] hotcue2
    [0x61] = { [127] = { unassigned     } }, -- [shift] hotcue3
    [0x62] = { [127] = { unassigned     } }, -- [shift] hotcue4
    [0x67] = { [127] = { unassigned     } }, -- [shift] touch
}
default.map[0xB2] = {
    [0x00] = { ['*'] = { MSB            } }, --         tempo MSB
    [0x05] = { ['*'] = { MSB            } }, -- [shift] tempo MSB
    [0x13] = { ['*'] = { unassigned     } }, --         auto loop turn
    [0x20] = { ['*'] = { unassigned     } }, --         tempo LSB
    [0x21] = { ['*'] = { unassigned     } }, --         jog wheel
    [0x22] = { ['*'] = { unassigned     } }, --         scratch
    [0x25] = { ['*'] = { unassigned     } }, -- [shift] tempo LSB
    [0x26] = { ['*'] = { unassigned     } }, -- [shift] jog wheel
    [0x27] = { ['*'] = { unassigned     } }, -- [shift] scratch
    [0x4F] = { ['*'] = { unassigned     } }, -- [shift] auto loop turn
}
--deckD
default.map[0x93] = {
    [0x0B] = { [127] = { default.toggle } }, --         play
    [0x0C] = { [127] = { default.toggle } }, --         cue
    [0x14] = { [127] = { unassigned     } }, --         autoloop
    [0x2E] = { [127] = { default.toggle } }, --         hotcue1
    [0x2F] = { [127] = { default.toggle } }, --         hotcue2
    [0x30] = { [127] = { default.toggle } }, --         hotcue3
    [0x31] = { [127] = { default.toggle } }, --         hotcue4
    [0x36] = { [127] = { unassigned     } }, --         touch
    [0x3C] = { [127] = { default.toggle } }, --         sampler1
    [0x3D] = { [127] = { unassigned     } }, -- [shift] sampler1
    [0x3E] = { [127] = { default.toggle } }, --         sampler2
    [0x3F] = { [127] = { unassigned     } }, -- [shift] sampler2
    [0x40] = { [127] = { default.toggle } }, --         sampler3
    [0x41] = { [127] = { unassigned     } }, -- [shift] sampler3
    [0x42] = { [127] = { default.toggle } }, --         sampler4
    [0x43] = { [127] = { unassigned     } }, -- [shift] sampler4
    [0x47] = { [127] = { unassigned     } }, -- [shift] play
    [0x48] = { [127] = { unassigned     } }, -- [shift] cue
    [0x50] = { [127] = { unassigned     } }, -- [shift] autoloop
    [0x58] = { [127] = { default.toggle } }, --         sync
    [0x59] = { [127] = { unassigned     } }, --         sampler
    [0x5C] = { [127] = { unassigned     } }, -- [shift] sync
    [0x5F] = { [127] = { unassigned     } }, -- [shift] hotcue1
    [0x60] = { [127] = { unassigned     } }, -- [shift] hotcue2
    [0x61] = { [127] = { unassigned     } }, -- [shift] hotcue3
    [0x62] = { [127] = { unassigned     } }, -- [shift] hotcue4
    [0x67] = { [127] = { unassigned     } }, -- [shift] touch
}
default.map[0xB3] = {
    [0x00] = { ['*'] = { MSB            } }, --         tempo MSB
    [0x05] = { ['*'] = { MSB            } }, -- [shift] tempo MSB
    [0x13] = { ['*'] = { unassigned     } }, --         auto loop turn
    [0x20] = { ['*'] = { unassigned     } }, --         tempo LSB
    [0x21] = { ['*'] = { unassigned     } }, --         jog wheel
    [0x22] = { ['*'] = { unassigned     } }, --         scratch
    [0x25] = { ['*'] = { unassigned     } }, -- [shift] tempo LSB
    [0x26] = { ['*'] = { unassigned     } }, -- [shift] jog wheel
    [0x27] = { ['*'] = { unassigned     } }, -- [shift] scratch
    [0x4F] = { ['*'] = { unassigned     } }, -- [shift] auto loop turn
}
--ctrl & fx for deckAC {
default.map[0x94] = {
    [0x42] = { [127] = { default.toggle } }, --         deckA ctrlA
    [0x43] = { [127] = { default.toggle } }, --         deckA fx1
    [0x44] = { [127] = { default.toggle } }, --         deckA fx2
    [0x45] = { [127] = { default.toggle } }, --         deckA fx3
    [0x46] = { [127] = { default.toggle } }, --         deckA ctrlB
    [0x47] = { [127] = { default.toggle } }, --         deckC ctrlA
    [0x48] = { [127] = { default.toggle } }, --         deckC fx1
    [0x49] = { [127] = { default.toggle } }, --         deckC fx2
    [0x4A] = { [127] = { default.toggle } }, --         deckC fx3
    [0x4B] = { [127] = { default.toggle } }, --         deckC ctrlB
    [0x4D] = { [127] = { unassigned     } }, -- [shift] deckA fx1
    [0x4E] = { [127] = { unassigned     } }, -- [shift] deckA fx2
    [0x4F] = { [127] = { unassigned     } }, -- [shift] deckA fx3
    [0x52] = { [127] = { unassigned     } }, -- [shift] deckC fx1
    [0x53] = { [127] = { unassigned     } }, -- [shift] deckC fx2
    [0x54] = { [127] = { unassigned     } }, -- [shift] deckC fx3
}
default.map[0xB4] = {
--deckA
    [0x00] = { ['*'] = { MSB            } }, --         ctrlA wheel MSB
    [0x20] = { ['*'] = { unassigned     } }, --         ctrlA wheel LSB
    [0x02] = { ['*'] = { MSB            } }, --         fx1 wheel MSB
    [0x22] = { ['*'] = { unassigned     } }, --         fx1 wheel LSB
    [0x04] = { ['*'] = { MSB            } }, --         fx2 wheel MSB
    [0x24] = { ['*'] = { unassigned     } }, --         fx2 wheel LSB
    [0x06] = { ['*'] = { MSB            } }, --         fx3 wheel MSB
    [0x26] = { ['*'] = { unassigned     } }, --         fx3 wheel LSB
    [0x07] = { ['*'] = { MSB            } }, --         ctrlB wheel MSB
    [0x27] = { ['*'] = { unassigned     } }, --         ctrlB wheel LSB
    [0x10] = { ['*'] = { MSB            } }, -- [shift] ctrlA wheel MSB
    [0x30] = { ['*'] = { unassigned     } }, -- [shift] ctrlA wheel LSB
    [0x12] = { ['*'] = { MSB            } }, -- [shift] fx1 wheel MSB
    [0x32] = { ['*'] = { unassigned     } }, -- [shift] fx1 wheel LSB
    [0x14] = { ['*'] = { MSB            } }, -- [shift] fx2 wheel MSB
    [0x34] = { ['*'] = { unassigned     } }, -- [shift] fx2 wheel LSB
    [0x16] = { ['*'] = { MSB            } }, -- [shift] fx3 wheel MSB
    [0x36] = { ['*'] = { unassigned     } }, -- [shift] fx3 wheel LSB
    [0x17] = { ['*'] = { MSB            } }, -- [shift] ctrlB wheel MSB
    [0x37] = { ['*'] = { unassigned     } }, -- [shift] ctrlB wheel LSB
--deckC
    [0x08] = { ['*'] = { MSB            } }, --         ctrlA wheel MSB
    [0x28] = { ['*'] = { unassigned     } }, --         ctrlA wheel LSB
    [0x0A] = { ['*'] = { MSB            } }, --         fx1 wheel MSB
    [0x2A] = { ['*'] = { unassigned     } }, --         fx1 wheel LSB
    [0x0C] = { ['*'] = { MSB            } }, --         fx2 wheel MSB
    [0x2C] = { ['*'] = { unassigned     } }, --         fx2 wheel LSB
    [0x0E] = { ['*'] = { MSB            } }, --         fx3 wheel MSB
    [0x2E] = { ['*'] = { unassigned     } }, --         fx3 wheel LSB
    [0x0F] = { ['*'] = { MSB            } }, --         ctrlB wheel MSB
    [0x2F] = { ['*'] = { unassigned     } }, --         ctrlB wheel LSB
    [0x18] = { ['*'] = { MSB            } }, -- [shift] ctrlA wheel MSB
    [0x38] = { ['*'] = { unassigned     } }, -- [shift] ctrlA wheel LSB
    [0x1A] = { ['*'] = { MSB            } }, -- [shift] fx1 wheel MSB
    [0x3A] = { ['*'] = { unassigned     } }, -- [shift] fx1 wheel LSB
    [0x1C] = { ['*'] = { MSB            } }, -- [shift] fx2 wheel MSB
    [0x3C] = { ['*'] = { unassigned     } }, -- [shift] fx2 wheel LSB
    [0x1E] = { ['*'] = { MSB            } }, -- [shift] fx3 wheel MSB
    [0x3E] = { ['*'] = { unassigned     } }, -- [shift] fx3 wheel LSB
    [0x1F] = { ['*'] = { MSB            } }, -- [shift] ctrlB wheel MSB
    [0x3F] = { ['*'] = { unassigned     } }, -- [shift] ctrlB wheel LSB
}
--ctrl & fx for deckBD {
default.map[0x95] = {
    [0x42] = { [127] = { default.toggle } }, --         deckB ctrlA
    [0x43] = { [127] = { default.toggle } }, --         deckB fx1
    [0x44] = { [127] = { default.toggle } }, --         deckB fx2
    [0x45] = { [127] = { default.toggle } }, --         deckB fx3
    [0x46] = { [127] = { default.toggle } }, --         deckB ctrlB
    [0x47] = { [127] = { default.toggle } }, --         deckD ctrlA
    [0x48] = { [127] = { default.toggle } }, --         deckD fx1
    [0x49] = { [127] = { default.toggle } }, --         deckD fx2
    [0x4A] = { [127] = { default.toggle } }, --         deckD fx3
    [0x4B] = { [127] = { default.toggle } }, --         deckD ctrlB
    [0x4D] = { [127] = { unassigned     } }, -- [shift] deckB fx1
    [0x4E] = { [127] = { unassigned     } }, -- [shift] deckB fx2
    [0x4F] = { [127] = { unassigned     } }, -- [shift] deckB fx3
    [0x52] = { [127] = { unassigned     } }, -- [shift] deckD fx1
    [0x53] = { [127] = { unassigned     } }, -- [shift] deckD fx2
    [0x54] = { [127] = { unassigned     } }, -- [shift] deckD fx3
}
default.map[0xB5] = {
--deckB
    [0x00] = { ['*'] = { MSB            } }, --         ctrlA wheel MSB
    [0x20] = { ['*'] = { unassigned     } }, --         ctrlA wheel LSB
    [0x02] = { ['*'] = { MSB            } }, --         fx1 wheel MSB
    [0x22] = { ['*'] = { unassigned     } }, --         fx1 wheel LSB
    [0x04] = { ['*'] = { MSB            } }, --         fx2 wheel MSB
    [0x24] = { ['*'] = { unassigned     } }, --         fx2 wheel LSB
    [0x06] = { ['*'] = { MSB            } }, --         fx3 wheel MSB
    [0x26] = { ['*'] = { unassigned     } }, --         fx3 wheel LSB
    [0x07] = { ['*'] = { MSB            } }, --         ctrlB wheel MSB
    [0x27] = { ['*'] = { unassigned     } }, --         ctrlB wheel LSB
    [0x10] = { ['*'] = { MSB            } }, -- [shift] ctrlA wheel MSB
    [0x30] = { ['*'] = { unassigned     } }, -- [shift] ctrlA wheel LSB
    [0x12] = { ['*'] = { MSB            } }, -- [shift] fx1 wheel MSB
    [0x32] = { ['*'] = { unassigned     } }, -- [shift] fx1 wheel LSB
    [0x14] = { ['*'] = { MSB            } }, -- [shift] fx2 wheel MSB
    [0x34] = { ['*'] = { unassigned     } }, -- [shift] fx2 wheel LSB
    [0x16] = { ['*'] = { MSB            } }, -- [shift] fx3 wheel MSB
    [0x36] = { ['*'] = { unassigned     } }, -- [shift] fx3 wheel LSB
    [0x17] = { ['*'] = { MSB            } }, -- [shift] ctrlB wheel MSB
    [0x37] = { ['*'] = { unassigned     } }, -- [shift] ctrlB wheel LSB
--deckD
    [0x08] = { ['*'] = { MSB            } }, --         ctrlA wheel MSB
    [0x28] = { ['*'] = { unassigned     } }, --         ctrlA wheel LSB
    [0x0A] = { ['*'] = { MSB            } }, --         fx1 wheel MSB
    [0x2A] = { ['*'] = { unassigned     } }, --         fx1 wheel LSB
    [0x0C] = { ['*'] = { MSB            } }, --         fx2 wheel MSB
    [0x2C] = { ['*'] = { unassigned     } }, --         fx2 wheel LSB
    [0x0E] = { ['*'] = { MSB            } }, --         fx3 wheel MSB
    [0x2E] = { ['*'] = { unassigned     } }, --         fx3 wheel LSB
    [0x0F] = { ['*'] = { MSB            } }, --         ctrlB wheel MSB
    [0x2F] = { ['*'] = { unassigned     } }, --         ctrlB wheel LSB
    [0x18] = { ['*'] = { MSB            } }, -- [shift] ctrlA wheel MSB
    [0x38] = { ['*'] = { unassigned     } }, -- [shift] ctrlA wheel LSB
    [0x1A] = { ['*'] = { MSB            } }, -- [shift] fx1 wheel MSB
    [0x3A] = { ['*'] = { unassigned     } }, -- [shift] fx1 wheel LSB
    [0x1C] = { ['*'] = { MSB            } }, -- [shift] fx2 wheel MSB
    [0x3C] = { ['*'] = { unassigned     } }, -- [shift] fx2 wheel LSB
    [0x1E] = { ['*'] = { MSB            } }, -- [shift] fx3 wheel MSB
    [0x3E] = { ['*'] = { unassigned     } }, -- [shift] fx3 wheel LSB
    [0x1F] = { ['*'] = { MSB            } }, -- [shift] ctrlB wheel MSB
    [0x3F] = { ['*'] = { unassigned     } }, -- [shift] ctrlB wheel LSB
}
-- Master
default.map[0x96] = {
    [0x41] = { [127] = { unassigned     } }, --         browse
    [0x42] = { [127] = { unassigned     } }, -- [shift] browse
    [0x46] = { [127] = { unassigned     } }, --         load deckA
    [0x47] = { [127] = { unassigned     } }, --         load deckB
    [0x48] = { [127] = { unassigned     } }, --         load deckC
    [0x49] = { [127] = { unassigned     } }, --         load deckD
    [0x54] = { [127] = { default.toggle } }, --         headphone deckA
    [0x55] = { [127] = { default.toggle } }, --         headphone deckB
    [0x56] = { [127] = { default.toggle } }, --         headphone deckC
    [0x57] = { [127] = { default.toggle } }, --         headphone deckD
    [0x58] = { [127] = { unassigned     } }, -- [shift] load deckA
    [0x59] = { [127] = { unassigned     } }, -- [shift] load deckB
    [0x5A] = { [127] = { unassigned     } }, -- [shift] load deckC
    [0x5B] = { [127] = { unassigned     } }, -- [shift] load deckD
    [0x5C] = { [127] = { unassigned     } }, -- [shift] headphone deckA
    [0x5D] = { [127] = { unassigned     } }, -- [shift] headphone deckB
    [0x5E] = { [127] = { unassigned     } }, -- [shift] headphone deckC
    [0x5F] = { [127] = { unassigned     } }, -- [shift] headphone deckD
}
default.map[0xB6] = {
    [0x00] = { ['*'] = { MSB            } }, -- [shift] crossfader MSB
    [0x20] = { ['*'] = { unassigned     } }, -- [shift] crossfader LSB
    [0x01] = { ['*'] = { MSB            } }, --         mix MSB
    [0x21] = { ['*'] = { unassigned     } }, --         mix LSB
    [0x1F] = { ['*'] = { MSB            } }, --         crossfader MSB
    [0x3F] = { ['*'] = { unassigned     } }, --         crossfader LSB
--deckA
    [0x07] = { ['*'] = { MSB            } }, --         high MSB
    [0x27] = { ['*'] = { unassigned     } }, --         high LSB
    [0x0B] = { ['*'] = { MSB            } }, --         mid MSB
    [0x2B] = { ['*'] = { unassigned     } }, --         mid LSB
    [0x0F] = { ['*'] = { MSB            } }, --         low MSB
    [0x2F] = { ['*'] = { unassigned     } }, --         low LSB
    [0x13] = { ['*'] = { MSB            } }, --         volume MSB
    [0x33] = { ['*'] = { unassigned     } }, --         volume LSB
    [0x14] = { ['*'] = { MSB            } }, -- [shift] volume MSB
    [0x34] = { ['*'] = { unassigned     } }, -- [shift] volume LSB
--deckB
    [0x08] = { ['*'] = { MSB            } }, --         high MSB
    [0x28] = { ['*'] = { unassigned     } }, --         high LSB
    [0x0C] = { ['*'] = { MSB            } }, --         mid MSB
    [0x2C] = { ['*'] = { unassigned     } }, --         mid LSB
    [0x10] = { ['*'] = { MSB            } }, --         low MSB
    [0x30] = { ['*'] = { unassigned     } }, --         low LSB
    [0x15] = { ['*'] = { MSB            } }, --         volume MSB
    [0x35] = { ['*'] = { unassigned     } }, --         volume LSB
    [0x16] = { ['*'] = { MSB            } }, -- [shift] volume MSB
    [0x36] = { ['*'] = { unassigned     } }, -- [shift] volume LSB
--deckC
    [0x09] = { ['*'] = { MSB            } }, --         high MSB
    [0x29] = { ['*'] = { unassigned     } }, --         high LSB
    [0x0D] = { ['*'] = { MSB            } }, --         mid MSB
    [0x2D] = { ['*'] = { unassigned     } }, --         mid LSB
    [0x11] = { ['*'] = { MSB            } }, --         low MSB
    [0x31] = { ['*'] = { unassigned     } }, --         low LSB
    [0x17] = { ['*'] = { MSB            } }, --         volume MSB
    [0x37] = { ['*'] = { unassigned     } }, --         volume LSB
    [0x18] = { ['*'] = { MSB            } }, -- [shift] volume MSB
    [0x38] = { ['*'] = { unassigned     } }, -- [shift] volume LSB
--deckD
    [0x0A] = { ['*'] = { MSB            } }, --         high MSB
    [0x2A] = { ['*'] = { unassigned     } }, --         high LSB
    [0x0E] = { ['*'] = { MSB            } }, --         mid MSB
    [0x2E] = { ['*'] = { unassigned     } }, --         mid LSB
    [0x12] = { ['*'] = { MSB            } }, --         low MSB
    [0x32] = { ['*'] = { unassigned     } }, --         low LSB
    [0x19] = { ['*'] = { MSB            } }, --         volume MSB
    [0x39] = { ['*'] = { unassigned     } }, --         volume LSB
    [0x1A] = { ['*'] = { MSB            } }, -- [shift] volume MSB
    [0x3A] = { ['*'] = { unassigned     } }, -- [shift] volume LSB
}

--[[ application specific maps ]]--
function func( user_data )
    print( "I am a functioni: ", user_data )
end

lightworks = { map = {} }
lightworks.map[0x91] = {}
lightworks.map[0x91][0x00] = {}
lightworks.map[0x91][0x00][0x00] = {func, "user data" }

--[[ Missing assigments from previous version ]]--
-- * dbus message sending to mediaplayer advertisement, honestly it was shit
--   house anyway because i would experiment with my mediaplayer too much
-- * deckD and deckC lights, i think this will be as easy as assigning toggle
--   to them
-- * lightworks mapping - jogdial, play/pause, i dunno... lol
-- * disabling controls when mixx is playing, either way or..
-- * vlc controls
-- * ffplay controls
-- * mplayer controls
-- * generic scrolling for right wheel
-- * generic alsa volume controls
--[[ Brainstorming things for future]]--
-- * each application having a default state so that when switching between i
--   can light up the buttons which have commands associated with them. or check
--   using dbus to query the state and make the deck show whats up. so that
--   would mean polling functions, or dbus subscriptions or some such.



function initialise()
    print( "nothing to do" )
end

function trigger( app, event )
    if not app then return -1 end
    if not event then return -1 end

    local channel = event[1]
    local control = event[2]
    local value   = event[3]

    if app.map[channel] and app.map[channel][control]
    then
        local control = app.map[channel][control]
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
            return
        else
            action[1]( event )
            return
        end
    end

end
--[[ receive and react ]]--
function midi_recv( channel, control, value )
    local event = {channel, control, value}
    local app = _G[wm_class]
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


