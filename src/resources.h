#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstdint>
#include <cstddef>
#include <GL/glew.h>

uint8_t* readFile(const char* name, size_t* size);
void writeFile(const char* name, const char* data, size_t size);

GLint loadShaderFile(const char* vertPath, const char* fragPath);
GLint loadShader(const char* vertSource, const char* fragSource);

void loadTextureFile(const char* path, GLenum target, int* width, int* height);

#endif
