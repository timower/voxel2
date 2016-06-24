#ifndef MESSAGE_H
#define MESSAGE_H

#include "systems/systemData.h"

#include <iostream>

enum MessageType {
	// entity:
	DESTROY = 1,
	// graphics
	SET_GRAPH_TRANS,	// <Transform*> 	sets the graphics matrix,
	GET_VBO,			// <GLuint*>		gets the vbo of the drawable.
	SET_NVERTICES,		// <size_t*>		sets the amount of vertices.
	// transform
	SET_TRANSFORM,		// <Transform*> 	sets the entity's transform.
	GET_TRANSFORM,		// <Transform*>		gets the entity's transform.
	// physics
	SET_VELOCITY,		// <glm::vec3*> 	sets the velocity.
};

typedef void(*MessageHandler)(SystemData&, Handle, uint32_t, void*);

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

MessageHandler messageHandlers[] = {
	sendEntityMessage,		// entity
	sendTransformMessage,	// transform
	sendGraphicsMessage,	// graphics
	sendGraphicsMessage,	// camera
	sendPhysicsMessage,		// physics
	sendInputMessage,		// input
	sendChunkMessage,
};

#endif
