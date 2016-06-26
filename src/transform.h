#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

#define LEFT    glm::vec3(1.0f, 0.0f, 0.0f)
#define UP      glm::vec3(0.0f, 1.0f, 0.0f)
#define FRONT   glm::vec3(0.0f, 0.0f, 1.0f)

#define NUP    0
#define NFRONT 1
#define NLEFT  2
#define NRIGHT 3
#define NBACK  4
#define NDOWN  5

glm::ivec3 normals[] = {
	{ 0,  1,  0}, // up
	{ 0,  0,  1}, // front
	{ 1,  0,  0}, // left
	{-1,  0,  0}, // right
	{ 0,  0, -1}, // back
	{ 0, -1,  0}, // down
};

struct Transform {
	// TODO: add AABB?
	glm::vec3 position;
	float yaw, pitch, roll;
	glm::vec3 scale;
};

static Transform ZERO = {glm::vec3(0), 0.0f, 0.0f, 0.0f, glm::vec3(0)};

glm::mat4 transformToModel(const Transform& transform);
glm::mat4 transformToView(const Transform& transform);

glm::mat3 transformRotation(const Transform& transform);

#endif
