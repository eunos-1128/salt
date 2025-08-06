Salt
====

Salt is a terminal emulator with built-in SSH. It is currently available as an executable that uses GTK4 on Linux, but it once was a Windows application. The library used for the GUI is still cross platform so it might one day return to Windows as well.

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