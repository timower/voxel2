#!/bin/sh

# build:
# glfw:
cd deps
cd glfw
cmake .
make glfw
cd ..
# SOIL:
cd SOIL/projects/makefile
mkdir obj
make
cd ../../..
#glew:
cd glew
make

