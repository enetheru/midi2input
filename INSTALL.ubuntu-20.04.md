Installation Instructions for Debian Bullseye
---------------------------------------------
I've only tested this in a VM and haven't used it for anything.
Currently the qt cmake find scripts didn't detect the qt core and widgets
libraries which prevents the use of the system tray icon, not sure what's up
with that, could use some help debugging.

```bash
apt-get install git cmake g++ liblua5.3-dev libasound2-dev libspdlog-dev libfmt-dev qtbase5-dev libev-dev openimageio-tools
  - OPTIONALLY libjack-dev
git clone https://gitlab.com/enetheru/midi2input.git
cd midi2input
mkdir build
cd build
cmake ../
make
```
You can just `make install` to get it on your system but you should generate a
package so that it can be updated and removed using the install manifest for
clean system operation.

The problem is that it depends on how you configured the application too, and i
haven't setup the CMakeLists to properly consider how that operates.

For my testing purposes I changed 

```cmake
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libspdlog1, libx11-6, jackd1, liblua5.3-0" )
```

I could then build the package and install it as below

```bash
make package
sudo dpkg -i m2i-*-Linux.deb
```
