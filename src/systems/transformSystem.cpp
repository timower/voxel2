#include "transformSystem.h"

#include "systemData.h"
#include "../message.h"

Handle addTransformComponent(SystemData& systemData, Handle entityHndl) {
	assert(systemData.transformData.nComponents < MAX_ENTITIES);

	uint16_t idx = systemData.transformData.nComponents++;
	systemData.transformData.components[idx] = {.entity = entityHndl, .dirty = true};

	Handle ret = {idx, SystemTypes::TRANSFORM, 0};
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

TransformComponent& getTransformComponent(TransformData& transformData, Handle component) {
	assert(component.type == SystemTypes::TRANSFORM);
	assert(component.index < transformData.nComponents);
	return transformData.components[component.index];
}

void updateTransformSystem(SystemData& systemData) {
	uint16_t n = systemData.transformData.nComponents;
	for (uint16_t i = 0; i < n; i++) {
		TransformComponent& comp = systemData.transformData.components[i];

		if (comp.dirty) {
			comp.dirty = false;
			sendMessage(systemData, comp.entity, SET_GRAPH_TRANS, &comp.transform);
		}
	}
}

void sendTransformMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_TRANSFORM: {
			Transform* trans = static_cast<Transform*>(arg);
			TransformComponent& comp = getTransformComponent(systemData.transformData, receiver);
			comp.transform = *trans;
			comp.dirty = true;
		} break;
		case GET_TRANSFORM: {
			Transform* trans = static_cast<Transform*>(arg);
			*trans = getTransformComponent(systemData.transformData, receiver).transform;
		} break;
	}
}
