--[[ global settings ]]--
-- autoconnect: can be true, false, or a named jack port. default = true
autoconnect = true
--[[ Mapping ]]--
--[[ this section maps midi events to input events using the following
    construct
    { {type, channel, note, velocity}, {modifyer mask, input event} }
eg. { {0x9, 0-15, 0-127, 0-127}, {0x00, 'b'} }
    ]]--

map = {
    { {0x9, 1, 11, -1}, {0x00, 'z'} },
}

--# separate definition of button physical logic, from physical looks. with overrides

--[[#ie
control_definition_button = {
    type = "button",
    name = "button",
    midi_on = {0x00, 0x00},
    midi_off = {0x00, 0x00},
    midi_light_on = {0x00, 0x00},
    midi_light_off = {0x00, 0x00},
    light_default = off;
}

button_appearance = {
    --measurements in mm, and radians
    location_default = {x,y},
    location_min = {x,y},
    location_max = {x,y},
    shape = "circle", --# or square or custom
    shape_custom = {{x,y},{x,y}},
    radius = 0.0,
    rotation_default = 0.0,
    rotation_min = 0.0,
    rotation_max = 0.0,
}

control_01 = {
    name = "desk_1_play",
    control_definition = control_definition_button,
    control_appearance = button_appearance,
    control_logic = button_logic,
    --exampple override
    control_definition.midi_on = {0x00, 0x00},
}

map = {
    {{0x90,0x00,0x00}, 'v'},
}
]]--

--[[
the ultra simple case:
midi code{0x00, 0x00, 0x00} -> unicode keypress/mouse button
midi events are 3 bytes, first byte is the status, channel/note etc second is the note number 0-127 and the last is the intensity? 0-127

complex case:
input as defined by a group/set/type of event from midi and then a function called or set of functions called.
]]--
