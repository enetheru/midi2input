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
