#include "entitySystem.h"
#include "systemData.h"
#include <cassert>

Handle createEntity(EntityData& entityData) {
	assert(entityData.nEntities < MAX_ENTITIES);
	uint16_t idx = entityData.nEntities++;
	entityData.entities[idx] = {0};
	return {idx, SystemTypes::ENTITY, 0};
}

Entity& getEntity(EntityData& entityData, Handle entity) {
	assert(entity.type == SystemTypes::ENTITY);
	assert(entity.index < entityData.nEntities);
	return entityData.entities[entity.index];
}

void addComponent(EntityData& entityData, Handle entityHndl, Handle component) {
	Entity& entity = getEntity(entityData, entityHndl);
	assert(entity.nComponents < MAX_COMPONENTS);
	entity.components[entity.nComponents++] = component;
}
