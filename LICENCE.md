project clearly links to open source projects

* ALSA is released under the GPL (GNU General Public license) and the LGPL (GNU Lesser General Public License).
* JACK is under the terms of the GNU GPL and LGPL licenses.
* adishavit/argh is released under BSD 3-clause "New" or "Revised" License.
* fmtlib is relased under BSD 2-clause "Simplified" License.
* spdlog is released under the MIT Licence.
* linking against the stdlib is exempted from having to be GPL.
* system calls like inotify are considered normal use: http://inotify.aiken.cz/?section=inotify&page=faq
* Qt is released under GPL & LGPLv3 licenses.
* xlib looks like bsd 3-clause or similar.

That being the case, looks like LGPL is the most restrictive required licence.

And since this is an end user program which I dont imagine anyone would ever
link to I'll go with the GPLv2 for my parts.