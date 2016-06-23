#include "transformSystem.h"

#include "systemData.h"
#include "../message.h"

TransformComponent& getTransformComponent(TransformData& transformData, Handle component) {
	return transformData.components.get(component);
}

Handle addTransformComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.transformData.components.add();

	getTransformComponent(systemData.transformData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);

	return ret;
}


void initTransformSystem(TransformData& transformData) {
	transformData.components.init();
}

void updateTransformSystem(SystemData& systemData) {
	size_t n = systemData.transformData.components.size;
	for (size_t i = 0; i < n; i++) {
		TransformComponent& comp = systemData.transformData.components.data[i];

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
