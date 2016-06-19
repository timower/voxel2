#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

glm::mat4 transformToModel(const Transform& transform) {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), transform.position);
	glm::mat4 rot   = glm::yawPitchRoll(transform.yaw, transform.pitch, transform.roll);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
	return trans * rot * scale;
}

// TODO: fix roll?
glm::mat4 transformToView(const Transform& transform) {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), -transform.position);
	glm::mat4 rot   = glm::yawPitchRoll(transform.yaw + glm::pi<float>(), -transform.pitch, transform.roll);
	rot = glm::inverse(rot);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
	return scale * rot * trans;
}

glm::mat3 transformRotation(const Transform& transform) {
	glm::mat4 ret = glm::yawPitchRoll(transform.yaw, transform.pitch, transform.roll);
	return static_cast<glm::mat3>(ret);
}
