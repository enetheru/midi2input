--[[ Defines ]]--
-- these are all taken from X11\keysymdef.h
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

--[[ global settings ]]--
-- autoconnect: can be true, false, or a named jack port. default = true
autoconnect = true

-- a funtion to recursively walk the ddj structure and send signals
-- to the controller to refresh the lighting state.
function refresh_state( object )
    for index,value in pairs( object ) do
        if( type( object[ index ] ) == "table" ) then
            refresh_state( object[ index ] )
        end
        if( index == "lstate" ) then
            if( value == true ) then
                midi_send( object.lon )
            else
                midi_send( object.loff )
            end

            -- if we send all the events at once the controller does not
            -- respond to them all. 0.001 failed.
            sleep(0.01)
        end
    end
end

local clock = os.clock
function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end
-- warning: clock can eventually wrap around for sufficiently large n
-- (whose value is platform dependent).  Even for n == 1, clock() - t0
-- might become negative on the second that clock wraps.

--[[ ddj-wego button to midi tables]]--
ddj = {
    misc = {
        browse = {
            down   = { 0x96, 0x41, 127 },
            up     = { 0x86, 0x41,  64 },
            turn   = { 0xB6, 0x40,  -1 }, -- values 0-30 or 98-127
            sdown  = { 0x96, 0x42, 127 },
            sup    = { 0x86, 0x42,  64 },
            sturn  = { 0xB6, 0x64,  -1 }, -- values 0-30 or 98-127
        },
        hpmix = {
            msb = { 0xB6, 0x01, -1 },
            lsb = { 0xB6, 0x21, -1 },
        },
        crossfader = {
            msb  = { 0xB6, 0x1F, -1 },
            lsb  = { 0xB6, 0x3F, -1 },
            smsb = { 0xB6, 0x00, -1 },
            slsb = { 0xB6, 0x20, -1 },
        },
        deckC = {
            down  = { 0x92, 0x72, 127 },
            up    = { 0x82, 0x72,  64 },
            lon   = { 0x92, 0x72, 127 },
            loff  = { 0x92, 0x72,   0 },
            lstate = false,
        },
        deckD = {
            down  = { 0x93, 0x72, 127 },
            up    = { 0x83, 0x72,  64 },
            lon   = { 0x93, 0x72, 127 },
            loff  = { 0x93, 0x72,   0 },
            lstate = false,
        },
    },
}

--[[ add the four decks substituting the channel ]]--
decks = { deckA = 1; deckB = 2; deckC = 3, deckD = 4 }
for deck, ch in pairs(decks) do
    local note_on =  0x90 + ch -1
    local note_off =  0x80 + ch -1
    local control = 0xB0 + ch -1

    ddj[deck] = {}
    ddj[deck] = {
        tempo = {
            msb  = { control, 0x00, -1 },
            lsb  = { control, 0x20, -1 },
            smsb = { control, 0x05, -1 },
            slsb = { control, 0x25, -1 },
        },
        sync = {
            down   = { note_on,  0x58, 127 },
            up     = { note_off, 0x58,  64 },
            sdown  = { note_on,  0x5C, 127 },
            sup    = { note_off, 0x5C,  64 },
            lon    = { note_on,  0x58, 127 },
            loff   = { note_on,  0x58,   0 },
            lstate = false,
        },

        autoloop = {
            turn  = { control,  0x13,  -1 }, -- values 0-30 or 98-127
            sturn = { control,  0x4F,  -1 }, -- values 0-30 or 98-127

            next  = { control,  0x13,   1 }, 
            snext = { control,  0x4F,   1 },

            prev  = { control,  0x13, 127 },
            sprev = { control,  0x4F, 127 },

            down  = { note_on,  0x14, 127 },
            sdown = { note_on,  0x50, 127 },

            up    = { note_off, 0x14,  64 },
            sup   = { note_off, 0x50,  64 },
        },
        load = {
            down  = { 0x96, 0x45+ch, 127 },
            up    = { 0x86, 0x45+ch,  64 },
            sdown = { 0x96, 0x57+ch, 127 },
            sup   = { 0x86, 0x57+ch,  64 },
        },

        --equaliser
        eqhi = {
            msb = { 0xB6, 0x07, -1 },
            lsb = { 0xB6, 0x27, -1 },
        },
        eqmid = {
            msb = { 0xB6, 0x0B, -1 },
            lsb = { 0xB6, 0x2B, -1 },
        },
        eqlow = {
            msb = { 0xB6, 0x0F, -1 },
            lsb = { 0xB6, 0x2F, -1 },
        },

        headphone = {
            down  = { 0x96, 0x53+ch, 127 },
            up    = { 0x86, 0x53+ch,  64 },
            sdown = { 0x96, 0x5B+ch, 127 },
            sup   = { 0x86, 0x5B+ch,  64 },
            lon   = { 0x96, 0x53+ch, 127 },
            loff  = { 0x96, 0x53+ch,   0 },
            lstate = false,
        },
        fader = {
            msb  = { 0xB6, 0x13, -1 },
            lsb  = { 0xB6, 0x33, -1 },
            smsb = { 0xB6, 0x14, -1 },
            slsb = { 0xB6, 0x34, -1 },
        },



        jogdial = {
            down  = { note_on,  0x36, 127 },
            up    = { note_off, 0x36,  64 },
            sdown = { note_on,  0x67, 127 },
            sup   = { note_on,  0x67,  64 },
            turn  = { control,  0x22,  -1 }, --clockwise=65-127,cc=1-63
            sturn = { control,  0x27,  -1 }, --clockwise=65-127,cc=1-63
            rim   = { control,  0x22,  -1 }, --clockwise=65-127,cc=1-63
            srim  = { control,  0x26,  -1 }, --clockwise=65-127,cc=1-63
        },

        cue = {
            down   = { note_on,  0x0C, 127 },
            up     = { note_off, 0x0C,  64 },
            sdown  = { note_on,  0x48, 127 },
            sup    = { note_off, 0x48,  64 },
            lon    = { note_on,  0x0C, 127 },
            loff   = { note_on,  0x0C,   0 },
            lstate = false,
        },
        play = {
            down   = { note_on,  0x0B, 127 },
            up     = { note_off, 0x0B,  64 },
            sdown  = { note_on,  0x47, 127 },
            sup    = { note_off, 0x47,  64 },
            lon    = { note_on,  0x0B, 127 },
            loff   = { note_on,  0x0B,   0 },
            lstate = false,
        },

        --Samplers
        sampler = {
            down   = { note_on, 0x59, 127 },
            up     = { note_off, 0x59,  64 },
        },
        sampler1 = {
            down   = { note_on,  0x2E, 127 },
            up     = { note_off, 0x2E,  64 },
            sdown  = { note_on,  0x5F, 127 },
            sup    = { note_off, 0x5F,  64 },
            lon    = { note_on,  0x2E, 127 },
            loff   = { note_on,  0x2E,   0 },
            lstate = false,
        },
        sampler2 = {
            down   = { note_on,  0x2F, 127 },
            up     = { note_off, 0x2F,  64 },
            sdown  = { note_on,  0x60, 127 },
            sup    = { note_off, 0x60,  64 },
            lon    = { note_on,  0x2F, 127 },
            loff   = { note_on,  0x2F,   0 },
            lstate = false,
        },
        sampler3 = {
            down   = { note_on,  0x30, 127 },
            up     = { note_off, 0x30,  64 },
            sdown  = { note_on,  0x61, 127 },
            sup    = { note_off, 0x61,  64 },
            lon    = { note_on,  0x30, 127 },
            loff   = { note_on,  0x30,   0 },
            lstate = false,
        },
        sampler4 = {
            down   = { note_on,  0x31, 127 },
            up     = { note_off, 0x31,  64 },
            sdown  = { note_on,  0x62, 127 },
            sup    = { note_off, 0x62,  64 },
            lon    = { note_on,  0x31, 127 },
            loff   = { note_on,  0x31,   0 },
            lstate = false,
        },
        sampler5 = {
            down   = { note_on,  0x3C, 127 },
            up     = { note_off, 0x3C,  64 },
            sdown  = { note_on,  0x3D, 127 },
            sup    = { note_off, 0x3D,  64 },
            lon    = { note_on,  0x3C, 127 },
            loff   = { note_on,  0x3C,   0 },
            lstate = false,
        },
        sampler6 = {
            down   = { note_on,  0x3E, 127 },
            up     = { note_off, 0x3E,  64 },
            sdown  = { note_on,  0x3F, 127 },
            sup    = { note_off, 0x3F,  64 },
            lon    = { note_on,  0x3E, 127 },
            loff   = { note_on,  0x3E,   0 },
            lstate = false,
        },
        sampler7 = {
            down   = { note_on,  0x40, 127 },
            up     = { note_off, 0x40,  64 },
            sdown  = { note_on,  0x41, 127 },
            sup    = { note_off, 0x41,  64 },
            lon    = { note_on,  0x40, 127 },
            loff   = { note_on,  0x40,   0 },
            lstate = false,
        },
        sampler8 = {
            down   = { note_on,  0x42, 127 },
            up     = { note_off, 0x42,  64 },
            sdown  = { note_on,  0x43, 127 },
            sup    = { note_off, 0x43,  64 },
            lon    = { note_on,  0x42, 127 },
            loff   = { note_on,  0x42,   0 },
            lstate = false,
        },



    }
end
-- Deck A
ddj.deckA.ctrlA = {
            down = { 0x94, 0x42, 127 },
            up   = { 0x84, 0x42, 0   },
            loff = { 0x94, 0x42, 0   },
            lon  = { 0x94, 0x42, 127 },
            lstate = false,
}
ddj.deckA.FX1 = {
            down  = { 0x94, 0x43, 127 },
            up    = { 0x84, 0x43, 64  },
            sdown = { 0x94, 0x4D, 127 },
            sup   = { 0x94, 0x4D, 64  },
            loff  = { 0x94, 0x43, 0   },
            lon   = { 0x94, 0x43, 127 },
            lstate = false,
}
ddj.deckA.FX2 = {
            down  = { 0x94, 0x44, 127 },
            up    = { 0x84, 0x44, 64  },
            sdown = { 0x94, 0x4E, 127 },
            sup   = { 0x94, 0x4E, 64  },
            loff  = { 0x94, 0x44, 0   },
            lon   = { 0x94, 0x44, 127 },
            lstate = false,
}
ddj.deckA.FX3 = {
            down  = { 0x94, 0x45, 127 },
            up    = { 0x84, 0x45, 64  },
            sdown = { 0x94, 0x4F, 127 },
            sup   = { 0x94, 0x4F, 64  },
            loff  = { 0x94, 0x45, 0   },
            lon   = { 0x94, 0x45, 127 },
            lstate = false,
}
ddj.deckA.ctrlB = {
            down  = { 0x94, 0x46, 127 },
            up    = { 0x84, 0x46, 64   },
            loff  = { 0x94, 0x46, 0   },
            lon   = { 0x94, 0x46, 127 },
            lstate = false,
}
-- Deck B
ddj.deckB.ctrlA = {
            down = { 0x95, 0x42, 127 },
            up   = { 0x85, 0x42, 0   },
            loff = { 0x95, 0x42, 0   },
            lon  = { 0x95, 0x42, 127 },
            lstate = false,
}
ddj.deckB.FX1 = {
            down  = { 0x95, 0x43, 127 },
            up    = { 0x85, 0x43, 64  },
            sdown = { 0x95, 0x4D, 127 },
            sup   = { 0x95, 0x4D, 64  },
            loff  = { 0x95, 0x43, 0   },
            lon   = { 0x95, 0x43, 127 },
            lstate = false,
}
ddj.deckB.FX2 = {
            down  = { 0x95, 0x44, 127 },
            up    = { 0x85, 0x44, 64  },
            sdown = { 0x95, 0x4E, 127 },
            sup   = { 0x95, 0x4E, 64  },
            loff  = { 0x95, 0x44, 0   },
            lon   = { 0x95, 0x44, 127 },
            lstate = false,
}
ddj.deckB.FX3 = {
            down  = { 0x95, 0x45, 127 },
            up    = { 0x85, 0x45, 64  },
            sdown = { 0x95, 0x4F, 127 },
            sup   = { 0x95, 0x4F, 64  },
            loff  = { 0x95, 0x45, 0   },
            lon   = { 0x95, 0x45, 127 },
            lstate = false,
}
ddj.deckB.ctrlB = {
            down  = { 0x95, 0x46, 127 },
            up    = { 0x85, 0x46, 64   },
            loff  = { 0x95, 0x46, 0   },
            lon   = { 0x95, 0x46, 127 },
            lstate = false,
}

-- Deck C
ddj.deckC.ctrlA = {
            down = { 0x94, 0x47, 127 },
            up   = { 0x84, 0x47, 0   },
            loff = { 0x94, 0x47, 0   },
            lon  = { 0x94, 0x47, 127 },
            lstate = false,
}
ddj.deckC.FX1 = {
            down  = { 0x94, 0x48, 127 },
            up    = { 0x84, 0x48, 64  },
            sdown = { 0x94, 0x52, 127 },
            sup   = { 0x94, 0x52, 64  },
            loff  = { 0x94, 0x48, 0   },
            lon   = { 0x94, 0x48, 127 },
            lstate = false,
}
ddj.deckC.FX2 = {
            down  = { 0x94, 0x49, 127 },
            up    = { 0x84, 0x49, 64  },
            sdown = { 0x94, 0x53, 127 },
            sup   = { 0x94, 0x53, 64  },
            loff  = { 0x94, 0x49, 0   },
            lon   = { 0x94, 0x49, 127 },
            lstate = false,
}
ddj.deckC.FX3 = {
            down  = { 0x94, 0x4A, 127 },
            up    = { 0x84, 0x4A, 64  },
            sdown = { 0x94, 0x54, 127 },
            sup   = { 0x94, 0x54, 64  },
            loff  = { 0x94, 0x4A, 0   },
            lon   = { 0x94, 0x4A, 127 },
            lstate = false,
}
ddj.deckC.ctrlB = {
            down  = { 0x94, 0x4B, 127 },
            up    = { 0x84, 0x4B, 64   },
            loff  = { 0x94, 0x4B, 0   },
            lon   = { 0x94, 0x4B, 127 },
            lstate = false,
}
--deck D
ddj.deckD.ctrlA = {
            down = { 0x95, 0x47, 127 },
            up   = { 0x85, 0x47, 0   },
            loff = { 0x95, 0x47, 0   },
            lon  = { 0x95, 0x47, 127 },
            lstate = false,
}
ddj.deckD.FX1 = {
            down  = { 0x95, 0x48, 127 },
            up    = { 0x85, 0x48, 64  },
            sdown = { 0x95, 0x52, 127 },
            sup   = { 0x95, 0x52, 64  },
            loff  = { 0x95, 0x48, 0   },
            lon   = { 0x95, 0x48, 127 },
            lstate = false,
}
ddj.deckD.FX2 = {
            down  = { 0x95, 0x49, 127 },
            up    = { 0x85, 0x49, 64  },
            sdown = { 0x95, 0x53, 127 },
            sup   = { 0x95, 0x53, 64  },
            loff  = { 0x95, 0x49, 0   },
            lon   = { 0x95, 0x49, 127 },
            lstate = false,
}
ddj.deckD.FX3 = {
            down  = { 0x95, 0x4A, 127 },
            up    = { 0x85, 0x4A, 64  },
            sdown = { 0x95, 0x54, 127 },
            sup   = { 0x95, 0x54, 64  },
            loff  = { 0x95, 0x4A, 0   },
            lon   = { 0x95, 0x4A, 127 },
            lstate = false,
}
ddj.deckD.ctrlB = {
            down  = { 0x95, 0x4B, 127 },
            up    = { 0x85, 0x4B, 64   },
            loff  = { 0x95, 0x4B, 0   },
            lon   = { 0x95, 0x4B, 127 },
            lstate = false,
}
-- generic test function to turn on and off lighted buttons
function lbutton_toggle( button )
    if button.lstate == false then
        button.lstate = true
        midi_send( button.lon )
    else
        button.lstate = false
        midi_send( button.loff )
    end
end


--[[ initialisation function ]]--
-- run immeditely after the application launches and connects to the device
function initialise()
    print( "[LUA] nothing to initialise" )
    --refresh_state( ddj )
    return;
end

--[[ TODO add LOG( INFO/DEBUG/ERROR/ETC ) decription continues...
--   logging functionality such that LOG(INFO) etc are available to lua. exposing the 'elog' logging level to lua
--   probably will solve this nicely.
 ]]

--[[ Action Tables ]]--
--[[ This section maps midi events to actions using the following format
    { { status, data1, data2 }, { function, optional_args } }
eg. { {   0x90,  0x72,   127 }, { keypress, XK_a          } }
eg. {              named_event, { myFunction              } }
eg. { {   0x90,  0x72,    -1 }, { myFunction              } }

if no function arguments are specified then the midi event will be passed to
the function.
the comparison function for midi events works such that a -1 will compare as
equal.

pre-defined functions are:
* keypress( XK_keycode )
* keydown( XK_keycode )
* keyup( XK_keycode )
* buttonpress( n )
* buttondown( n )
* buttonup( n )
* mousemove( x, y )
* mousepos( x, y )
* send_midi( { status, data1, data2 } )
* exec( "command" )
    ]]--

--[[ dbus commands ]]--
MediaPlayer2 = 'rhythmbox'
dbus_MediaPlayer2 = 'dbus-send --session --type="method_call"'
    .. ' --dest=org.mpris.MediaPlayer2.' .. MediaPlayer2
    .. ' /org/mpris/MediaPlayer2'
    .. ' org.mpris.MediaPlayer2.Player.' -- append command to end

function dbus_MediaPlayer2_seek( midi )
    exec( dbus_MediaPlayer2 .. 'Seek int64:' .. (midi[3] - 64) * 1000000)
end

--[[ Default action map ]]--
default = {
    name = "Default Configuration",
    map = {
        --dbus playPause,next,prev using MediaPlayer2.Player interface
        { ddj.deckA.play.down,     { exec, dbus_MediaPlayer2 .. 'PlayPause' } },
        { ddj.deckA.autoloop.next, { exec, dbus_MediaPlayer2 .. 'Next' } },
        { ddj.deckA.autoloop.prev, { exec, dbus_MediaPlayer2 .. 'Previous' } },
        { ddj.deckA.jogdial.turn,   { dbus_MediaPlayer2_seek } },

        { ddj.misc.deckC.down, { lbutton_toggle, ddj.misc.deckC    } },
        { ddj.misc.deckD.down, { lbutton_toggle, ddj.misc.deckD    } },
        { ddj.misc.browse.sup, { midi_send,      {0xFF, 0, 0}      } },
        { ddj.deckA.sync.down, { lbutton_toggle, ddj.deckA.sync    } },

        { ddj.deckA.ctrlA.down, { lbutton_toggle, ddj.deckA.ctrlA } },
        { ddj.deckA.FX1.down,   { lbutton_toggle, ddj.deckA.FX1 } },
        { ddj.deckA.FX2.down,   { lbutton_toggle, ddj.deckA.FX2 } },
        { ddj.deckA.FX3.down,   { lbutton_toggle, ddj.deckA.FX3 } },
        { ddj.deckA.ctrlB.down, { lbutton_toggle, ddj.deckA.ctrlB } },

        { ddj.deckB.ctrlA.down, { lbutton_toggle, ddj.deckB.ctrlA } },
        { ddj.deckB.FX1.down,   { lbutton_toggle, ddj.deckB.FX1 } },
        { ddj.deckB.FX2.down,   { lbutton_toggle, ddj.deckB.FX2 } },
        { ddj.deckB.FX3.down,   { lbutton_toggle, ddj.deckB.FX3 } },
        { ddj.deckB.ctrlB.down, { lbutton_toggle, ddj.deckB.ctrlB } },

        { ddj.deckC.ctrlA.down, { lbutton_toggle, ddj.deckC.ctrlA } },
        { ddj.deckC.FX1.down,   { lbutton_toggle, ddj.deckC.FX1 } },
        { ddj.deckC.FX2.down,   { lbutton_toggle, ddj.deckC.FX2 } },
        { ddj.deckC.FX3.down,   { lbutton_toggle, ddj.deckC.FX3 } },
        { ddj.deckC.ctrlB.down, { lbutton_toggle, ddj.deckC.ctrlB } },

        { ddj.deckD.ctrlA.down, { lbutton_toggle, ddj.deckD.ctrlA } },
        { ddj.deckD.FX1.down,   { lbutton_toggle, ddj.deckD.FX1 } },
        { ddj.deckD.FX2.down,   { lbutton_toggle, ddj.deckD.FX2 } },
        { ddj.deckD.FX3.down,   { lbutton_toggle, ddj.deckD.FX3 } },
        { ddj.deckD.ctrlB.down, { lbutton_toggle, ddj.deckD.ctrlB } },
    }
}

-- Lightworks
-------------

-- Function to allow using the right jogdial pad in lightworks.
-- needs for lightworks to change keyboard shortcuts for XK_Right & Left
-- to jog right and left.
function lw_jogdial_turn( event )
    if event[3] > 64 then
        keypress( XK_Right )
    end
    if event[3] < 64 then
        keypress( XK_Left )
    end
end

function lw_movegrid_turn( event )
    if event[3] > 64 then
        keypress( XK_a )
    end
    if event[3] < 64 then
        keypress( XK_s )
    end
end

-- Action Map
lightworks = {
    name = "Lightworks",
    map = {
        { ddj.deckB.play.down,      { keypress, XK_space } },
        { ddj.deckB.jogdial.turn,   { lw_jogdial_turn } },
        { ddj.deckB.cue.down,       { keypress, XK_a } },
        { ddj.deckB.sampler1.down,  { keypress, XK_apostrophe } },
        { ddj.deckB.sampler2.down,  { keypress, XK_p } },
        { ddj.deckB.sampler3.down,  { keypress, XK_i } },
        { ddj.deckB.sampler4.down,  { keypress, XK_o } },
        --{ ddj.deckB.ctrla.down,     { keypress, XK_x } },
        { ddj.deckB.autoloop.turn,  { lw_movegrid_turn } },
    }
}


-- Banshee
----------
function banshee_autoloop_turn( event )
    if event[3] < 30 then keypress( XK_n ) end
    if event[3] > 100 then keypress( XK_b ) end
end

Banshee = {
    name = "Banshee Media Player",
    map = {
        { ddj.deckA.play.down, { keypress, XK_space } },
        { ddj.deckA.autoloop.turn, { banshee_autoloop_turn } },
    }
}


-- Mixxx
--------
mixxx = {
    name = "Mixxx DJ",
    map = nil,
}

-- VLC
------
vlc = {
    name = "VideoLAN Client",
    map = {
        { ddj.deckB.play.down, { keypress, XK_space } },
    }
}

-- Applications Table
---------------------
applications = {
    ["Banshee"] = Banshee,
    ["vlc"] = vlc,
    ["mixxx"] = mixxx,
    ["ntcardvt"] = lightworks,
}

function table_rows( t )
    local i = 0
    local n = #t
    return function ()
        i = i + 1
        if i <= n then return t[i] end
    end
end

--[[ Pattern matcher for messages ]]--
--
-- Both pattern and message share the same structure: {status, data1, data2}.
-- For any element of the pattern is equal -1, corresponding element of the
-- message is ignored / considered equal.
function message_matches( pattern, message )
    if pattern[1] ~= -1 then
        if pattern[1] ~= message[1] then return false end
    end
    if pattern[2] ~= -1 then
        if pattern[2] ~= message[2] then return false end
    end
    if pattern[3] ~= -1 then
        if pattern[3] ~= message[3] then return false end
    end
    return true
end

--[[ Input Event Handler ]]--
function midi_recv( status, data1, data2 )
    local message = { status, data1, data2 }

    -- select application based on wm_name pulled from X11
    local app = applications[ wm_class ]
    if( not app ) then
        app = default
    end
    local table = app.map

    -- look for control in the application map first, before the default map
    while( table ) do
        -- search items
        for row in table_rows( table ) do
            -- test for event
            if( message_matches( row[ 1 ], message ) ) then
                -- check for argument in second pair
                if( row[ 2 ][ 2 ] ) then
                    row[ 2 ][ 1 ]( row[ 2 ][ 2 ] )
                else
                    row[ 2 ][ 1 ]( message )
                end
                return
            end
        end
        --if not found in application.map search default
        if( table == default.map ) then table = nil
        else table = default.map
        end
    end

end
