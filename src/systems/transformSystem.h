#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "entitySystem.h"
#include "../transform.h"

struct SystemData;

struct TransformComponent {
	Handle handle;
	Handle entity;
	bool dirty; // TODO: remove
	Transform transform;
};

struct TransformData {
	Container<TransformComponent, MAX_ENTITIES, SystemTypes::TRANSFORM> components;
};

Handle addTransformComponent(SystemData& systemData, Handle entityHndl);

void initTransformSystem(TransformData& transformData);
void updateTransformSystem(SystemData& systemData);

void sendTransformMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
