# Vortex Engine
A 3D Game Engine developed to implement/test a game engine from scratch.

# Requirements
1. CMake 3+.
2. Compiler with support for lambda functions (C++11), such as VS 2013.
3. GPU supporting OGL 4.0+
4. OS: Windows or Linux (so far)
  
# Instructions
Put your test-project in the folder titled 'projects'. It must be accompanied with its own CMakeLists.txt which is used to generate a project/solution. Then run CMake in the root of the project. There should be a target in your solution matching the name of your test/project.
  
# Usage
The Example folder contains two folders, "code" and "resources". 
In the code folder you can put all the project specific code that you project might be using.
The engine will look for resources and assets in the resources folder. If not, make sure your working directory is pointing at you test-projects root folder.

# Example
The example project is simply an example implementation which renders two assets.
