#include "resources.h"
#include "platform.h"

#include <cstdio>
#include <cstdlib>

#include <SOIL.h>

uint8_t* readFile(const char* name, size_t* size) {
	FILE* file = fopen(name, "r");

	fseek(file, 0, SEEK_END);
	size_t s = ftell(file);
	rewind(file);

	uint8_t* buffer = (uint8_t*)malloc(s);
	size_t read = fread(buffer, 1, s, file);
	if (read != s) {
		free(buffer);
		printf("Error reading: %s\n", name);
		stop();
		return nullptr;
	}
	fclose(file);
	*size = s;
	return buffer;
}

void writeFile(const char* name, const char* data, size_t size) {
	FILE* file = fopen(name, "wb");
	if (fwrite(data, size, 1, file) != size)  {
		printf("error writing file: %s\n", name);
		stop();
	}
	fclose(file);
}

GLint loadShaderFile(const char* vertPath, const char* fragPath) {
	char *vertSource, *fragSource;
	size_t vertSize, fragSize;

	vertSource = (char*)readFile(vertPath, &vertSize);
	if (!vertSource)
		return 0;
	vertSource[vertSize] = 0;
	fragSource = (char*)readFile(fragPath, &fragSize);
	if (!fragSource)
		return 0;
	fragSource[fragSize] = 0;
	GLint ret = loadShader(vertSource, fragSource);
	free(vertSource);
	free(fragSource);
	return ret;
}

GLint loadShader(const char* vertSource, const char* fragSource) {
	GLint success;
	char error[512];

	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, error);
		printf("vert shader error: %s\n", error);
		printf("%s\n", vertSource);
		stop();
	}

	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, error);
		printf("frag shader error: %s\n", error);
		printf("%s\n", fragSource);
		stop();
	}

	GLint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		printf("link error\n");
		stop();
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}

void loadTextureFile(const char* path, GLenum target, int* width, int* height) {
	int w, h;
	uint8_t* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGBA);
	if (!image) {
		printf("error loading texture: %s\n", path);
		stop();
	}
	glTexImage2D(target, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	*width = w;
	*height = h;
}
