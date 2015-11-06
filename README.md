midi2input
=======
Small GNU/Linux program that captures live midi input from jack midi and maps the notes to simulated key presses or mouse movement.
The map is set by a simple config file.

Requires X11 with the XTest extension and jack.

Installation
------------

### From source

  * FIXME

### Example usage

`./mid2key configfile.cfg`

### Example config file

```
#set note #32 to spacebar
32=space
#set note #33 to keycode 67
33=67
#leave note #34 unset
34=
#also set note 35 to space
35=space
```

License
-------
all rights reserved for now(just ask me and I will change it to GPL or something similar)
