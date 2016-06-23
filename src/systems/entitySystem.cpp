#include "entitySystem.h"
#include "systemData.h"
#include <cassert>

#include "../message.h"

Handle createEntity(EntityData& entityData) {
	return entityData.entities.add();
}

Entity& getEntity(EntityData& entityData, Handle entity) {
	return entityData.entities.get(entity);
}

void addComponent(EntityData& entityData, Handle entityHndl, Handle component) {
	Entity& entity = getEntity(entityData, entityHndl);
	assert(entity.nComponents < MAX_COMPONENTS);
	entity.components[entity.nComponents++] = component;
}

void initEntitySystem(EntityData& entityData) {
	entityData.entities.init();
}

void removeEntity(EntityData& entityData, Handle entity) {
	entityData.entities.remove(entity);
}

void sendEntityMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	Entity& entity = getEntity(systemData.entityData, receiver);

	for (uint16_t i = 0; i < entity.nComponents; i++) {
		sendMessage(systemData, entity.components[i], type, arg);
	}

	if (type == REMOVE_COMP)
		removeEntity(systemData.entityData, receiver);
}
