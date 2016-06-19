#include "physicsSystem.h"
#include "../message.h"

#define GRAV_A 9.81f

Handle addPhysComponent(SystemData& systemData, Handle entityHndl) {
	assert(systemData.physicsData.nComponents < MAX_PHYS_COMP);

	uint16_t idx = systemData.physicsData.nComponents++;
	systemData.physicsData.components[idx] = {.entity = entityHndl};

	Handle ret = {idx, SystemTypes::PHYSICS, 0};
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

PhysicsComponent& getPhysComponent(PhysicsData& pData, Handle component) {
	assert(component.type == SystemTypes::PHYSICS);
	assert(component.index < pData.nComponents);
	return pData.components[component.index];
}

void initPhysicsSystem(PhysicsData& physicsData) {

}

void updatePhysicsSystem(SystemData& systemData, float dt) {
	size_t nComp = systemData.physicsData.nComponents;
	for (size_t i = 0; i < nComp; i++) {
		PhysicsComponent& comp = systemData.physicsData.components[i];
		glm::vec3 velocity = comp.velocity + comp.physVel;

		Transform transform;
		sendMessage(systemData, comp.entity, GET_TRANSFORM, &transform);

		glm::vec3 newPosition = transform.position + velocity * dt;
		comp.physVel.y -= GRAV_A * dt;
		// TODO: collision detection.
		if (newPosition.y <= 0) {
			newPosition.y = 0;
			comp.physVel.y = 0;
		}
		transform.position = newPosition;

		sendMessage(systemData, comp.entity, SET_TRANSFORM, &transform);
	}
}

void sendPhysicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_VELOCITY: {
			glm::vec3* velocity = static_cast<glm::vec3*>(arg);
			getPhysComponent(systemData.physicsData, receiver).velocity = *velocity;
		} break;
	}
}
