#ifndef UNNAMED_H
#define UNNAMED_H

#include <cstddef>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define KILOBYTES(x) (1024 * (x))
#define MEGABYTES(x) (1024 * KILOBYTES(x))
#define GIGABYTES(x) (1024 * MEGABYTES(x))

struct Config {
	size_t width, height;
	int vsync;
	float fov;
};

void stop();

#endif
