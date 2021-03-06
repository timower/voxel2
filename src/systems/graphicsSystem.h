#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <cstdarg>

#include "entitySystem.h"
#include "../transform.h"
#include "physicsSystem.h"
#include "../platform.h"

struct SystemData;

#define MAX_DRAWABLES 128

struct GraphicsInit {
	GLuint VAO;
	GLuint TEX;
	GLint program;

	size_t nVertices;
	AABB aabb;
};

struct GraphicsComponent {
	Handle handle;
	Handle entity;

	glm::mat4 modelMat;
	glm::vec3 pos;
	AABB aabb;

	GLuint VAO;
	GLuint TEX;
	GLint program;

	size_t nVertices;
};

struct GraphicsData {
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	//GraphicsComponent drawables[MAX_DRAWABLES];
	//uint16_t nDrawables;
	Container<GraphicsComponent, MAX_DRAWABLES, SystemTypes::GRAPHICS> drawables;
};


Handle addDrawComponent(SystemData& systemData, Handle entityHndl);
//GraphicsComponent& getGraphicsComponent(GraphicsData& graphicsData, Handle component);

Handle addCameraComponent(SystemData& systemData, Handle entityHndl);

void initGraphicsSystem(GraphicsData& graphicsData, Config& config);

void updateGraphicsSystem(GraphicsData& graphicsData);

void sendGraphicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
