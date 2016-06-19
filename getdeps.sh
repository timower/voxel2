#!/bin/sh

cd deps
rm -rf *

# download:

wget -O glfw.zip https://github.com/glfw/glfw/releases/download/3.2/glfw-3.2.zip
unzip glfw.zip
mv glfw-3.2/ glfw/
rm glfw.zip

wget -O glm.tar.gz https://github.com/g-truc/glm/archive/0.9.7.5.tar.gz
tar xf glm.tar.gz
mv glm-0.9.7.5/ glm/
rm glm.tar.gz

wget -O soil.zip http://www.lonesock.net/files/soil.zip
unzip soil.zip
mv 'Simple OpenGL Image Library'/ SOIL/
rm soil.zip

git clone https://github.com/nigels-com/glew.git

# build:
# glfw:
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
cd glew/auto
make
cd ..
make

