#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "entitySystem.h"
#include "../transform.h"

struct SystemData;

struct TransformComponent {
	Handle entity;
	bool dirty; // TODO: remove
	Transform transform;
};

struct TransformData {
	uint16_t nComponents;
	TransformComponent components[MAX_ENTITIES];
};

Handle addTransformComponent(SystemData& systemData, Handle entityHndl);

void updateTransformSystem(SystemData& systemData);

void sendTransformMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
