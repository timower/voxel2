#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <cstdarg>

#include <glm/glm.hpp>

#include "entitySystem.h"
#include "../container.h"

struct SystemData;

#define MAX_PHYS_COMP 128

struct PhysicsComponent {
	Handle handle;
	Handle entity;

	glm::vec3 velocity;
	glm::vec3 physVel;
};

struct PhysicsData {
	Container<PhysicsComponent, MAX_PHYS_COMP, SystemTypes::PHYSICS> components;
};

Handle addPhysComponent(SystemData& systemData, Handle entityHndl);
PhysicsComponent& getPhysComponent(PhysicsData& pData, Handle component);

void initPhysicsSystem(PhysicsData& physicsData);

void updatePhysicsSystem(SystemData& systamData, float dt);

void sendPhysicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
