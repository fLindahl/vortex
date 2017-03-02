# Build Instructions

## Linux

To build for Linux, you need to install the lastest version of cmake

`$ sudo apt-get install cmake`

You also need to make sure that you have GCC installed.

`$ sudo apt-get install gcc`

Open the terminal and direct it to the directory that you want to build to.

ex. : `$ cd /home/user/git/vortex/builds`

run cmake from the directory of the the engine.

ex. : `$ cmake /home/user/git/vortex/ -DCMAKE_BUILD_TYPE="Debug"`

build the engine by executing make:

`$ make`


**Note:**
If you are having trouble building on linux, you might need to install some additional packages.

`$ sudo apt-get install libgl1-mesa-dev xorg-dev libglu1-mesa-dev`

`$ sudo apt-get install build-essential libgtk-3-dev`

## Windows

Not yet documented!
