#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <cstdarg>

#include <glm/glm.hpp>

#include "entitySystem.h"
#include "../container.h"

struct SystemData;

#define MAX_PHYS_COMP 128

struct AABB {
	glm::vec3 position;
	glm::vec3 size;
};

struct ColInfo {
	glm::ivec3 block;
	bool collision;
};

struct PhysicsComponent {
	Handle handle;
	Handle entity;

	glm::vec3 velocity;
	AABB aabb;

	glm::vec3 physVel;
	float mass;

};

struct PhysicsData {
	Container<PhysicsComponent, MAX_PHYS_COMP, SystemTypes::PHYSICS> components;
};

Handle addPhysComponent(SystemData& systemData, Handle entityHndl);

void initPhysicsSystem(PhysicsData& physicsData);

void updatePhysicsSystem(SystemData& systamData, float dt);

void sendPhysicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
