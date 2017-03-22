# Vortex Engine         {#mainpage}
A 3D Game Engine developed to implement/test a game engine from scratch.

## Disclaimer
Vortex is currently under heavy development and will most likely not be fully operational/buildable at this moment. 

## Requirements
1. CMake 3.0+.
2. Compiler with support for C++11.
3. GPU supporting OGL 4.0+
4. OS: Windows or Linux (so far)

## Build Instructions
See [Build Instructions](buildinstructions.md).

## Usage
The Example folder contains two folders, "code" and "resources". 
In the code folder you can put all the project specific code that you project might be using.
The engine will look for resources and assets in the resources folder. If not, make sure your working directory is pointing at you test-projects root folder.

## Example Project
The example project is simply an example implementation which renders two assets.

## Documentation
Documentation can be generated with Doxygen by running it with `doxy_config.conf`.
This will generate a `docs` folder which will contain all the documentation.

Documentation is written directly in the code using [doxygen syntax](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html) but can also be done via markdown language files like this one.
<!--- 
@todo Add a coding convention document that describes both coding conventions and comment syntax.
-->
