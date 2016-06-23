#include "entitySystem.h"
#include "systemData.h"
#include <cassert>

#include "../message.h"

Entity& getEntity(EntityData& entityData, Handle entity) {
	return entityData.entities.get(entity);
}

Handle createEntity(EntityData& entityData) {
	Handle ret = entityData.entities.add();
	for (size_t i = 0; i < MAX_COMPONENTS; i++)
		getEntity(entityData, ret).components[i] = {0, SystemTypes::INVALID, 0};
	return ret;
}

void addComponent(EntityData& entityData, Handle entityHndl, Handle component) {
	Entity& entity = getEntity(entityData, entityHndl);
	//assert(entity.nComponents < MAX_COMPONENTS);
	assert(component.type < MAX_COMPONENTS);
	entity.components[component.type] = component;
}

void initEntitySystem(EntityData& entityData) {
	entityData.entities.init();
}

void removeEntity(EntityData& entityData, Handle entity) {
	entityData.entities.remove(entity);
}

void sendEntityMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	Entity& entity = getEntity(systemData.entityData, receiver);

	for (uint16_t i = 0; i < MAX_COMPONENTS; i++) {
		if (entity.components[i].type != SystemTypes::INVALID)
			sendMessage(systemData, entity.components[i], type, arg);
	}

	if (type == DESTROY)
		removeEntity(systemData.entityData, receiver);
}


void sendEntitySysMsg(SystemData& systemData, Handle receiver, uint32_t system, uint32_t type, void* arg) {
		Entity& entity = getEntity(systemData.entityData, receiver);
		//assert(entity.components[system].type != SystemTypes::INVALID);
		if (entity.components[system].type != SystemTypes::INVALID)
			sendMessage(systemData,	entity.components[system], type, arg);
}
