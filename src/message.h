#ifndef MESSAGE_H
#define MESSAGE_H

#include "systems/systemData.h"

#include <iostream>

enum MessageType {
	// graphics
	SET_GRAPH_TRANS = 1,	// <Transform*> 	sets the graphics matrix,
	// transform
	SET_TRANSFORM,			// <Transform*> 	sets the entity's transform.
	GET_TRANSFORM,			// <Transform*>		gets the entity's transform.
	// physics
	SET_VELOCITY,			// <glm::vec3*> 	sets the velocity.
};

typedef void(*MessageHandler)(SystemData&, Handle, uint32_t, void*);

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

MessageHandler messageHandlers[] = {
	sendEntityMessage,
	sendTransformMessage,
	sendGraphicsMessage,
	sendPhysicsMessage,
	sendInputMessage,
};

#endif
