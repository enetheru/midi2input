--[[ Imported c++ Functions ]]--
--initialise()
--keypress( XK_keycode )
--keydown( XK_keycode )
--keyup( XK_keycode )
--buttonpress( n )
--buttondown( n )
--buttonup( n )
--mousemove( x, y )
--mousepos( x, y )
--midi_send( { status, data1, data2 } )
--exec( 'command' )
--
--[[ Imported Global Variables ]]--
--wm_class
--autoconnect
--
--[[ Functions you must create ]]
--midi_recv( status, data1, data2 )
--loop()

------------------------

--[[ Simple Example ]]--

--[[ Defines ]]--
-- look to X11\keysymdef.h for the full list
XK_space                       = 0x0020  --/* U+0020 SPACE */

controller = {
    deck = {
        A = {
            play = {0x90, 0x0B, 127}
        }
    }
}

--[[ global settings ]]--
-- autoconnect: can be true, false, or a named jack port. default = true
autoconnect = false

--[[ My custom Functions ]]--
function message_compare( a, b )
    if a[1] ~= -1 then
        if a[1] ~= b[1] then return true end
    end
    if a[2] ~= -1 then
        if a[2] ~= b[2] then return true end
    end
    if a[3] ~= -1 then
        if a[3] ~= b[3] then return true end
    end
    return false
end

--[[ initialisation function ]]--
-- run immeditely after the application launches and connects to the device
function initialise()
    print( "[LUA] nothing to initialise" )
end


--[[ Input Event Handler ]]--
function midi_recv( status, data1, data2 )
    local message = { status, data1, data2 }

    if( not message_compare( message, controller.deck['A'].play ) ) then
        keypress( XK_space )
    end
end
