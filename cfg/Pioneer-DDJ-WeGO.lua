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




--[[ Setup the state of the lights ]]--
controller = {}
-- friendly view
controller = {
    state = {
        deckA = {
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
    [0x0B] = { [127] = default.toggle  }, --         play
    [0x0C] = { [127] = default.toggle  }, --         cue
    [0x14] = { [127] = unassigned      }, --         autoloop
    [0x2E] = { [127] = default.toggle  }, --         hotcue1
    [0x2F] = { [127] = default.toggle  }, --         hotcue2
    [0x30] = { [127] = default.toggle  }, --         hotcue3
    [0x31] = { [127] = default.toggle  }, --         hotcue4
    [0x3C] = { [127] = default.toggle  }, --         sampler1
    [0x3D] = { [127] = unassigned      }, -- [shift] sampler1
    [0x3E] = { [127] = default.toggle  }, --         sampler2
    [0x3F] = { [127] = unassigned      }, -- [shift] sampler2
    [0x40] = { [127] = default.toggle  }, --         sampler3
    [0x41] = { [127] = unassigned      }, -- [shift] sampler3
    [0x42] = { [127] = default.toggle  }, --         sampler4
    [0x43] = { [127] = unassigned      }, -- [shift] sampler4
    [0x47] = { [127] = unassigned      }, -- [shift] play
    [0x48] = { [127] = unassigned      }, -- [shift] cue
    [0x50] = { [127] = unassigned      }, -- [shift] autoloop
    [0x58] = { [127] = default.toggle  }, --         sync
    [0x59] = { [127] = unassigned      }, --         sampler
    [0x5C] = { [127] = unassigned      }, -- [shift] sync
    [0x5F] = { [127] = unassigned      }, -- [shift] hotcue1
    [0x60] = { [127] = unassigned      }, -- [shift] hotcue2
    [0x61] = { [127] = unassigned      }, -- [shift] hotcue3
    [0x62] = { [127] = unassigned      }, -- [shift] hotcue4
}
default.map[0xB0] = {
    [0x00] = { ['*'] = MSB             }, --         tempo MSB
    [0x05] = { ['*'] = MSB             }, -- [shift] tempo MSB
    [0x13] = { ['*'] = unassigned      }, --         auto loop turn
    [0x20] = { ['*'] = unassigned      }, --         tempo LSB
    [0x21] = { ['*'] = unassigned      }, --         jog wheel
    [0x22] = { ['*'] = unassigned      }, --         scratch
    [0x25] = { ['*'] = unassigned      }, -- [shift] tempo LSB
    [0x26] = { ['*'] = unassigned      }, -- [shift] jog wheel
    [0x27] = { ['*'] = unassigned      }, -- [shift] scratch
    [0x4F] = { ['*'] = unassigned      }, -- [shift] auto loop turn
}
--deckB
default.map[0x91] = {
    [0x0B] = { [127] = default.toggle  }, --         play
    [0x0C] = { [127] = default.toggle  }, --         cue
    [0x14] = { [127] = unassigned      }, --         autoloop
    [0x2E] = { [127] = default.toggle  }, --         hotcue1
    [0x2F] = { [127] = default.toggle  }, --         hotcue2
    [0x30] = { [127] = default.toggle  }, --         hotcue3
    [0x31] = { [127] = default.toggle  }, --         hotcue4
    [0x3C] = { [127] = default.toggle  }, --         sampler1
    [0x3D] = { [127] = unassigned      }, -- [shift] sampler1
    [0x3E] = { [127] = default.toggle  }, --         sampler2
    [0x3F] = { [127] = unassigned      }, -- [shift] sampler2
    [0x40] = { [127] = default.toggle  }, --         sampler3
    [0x41] = { [127] = unassigned      }, -- [shift] sampler3
    [0x42] = { [127] = default.toggle  }, --         sampler4
    [0x43] = { [127] = unassigned      }, -- [shift] sampler4
    [0x47] = { [127] = unassigned      }, -- [shift] play
    [0x48] = { [127] = unassigned      }, -- [shift] cue
    [0x50] = { [127] = unassigned      }, -- [shift] autoloop
    [0x58] = { [127] = default.toggle  }, --         sync
    [0x59] = { [127] = unassigned      }, --         sampler
    [0x5C] = { [127] = unassigned      }, -- [shift] sync
    [0x5F] = { [127] = unassigned      }, -- [shift] hotcue1
    [0x60] = { [127] = unassigned      }, -- [shift] hotcue2
    [0x61] = { [127] = unassigned      }, -- [shift] hotcue3
    [0x62] = { [127] = unassigned      }, -- [shift] hotcue4
}
default.map[0xB1] = {
    [0x00] = { ['*'] = MSB             }, --         tempo MSB
    [0x05] = { ['*'] = MSB             }, -- [shift] tempo MSB
    [0x13] = { ['*'] = unassigned      }, --         auto loop turn
    [0x20] = { ['*'] = unassigned      }, --         tempo LSB
    [0x21] = { ['*'] = unassigned      }, --         jog wheel
    [0x22] = { ['*'] = unassigned      }, --         scratch
    [0x25] = { ['*'] = unassigned      }, -- [shift] tempo LSB
    [0x26] = { ['*'] = unassigned      }, -- [shift] jog wheel
    [0x27] = { ['*'] = unassigned      }, -- [shift] scratch
    [0x4F] = { ['*'] = unassigned      }, -- [shift] auto loop turn
}
--deckC
default.map[0x92] = {
    [0x0B] = { [127] = default.toggle  }, --         play
    [0x0C] = { [127] = default.toggle  }, --         cue
    [0x14] = { [127] = unassigned      }, --         autoloop
    [0x2E] = { [127] = default.toggle  }, --         hotcue1
    [0x2F] = { [127] = default.toggle  }, --         hotcue2
    [0x30] = { [127] = default.toggle  }, --         hotcue3
    [0x31] = { [127] = default.toggle  }, --         hotcue4
    [0x3C] = { [127] = default.toggle  }, --         sampler1
    [0x3D] = { [127] = unassigned      }, -- [shift] sampler1
    [0x3E] = { [127] = default.toggle  }, --         sampler2
    [0x3F] = { [127] = unassigned      }, -- [shift] sampler2
    [0x40] = { [127] = default.toggle  }, --         sampler3
    [0x41] = { [127] = unassigned      }, -- [shift] sampler3
    [0x42] = { [127] = default.toggle  }, --         sampler4
    [0x43] = { [127] = unassigned      }, -- [shift] sampler4
    [0x47] = { [127] = unassigned      }, -- [shift] play
    [0x48] = { [127] = unassigned      }, -- [shift] cue
    [0x50] = { [127] = unassigned      }, -- [shift] autoloop
    [0x58] = { [127] = default.toggle  }, --         sync
    [0x59] = { [127] = unassigned      }, --         sampler
    [0x5C] = { [127] = unassigned      }, -- [shift] sync
    [0x5F] = { [127] = unassigned      }, -- [shift] hotcue1
    [0x60] = { [127] = unassigned      }, -- [shift] hotcue2
    [0x61] = { [127] = unassigned      }, -- [shift] hotcue3
    [0x62] = { [127] = unassigned      }, -- [shift] hotcue4
}
default.map[0xB2] = {
    [0x00] = { ['*'] = MSB             }, --         tempo MSB
    [0x05] = { ['*'] = MSB             }, -- [shift] tempo MSB
    [0x13] = { ['*'] = unassigned      }, --         auto loop turn
    [0x20] = { ['*'] = unassigned      }, --         tempo LSB
    [0x21] = { ['*'] = unassigned      }, --         jog wheel
    [0x22] = { ['*'] = unassigned      }, --         scratch
    [0x25] = { ['*'] = unassigned      }, -- [shift] tempo LSB
    [0x26] = { ['*'] = unassigned      }, -- [shift] jog wheel
    [0x27] = { ['*'] = unassigned      }, -- [shift] scratch
    [0x4F] = { ['*'] = unassigned      }, -- [shift] auto loop turn
}
--deckD
default.map[0x93] = {
    [0x0B] = { [127] = default.toggle  }, --         play
    [0x0C] = { [127] = default.toggle  }, --         cue
    [0x14] = { [127] = unassigned      }, --         autoloop
    [0x2E] = { [127] = default.toggle  }, --         hotcue1
    [0x2F] = { [127] = default.toggle  }, --         hotcue2
    [0x30] = { [127] = default.toggle  }, --         hotcue3
    [0x31] = { [127] = default.toggle  }, --         hotcue4
    [0x3C] = { [127] = default.toggle  }, --         sampler1
    [0x3D] = { [127] = unassigned      }, -- [shift] sampler1
    [0x3E] = { [127] = default.toggle  }, --         sampler2
    [0x3F] = { [127] = unassigned      }, -- [shift] sampler2
    [0x40] = { [127] = default.toggle  }, --         sampler3
    [0x41] = { [127] = unassigned      }, -- [shift] sampler3
    [0x42] = { [127] = default.toggle  }, --         sampler4
    [0x43] = { [127] = unassigned      }, -- [shift] sampler4
    [0x47] = { [127] = unassigned      }, -- [shift] play
    [0x48] = { [127] = unassigned      }, -- [shift] cue
    [0x50] = { [127] = unassigned      }, -- [shift] autoloop
    [0x58] = { [127] = default.toggle  }, --         sync
    [0x59] = { [127] = unassigned      }, --         sampler
    [0x5C] = { [127] = unassigned      }, -- [shift] sync
    [0x5F] = { [127] = unassigned      }, -- [shift] hotcue1
    [0x60] = { [127] = unassigned      }, -- [shift] hotcue2
    [0x61] = { [127] = unassigned      }, -- [shift] hotcue3
    [0x62] = { [127] = unassigned      }, -- [shift] hotcue4
}
default.map[0xB3] = {
    [0x00] = { ['*'] = MSB             }, --         tempo MSB
    [0x05] = { ['*'] = MSB             }, -- [shift] tempo MSB
    [0x13] = { ['*'] = unassigned      }, --         auto loop turn
    [0x20] = { ['*'] = unassigned      }, --         tempo LSB
    [0x21] = { ['*'] = unassigned      }, --         jog wheel
    [0x22] = { ['*'] = unassigned      }, --         scratch
    [0x25] = { ['*'] = unassigned      }, -- [shift] tempo LSB
    [0x26] = { ['*'] = unassigned      }, -- [shift] jog wheel
    [0x27] = { ['*'] = unassigned      }, -- [shift] scratch
    [0x4F] = { ['*'] = unassigned      }, -- [shift] auto loop turn
}
--ctrl & fx for deckAC
default.map[0x94] = {
    [0x42] = { [127] = default.toggle  }, --         deckA ctrlA
    [0x43] = { [127] = default.toggle  }, --         deckA fx1
    [0x44] = { [127] = default.toggle  }, --         deckA fx2
    [0x45] = { [127] = default.toggle  }, --         deckA fx3
    [0x46] = { [127] = default.toggle  }, --         deckA ctrlB
    [0x47] = { [127] = default.toggle  }, --         deckC ctrlA
    [0x48] = { [127] = default.toggle  }, --         deckC fx1
    [0x49] = { [127] = default.toggle  }, --         deckC fx2
    [0x4A] = { [127] = default.toggle  }, --         deckC fx3
    [0x4B] = { [127] = default.toggle  }, --         deckC ctrlB
    [0x4D] = { [127] = unassigned      }, -- [shift] deckA fx1
    [0x4E] = { [127] = unassigned      }, -- [shift] deckA fx2
    [0x4F] = { [127] = unassigned      }, -- [shift] deckA fx3
    [0x52] = { [127] = unassigned      }, -- [shift] deckC fx1
    [0x53] = { [127] = unassigned      }, -- [shift] deckC fx2
    [0x54] = { [127] = unassigned      }, -- [shift] deckC fx3
}
default.map[0xB4] = {
--deckA
    [0x00] = { ['*'] = MSB         }, --         ctrlA wheel MSB
    [0x20] = { ['*'] = unassigned  }, --         ctrlA wheel LSB
    [0x02] = { ['*'] = MSB         }, --         fx1 wheel MSB
    [0x22] = { ['*'] = unassigned  }, --         fx1 wheel LSB
    [0x04] = { ['*'] = MSB         }, --         fx2 wheel MSB
    [0x24] = { ['*'] = unassigned  }, --         fx2 wheel LSB
    [0x06] = { ['*'] = MSB         }, --         fx3 wheel MSB
    [0x26] = { ['*'] = unassigned  }, --         fx3 wheel LSB
    [0x07] = { ['*'] = MSB         }, --         ctrlB wheel MSB
    [0x27] = { ['*'] = unassigned  }, --         ctrlB wheel LSB
    [0x10] = { ['*'] = MSB         }, -- [shift] ctrlA wheel MSB
    [0x30] = { ['*'] = unassigned  }, -- [shift] ctrlA wheel LSB
    [0x12] = { ['*'] = MSB         }, -- [shift] fx1 wheel MSB
    [0x32] = { ['*'] = unassigned  }, -- [shift] fx1 wheel LSB
    [0x14] = { ['*'] = MSB         }, -- [shift] fx2 wheel MSB
    [0x34] = { ['*'] = unassigned  }, -- [shift] fx2 wheel LSB
    [0x16] = { ['*'] = MSB         }, -- [shift] fx3 wheel MSB
    [0x36] = { ['*'] = unassigned  }, -- [shift] fx3 wheel LSB
    [0x17] = { ['*'] = MSB         }, -- [shift] ctrlB wheel MSB
    [0x37] = { ['*'] = unassigned  }, -- [shift] ctrlB wheel LSB
--deckC
    [0x08] = { ['*'] = MSB         }, --         ctrlA wheel MSB
    [0x28] = { ['*'] = unassigned  }, --         ctrlA wheel LSB
    [0x0A] = { ['*'] = MSB         }, --         fx1 wheel MSB
    [0x2A] = { ['*'] = unassigned  }, --         fx1 wheel LSB
    [0x0C] = { ['*'] = MSB         }, --         fx2 wheel MSB
    [0x2C] = { ['*'] = unassigned  }, --         fx2 wheel LSB
    [0x0E] = { ['*'] = MSB         }, --         fx3 wheel MSB
    [0x2E] = { ['*'] = unassigned  }, --         fx3 wheel LSB
    [0x0F] = { ['*'] = MSB         }, --         ctrlB wheel MSB
    [0x2F] = { ['*'] = unassigned  }, --         ctrlB wheel LSB
    [0x18] = { ['*'] = MSB         }, -- [shift] ctrlA wheel MSB
    [0x38] = { ['*'] = unassigned  }, -- [shift] ctrlA wheel LSB
    [0x1A] = { ['*'] = MSB         }, -- [shift] fx1 wheel MSB
    [0x3A] = { ['*'] = unassigned  }, -- [shift] fx1 wheel LSB
    [0x1C] = { ['*'] = MSB         }, -- [shift] fx2 wheel MSB
    [0x3C] = { ['*'] = unassigned  }, -- [shift] fx2 wheel LSB
    [0x1E] = { ['*'] = MSB         }, -- [shift] fx3 wheel MSB
    [0x3E] = { ['*'] = unassigned  }, -- [shift] fx3 wheel LSB
    [0x1F] = { ['*'] = MSB         }, -- [shift] ctrlB wheel MSB
    [0x3F] = { ['*'] = unassigned  }, -- [shift] ctrlB wheel LSB
}
--ctrl & fx for deckBD
default.map[0x95] = {
    [0x42] = { [127] = default.toggle  }, --         deckB ctrlA
    [0x43] = { [127] = default.toggle  }, --         deckB fx1
    [0x44] = { [127] = default.toggle  }, --         deckB fx2
    [0x45] = { [127] = default.toggle  }, --         deckB fx3
    [0x46] = { [127] = default.toggle  }, --         deckB ctrlB
    [0x47] = { [127] = default.toggle  }, --         deckD ctrlA
    [0x48] = { [127] = default.toggle  }, --         deckD fx1
    [0x49] = { [127] = default.toggle  }, --         deckD fx2
    [0x4A] = { [127] = default.toggle  }, --         deckD fx3
    [0x4B] = { [127] = default.toggle  }, --         deckD ctrlB
    [0x4D] = { [127] = unassigned      }, -- [shift] deckB fx1
    [0x4E] = { [127] = unassigned      }, -- [shift] deckB fx2
    [0x4F] = { [127] = unassigned      }, -- [shift] deckB fx3
    [0x52] = { [127] = unassigned      }, -- [shift] deckD fx1
    [0x53] = { [127] = unassigned      }, -- [shift] deckD fx2
    [0x54] = { [127] = unassigned      }, -- [shift] deckD fx3
}
default.map[0xB5] = {
--deckB
    [0x00] = { ['*'] = MSB         }, --         ctrlA wheel MSB
    [0x20] = { ['*'] = unassigned  }, --         ctrlA wheel LSB
    [0x02] = { ['*'] = MSB         }, --         fx1 wheel MSB
    [0x22] = { ['*'] = unassigned  }, --         fx1 wheel LSB
    [0x04] = { ['*'] = MSB         }, --         fx2 wheel MSB
    [0x24] = { ['*'] = unassigned  }, --         fx2 wheel LSB
    [0x06] = { ['*'] = MSB         }, --         fx3 wheel MSB
    [0x26] = { ['*'] = unassigned  }, --         fx3 wheel LSB
    [0x07] = { ['*'] = MSB         }, --         ctrlB wheel MSB
    [0x27] = { ['*'] = unassigned  }, --         ctrlB wheel LSB
    [0x10] = { ['*'] = MSB         }, -- [shift] ctrlA wheel MSB
    [0x30] = { ['*'] = unassigned  }, -- [shift] ctrlA wheel LSB
    [0x12] = { ['*'] = MSB         }, -- [shift] fx1 wheel MSB
    [0x32] = { ['*'] = unassigned  }, -- [shift] fx1 wheel LSB
    [0x14] = { ['*'] = MSB         }, -- [shift] fx2 wheel MSB
    [0x34] = { ['*'] = unassigned  }, -- [shift] fx2 wheel LSB
    [0x16] = { ['*'] = MSB         }, -- [shift] fx3 wheel MSB
    [0x36] = { ['*'] = unassigned  }, -- [shift] fx3 wheel LSB
    [0x17] = { ['*'] = MSB         }, -- [shift] ctrlB wheel MSB
    [0x37] = { ['*'] = unassigned  }, -- [shift] ctrlB wheel LSB
--deckD
    [0x08] = { ['*'] = MSB         }, --         ctrlA wheel MSB
    [0x28] = { ['*'] = unassigned  }, --         ctrlA wheel LSB
    [0x0A] = { ['*'] = MSB         }, --         fx1 wheel MSB
    [0x2A] = { ['*'] = unassigned  }, --         fx1 wheel LSB
    [0x0C] = { ['*'] = MSB         }, --         fx2 wheel MSB
    [0x2C] = { ['*'] = unassigned  }, --         fx2 wheel LSB
    [0x0E] = { ['*'] = MSB         }, --         fx3 wheel MSB
    [0x2E] = { ['*'] = unassigned  }, --         fx3 wheel LSB
    [0x0F] = { ['*'] = MSB         }, --         ctrlB wheel MSB
    [0x2F] = { ['*'] = unassigned  }, --         ctrlB wheel LSB
    [0x18] = { ['*'] = MSB         }, -- [shift] ctrlA wheel MSB
    [0x38] = { ['*'] = unassigned  }, -- [shift] ctrlA wheel LSB
    [0x1A] = { ['*'] = MSB         }, -- [shift] fx1 wheel MSB
    [0x3A] = { ['*'] = unassigned  }, -- [shift] fx1 wheel LSB
    [0x1C] = { ['*'] = MSB         }, -- [shift] fx2 wheel MSB
    [0x3C] = { ['*'] = unassigned  }, -- [shift] fx2 wheel LSB
    [0x1E] = { ['*'] = MSB         }, -- [shift] fx3 wheel MSB
    [0x3E] = { ['*'] = unassigned  }, -- [shift] fx3 wheel LSB
    [0x1F] = { ['*'] = MSB         }, -- [shift] ctrlB wheel MSB
    [0x3F] = { ['*'] = unassigned  }, -- [shift] ctrlB wheel LSB
}
-- Master
default.map[0x96] = {
    [0x41] = { [127] = unassigned      }, --         browse
    [0x42] = { [127] = unassigned      }, -- [shift] browse
    [0x46] = { [127] = unassigned      }, --         load deckA
    [0x47] = { [127] = unassigned      }, --         load deckB
    [0x48] = { [127] = unassigned      }, --         load deckC
    [0x49] = { [127] = unassigned      }, --         load deckD
    [0x54] = { [127] = default.toggle  }, --         headphone deckA
    [0x55] = { [127] = default.toggle  }, --         headphone deckB
    [0x56] = { [127] = default.toggle  }, --         headphone deckC
    [0x57] = { [127] = default.toggle  }, --         headphone deckD
    [0x58] = { [127] = unassigned      }, -- [shift] load deckA
    [0x59] = { [127] = unassigned      }, -- [shift] load deckB
    [0x5A] = { [127] = unassigned      }, -- [shift] load deckC
    [0x5B] = { [127] = unassigned      }, -- [shift] load deckD
    [0x5C] = { [127] = unassigned      }, -- [shift] headphone deckA
    [0x5D] = { [127] = unassigned      }, -- [shift] headphone deckB
    [0x5E] = { [127] = unassigned      }, -- [shift] headphone deckC
    [0x5F] = { [127] = unassigned      }, -- [shift] headphone deckD
}
default.map[0xB6] = {
    [0x00] = { ['*'] = MSB             }, -- [shift] crossfader MSB
    [0x20] = { ['*'] = unassigned      }, -- [shift] crossfader LSB
    [0x01] = { ['*'] = MSB             }, --         mix MSB
    [0x21] = { ['*'] = unassigned      }, --         mix LSB
    [0x1F] = { ['*'] = MSB             }, --         crossfader MSB
    [0x3F] = { ['*'] = unassigned      }, --         crossfader LSB
--deckA
    [0x07] = { ['*'] = MSB             }, --         high MSB
    [0x27] = { ['*'] = unassigned      }, --         high LSB
    [0x0B] = { ['*'] = MSB             }, --         mid MSB
    [0x2B] = { ['*'] = unassigned      }, --         mid LSB
    [0x0F] = { ['*'] = MSB             }, --         low MSB
    [0x2F] = { ['*'] = unassigned      }, --         low LSB
    [0x13] = { ['*'] = MSB             }, --         volume MSB
    [0x33] = { ['*'] = unassigned      }, --         volume LSB
    [0x14] = { ['*'] = MSB             }, -- [shift] volume MSB
    [0x34] = { ['*'] = unassigned      }, -- [shift] volume LSB
--deckB
    [0x08] = { ['*'] = MSB             }, --         high MSB
    [0x28] = { ['*'] = unassigned      }, --         high LSB
    [0x0C] = { ['*'] = MSB             }, --         mid MSB
    [0x2C] = { ['*'] = unassigned      }, --         mid LSB
    [0x10] = { ['*'] = MSB             }, --         low MSB
    [0x30] = { ['*'] = unassigned      }, --         low LSB
    [0x15] = { ['*'] = MSB             }, --         volume MSB
    [0x35] = { ['*'] = unassigned      }, --         volume LSB
    [0x16] = { ['*'] = MSB             }, -- [shift] volume MSB
    [0x36] = { ['*'] = unassigned      }, -- [shift] volume LSB
--deckC
    [0x09] = { ['*'] = MSB             }, --         high MSB
    [0x29] = { ['*'] = unassigned      }, --         high LSB
    [0x0D] = { ['*'] = MSB             }, --         mid MSB
    [0x2D] = { ['*'] = unassigned      }, --         mid LSB
    [0x11] = { ['*'] = MSB             }, --         low MSB
    [0x31] = { ['*'] = unassigned      }, --         low LSB
    [0x17] = { ['*'] = MSB             }, --         volume MSB
    [0x37] = { ['*'] = unassigned      }, --         volume LSB
    [0x18] = { ['*'] = MSB             }, -- [shift] volume MSB
    [0x38] = { ['*'] = unassigned      }, -- [shift] volume LSB
--deckD
    [0x0A] = { ['*'] = MSB             }, --         high MSB
    [0x2A] = { ['*'] = unassigned      }, --         high LSB
    [0x0E] = { ['*'] = MSB             }, --         mid MSB
    [0x2E] = { ['*'] = unassigned      }, --         mid LSB
    [0x12] = { ['*'] = MSB             }, --         low MSB
    [0x32] = { ['*'] = unassigned      }, --         low LSB
    [0x19] = { ['*'] = MSB             }, --         volume MSB
    [0x39] = { ['*'] = unassigned      }, --         volume LSB
    [0x1A] = { ['*'] = MSB             }, -- [shift] volume MSB
    [0x3A] = { ['*'] = unassigned      }, -- [shift] volume LSB
}

--[[ receive and react ]]--
current = {}
current = default.map

function initialise()
    print( "nothing to do" )
end

function midi_recv( status, data1, data2 )
    if      current[status]
        and current[status][data1]
    then
        if current[status][data1][data2] then
            current[status][data1][data2]( {status, data1, data2} )
        elseif current[status][data1]['*'] then
            current[status][data1]['*']( {status, data1, data2} )
        end
    end
end


