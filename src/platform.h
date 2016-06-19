#ifndef UNNAMED_H
#define UNNAMED_H

#include <cstddef>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WIDTH   1280
#define HEIGHT  720

#define KILOBYTES(x) (1024 * (x))
#define MEGABYTES(x) (1024 * KILOBYTES(x))
#define GIGABYTES(x) (1024 * MEGABYTES(x))

void stop();

#endif
