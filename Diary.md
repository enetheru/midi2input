Enetheru: Sometime back in the day
------------------------
so on inspection of wanting to update the documentation i have come to the
conclusion that:
* README.md - is primarily an advertisement and depth of description and not used for
reference
* man page - is the primary reference inforamtion and it might be possible to manually update the README.md by tacking on the man page at the bottom.

so README.md = advertisement + deep description + man page
that way it becomes easier to manage.

so an idea i had which was to convert the man page to markdown and tack it onto
the end of README.md turned out to be harder than i expected.

it appears as though nobody is really converting man to markdown, they do it
the other way which IMHO seems weird.. anyway.

I needed to get http://www.kylheku.com/cgit/man/
so that i had a better man2html converter, beacuse the one that comes with arch
is shit.

then i used pandoc to convert that to markdown and i still have to clean it up.
the commands are thus:
* build project first
* man2html m2i.1 > temp.html
* pandoc -f html -t markdown temp.html temp.md

or all in one go

cmake ../ && ~/bin/man2html < m2i.1 > test.html && pandoc -f html -t markdown test.html > test.md

then manually edit.. its notas good as automatic but it isnt totally shit.

this helps:
https://liw.fi/manpages/

Enetheru: Tue 15 Sep 2020 12:24:22
-----------------------------
Looks like this project is pre-diary, which is interesting. All newer things I
create have a Diary to use as a thought dump.

Anyway, Today I decided to take a look at where this stands, I've had a few
interested parties over the years, but they as expected come and go, I would
like perhaps to make this friendlier to updating with features that make sense.

But mainly I wish to perform some maintenance more than anything, it's been a
while and that can be informative.

Enetheru: Sun 20 Sep 2020 16:02:29
----------------------------------
Whilst reviewing some of the code today to try to clean up some items, create
QT menu's and such I have started thinking about how the various components
need a consistent interface so that  interacting with it via LUA, QT, DBus,
c++, and anything else that comes along will be consistent.

I would expect that these things bundled into a sort of library as a core
component, and then shared among the optional interfaces would be the way to
go.

It becomes more and more like ingesting a thing into the lua and then spitting
it back out after modification just with a focus on midi. And as such makes me
think that there is probably a lower level subsystem that does this job already
that I am simply unaware of.

Which sends me down one of the other goals for the project, to write a
uinput/evdev driver so that it can work regardless of xorg. True midi2input
conversion.

[Linux Input Subsystem Userspace API](https://www.kernel.org/doc/html/v4.12/input/input.html)

It's always a cascade of todo's till you hit bedrock and refactor the whole
project lol.

So far I've at least identified some things that need to change.
* libify the main app
* provide consistent interface
* turn to uinput to really simplify/push the project forward.

There dont appear to be too many examples of uinput/libevdev that are
immediately obvious, but I bet there are a ton of real world code using them.

Really if we can get uinput/evdev input working then we can nix the x11
dependency and make it viable for headless systems

Been reading and reading, good old Who-T an Adelaide born fella, for coming
through with the goods again..
[creating an uinput device](https://who-t.blogspot.com/2013/09/libevdev-creating-uinput-devices.html)

Looks like Who-T has convinced me to use libevdev even though the freedesktop
site is broken and wont let me register. He has a synced fork though so should
be OK.

His blog is a treasure trove of input related things, I remember back in the
day following along his work on multi seat setups, thought collab on blender
could have been epic.

Enetheru: Mon 21 Sep 2020 14:34:03
----------------------------------
it's entirely possible that after implementing the midi2input evdev translation
layer, that it could be used for all devices immediately as midi is generic.

It might be nice to massage the data a little in some cases, but it appears to
be possible to do a straight forward thing.

Then all the reactionary things can be performed like all the other input
things like volume keys, and power buttons, laptop lid switch etc.

Certainly makes me think on how I want to push this project forward. Seems
simpler and more complicated at the same time.

Enetheru: Tue 22 Sep 2020 10:32:48
----------------------------------
So continuing down this uinput thread, when creating a new input device from
scratch you need to enable the codes individually to unmask them and allow the
kernel to send through the codes.

Firstly by event type `libevdev_enable_event_type(dev, type );`
And then by event code `libevdev_enable_event_code(dev, type, code, data);`

This makes me think that there needs to be a configuration file to firstly
enable these things on creation. I wonder what the consequences of enabling
everything would be.

This is useful
[Difference between uinput and evdev](https://who-t.blogspot.com/2016/05/the-difference-between-uinput-and-evdev.html)
[Understanding evdev](https://who-t.blogspot.com/2016/09/understanding-evdev.html)
[input event code descriptions](https://www.kernel.org/doc/html/v4.12/input/event-codes.html)

The thing I'm stuck on is what the convention is for generating events, and
whether that will provide the outcomes I want.
Because evdev events can be very interesting as they are composed of multiple
event/code tuples with optional data, before a SYN synchronisation frame.

This means that its possible to compose up some fairly complicated things if
desired, but that doesn't mean that it will be useful, so the standard
convention might be to copy whatever my current keyboard does.

Another thing that bugs me is that there isn't any description of arbitrary
codes anywhere, most of what is described shows standard mouse/keyboard
hardware, touchpad(because that's relatively new) and joysticks. nothing in
there about completely custom random buttons that don't have a standardisation.

And would that even show up in window managers as a thing to map to hotkey
combinations for software?

I'd rather not fudge things if I can help it, its important to me to put some
effort into getting it right over expediency.

As I understand things currently:
* Configuration file to unmask evdev event and code bits
* mapping to transcode midi events to evdev emissions

I suppose the expedient thing is to just replace all xtst code with libevdev
code.

It would still be nice to detect the current application and switch layouts
based on detection, because its not like a keyboard. So some X thing will need
to stay for now.

A thread on Stack overflow describing the problem of detecting active
window with links to various projects attempting to implement this feature
[Stack Overflow Thread](https://stackoverflow.com/questions/45465016/how-do-i-get-the-active-window-on-gnome-wayland)

Lets see, the naive approach would be to simply emit keyboard events, then at
least the buttons can be mapped to application functionality.

Given I cannot re-implement focused application re-mapping then perhaps that
could be something for the qt menu to switch scripts, or have options.

Hmm, perhaps it would be more wise to copy the keyboard rather than create a
new one.

Enetheru: Tue 13 Oct 2020 12:47:07
----------------------------------
It appears that when udev creates the device nodes in /dev it pattern matches
against the enabled bits for input devices, which means that symlinks will be
mutually exclusive, and in order to emulate different types of hardware, it
will be necessary to have multiple evdev devices.

This pushes one more towards requiring configuration that will enable types of
hardware, and then maps midi events to those specific types.

The input-event-codes.h is laid out with sections of various input devices
and their corresponding input codes.

I imagine that within the lua configuration sections would need to be added to
enable a type of device mapping, and then what midi events to use as the
mapping.

An alternative would be to require multiple instances of m2i, but for a general
use case I don't see this as being practical. For sound engineers it might be
useful however different configuration per m2i instance, attached to separate
midi devices. So the trade off would be multiple configurations with event
routing from different midi devices within the same executable, or separate
processes. Trade-offs.

I think the simple case right now would be to implement it as simple as
possible as a replacement for the xtst code, the only question is whether
keyboard and mouse events are still generated if udev detects the device as a
joystick. and for that I need to be at my desk, not at a cafe.

Enetheru: Tue 13 Oct 2020 17:21:39
----------------------------------
So far so good, it appears that it doesn't matter if I enable everything, for me
at least the scroll actions happen even thought there isn't a mouse device.

Lets keep going.

So I have keypresses working, and scroll events. does it show up in gamepad
programs? yup, tested with qjoypad for translating gamepad events to
keypresses.

This basically allows us to do straight translation from midi directly to input
of any kind. Now how to make nice.
