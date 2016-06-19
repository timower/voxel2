#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

struct SystemData;

#include "entitySystem.h"
#include "transformSystem.h"
#include "graphicsSystem.h"
#include "physicsSystem.h"
#include "inputSystem.h"

enum SystemTypes {
	ENTITY = 0,
	TRANSFORM,
	GRAPHICS,
	PHYSICS,
	INPUT
};

struct SystemData {
	EntityData entityData;
	TransformData transformData;
	GraphicsData graphicsData;
	PhysicsData physicsData;
	InputData inputData;
};

#endif
