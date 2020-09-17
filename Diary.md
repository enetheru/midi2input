Sometime back in the day
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

Tue 15 Sep 2020 12:24:22
-----------------------------
Looks like this project is pre-diary, which is interesting. All newer things I
create have a Diary to use as a thought dump.

Anyway, Today I decided to take a look at where this stands, I've had a few
interested parties over the years, but they as expected come and go, I would
like perhaps to make this friendlier to updating with features that make sense.

But mainly I wish to perform some maintenance more than anything, it's been a
while and that can be informative.
