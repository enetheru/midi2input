Generic Install Instructions
-------
Install dependencies:

* build time: git cmake spdlog, fmt, openimageio gtk-update-icon-cache
* required: spdlog, lua 5.3, alsa-lib, libx11, libxtst
* Optional: jack | jack2, qt5-core, qt-widgets

```bash
git clone https://gitlab.com/enetheru/midi2input.git
cd midi2input
mkdir build
cd build
cmake ../midi2input/
make
make install
gtk-update-icon-cache -q -t -f usr/share/icons/hicolor
```
