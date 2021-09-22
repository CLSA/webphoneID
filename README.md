How to build WebphoneID
=======================

WebphoneID uses CMake (http://cmake.org/) to configure its build process.  CMake is operating system
independent, so in theory it should be possible to build this project under Linux, Windows or
Macintosh.


Required Libraries
==================

CMake (cmake.org)
-----------------
There are plenty of pre-compiled binaries of CMake available on the website, or you can download
the source and build it yourself.  Also, many linux distros have it build into their package
managers.  I recommend trying this option first since it's the quickest and easiest to do.


Qt5
---
You will need the CURL development libraries.  You can download the source and build it yourself,
or install from your linux distro.  Again, it's much easier to do this then build it yourself so
I recommend trying this first.


LIBCURL 4
---------
You will need the CURL development libraries.  You can download the source and build it yourself,
or install from your linux distro.  Again, it's much easier to do this then build it yourself so
I recommend trying this first.


Building WebphoneID
================

1. Get a copy of WebphoneID.

2. Create a build directory somewhere OUTSIDE of the WebphoneID source code tree so as not to
   polute the repository.

3. Inside your build directory, run cmake:

   For linux: cmake -G "Unix Makefiles" /path/to/WebphoneID

4. Open the CMakeCache.txt file and change the following:

   CMAKE_BUILD_TYPE:STRING=Release

5. Re-run cmake by typing "cmake ." in the build directory.  When it is done you should see:
   "Build files have been written to: ..."

6. You are now ready to build WebphoneID by typing "make".
