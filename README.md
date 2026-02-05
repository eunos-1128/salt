Salt
====

Salt is a terminal emulator with built-in SSH. It is currently available as an executable that uses GTK4 on Linux, but it once was a Windows application. The library used for the GUI is still cross platform so it might one day return to Windows as well.

Installation
------------

A .deb for Debian trixie (13) is available, which also seems to work for Ubuntu noble (24.04). To install:

```console
wget https://www.hekkelman.net/repo/hekkelman-archive-keyring.deb
sudo dpkg -i hekkelman-archive-keyring.deb
sudo apt update
sudo apt install salt
```

Building
--------

```console
git clone --recurse-submodules https://github.com/mhekkel/salt.git
cd salt
cmake -B build
cmake --build build
build/salt -i
```

Theoretically, that will build a new salt and the last step will install it.

Manual
------

Here you can find a markdown version of the [manual page](./salt-manual.md)