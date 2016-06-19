#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include <cstdint>

#define MAX_COMPONENTS 6
#define MAX_ENTITIES 128

struct Handle {
	uint32_t index : 16;
	uint32_t type : 6;
	uint32_t generation : 10;
};

struct Entity {
	uint8_t nComponents;
	Handle components[MAX_COMPONENTS];
};

struct EntityData {
	uint16_t nEntities;
	Entity entities[MAX_ENTITIES];
};

Handle createEntity(EntityData& entityData);
Entity& getEntity(EntityData& entityData, Handle entity);
void addComponent(EntityData& entityData, Handle entityHndl, Handle component);

#endif
