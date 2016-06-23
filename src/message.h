#ifndef MESSAGE_H
#define MESSAGE_H

#include "systems/systemData.h"

#include <iostream>

enum MessageType {
	// entity:
	REMOVE_COMP = 1,
	// graphics
	SET_GRAPH_TRANS,	// <Transform*> 	sets the graphics matrix,
	// transform
	SET_TRANSFORM,			// <Transform*> 	sets the entity's transform.
	GET_TRANSFORM,			// <Transform*>		gets the entity's transform.
	// physics
	SET_VELOCITY,			// <glm::vec3*> 	sets the velocity.
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
};

#endif
