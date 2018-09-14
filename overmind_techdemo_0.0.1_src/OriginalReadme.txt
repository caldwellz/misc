This is the readme for Overmind.

Overmind is still in a very, very early tech demo state. As such, it doesn't currently seem to do much and isn't terribly interesting.
It just executes a demo GUI layout, and moves a clock sound back and forth in 3D space.
But hey, it's a start :D

=== Building ===
Overmind uses the CMake build system (cmake.org) which generates project files for your native toolchain.
Just run CMake and then build using your generated project files.

*nix users will probably want to run `cd build && cmake -G "Unix Makefiles" ..`
Windows users will probably want to run either `cd build && cmake -G "MinGW Makefiles" ..` or `cd build && cmake -G "Visual Studio x" ..`, depending on preferred toolchain.


=== Dependencies ===
Techdemo-0.0.1 depends on:

POCO >= 1.4.3 (Tested with 1.4.3)
OGRE >= 1.7.2 (Tested with 1.7.2, 1.7.4)
OIS >= 1.2.0 (Tested with 1.2.0)
CEGUI >= 0.7.6 (Tested with 0.7.6)
OgreOggSound >= 1.21 (Tested with 1.21)
