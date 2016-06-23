#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

#define LEFT    glm::vec3(1.0f, 0.0f, 0.0f)
#define UP      glm::vec3(0.0f, 1.0f, 0.0f)
#define FRONT   glm::vec3(0.0f, 0.0f, 1.0f)


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
