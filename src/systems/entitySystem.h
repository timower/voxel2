#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <cstdint>

#include "systemTypes.h"

#include "../container.h"
#include "../handle.h"

#define MAX_COMPONENTS 6
#define MAX_ENTITIES 128

struct Entity {
	Handle handle;

	uint8_t nComponents;
	Handle components[MAX_COMPONENTS];
};

struct EntityData {
	Container<Entity, MAX_ENTITIES, SystemTypes::ENTITY> entities;
};

Handle createEntity(EntityData& entityData);
Entity& getEntity(EntityData& entityData, Handle entity);
void addComponent(EntityData& entityData, Handle entityHndl, Handle component);

void initEntitySystem(EntityData& entityData);

struct SystemData;
void sendEntityMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
