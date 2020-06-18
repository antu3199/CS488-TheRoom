# CS488-TheRoom
Source code for my CS488 final project titled, "The Room", which is an OpenGL animation system in C++/Lua with an example animation entitled, "The Room".

To run, setup the framework as described in https://www.student.cs.uwaterloo.ca/~cs488/Winter2020/a0.html
Then input:

`./scripts/cs488-maker A5 Assets/room.lua`

See cs488-vid.mkv for a showcase of the animation (at a low framerate)

Excerpt from the CS488 course page:
## Building Projects
We use **premake4** as our cross-platform build system. First you will need to build all
the static libraries that the projects depend on. To build the libraries, open up a
terminal, and **cd** to the top level of the CS488 project directory and then run the
following:

    $ premake4 gmake
    $ make

This will build the following static libraries, and place them in the top level **lib**
folder of your cs488 project directory.
* libcs488-framework.a
* libglfw3.a
* libimgui.a

Next we can build a specific project.  To do this, **cd** into one of the project folders,
say **A0** for example, and run the following terminal commands in order to compile the A0 executable using all .cpp files in the A0 directory:

    $ cd A0/
    $ premake4 gmake
    $ make


----

## Windows
Sorry for all of the hardcore Microsoft fans out there.  We have not had time to test the build system on Windows yet. Currently our build steps work for OSX and Linux, but all the steps should be the same on Windows, except you will need different libraries to link against when building your project executables.  Some good news is that premake4 can output a Visual Studio .sln file by running:

    $ premake4 vs2013

 This should point you in the general direction.
