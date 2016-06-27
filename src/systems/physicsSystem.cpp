#include "physicsSystem.h"
#include "../message.h"

#define GRAV_A 9.81f

PhysicsComponent& getPhysComponent(PhysicsData& pData, Handle component) {
	return pData.components.get(component);
}

Handle addPhysComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.physicsData.components.add();
	PhysicsComponent& comp = getPhysComponent(systemData.physicsData, ret);
	comp.entity = entityHndl;
	//comp.mass = 1.0f;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

void initPhysicsSystem(PhysicsData& physicsData) {
	physicsData.components.init();
}

void updatePhysicsSystem(SystemData& systemData, float dt) {
	size_t nComp = systemData.physicsData.components.size;
	for (size_t i = 0; i < nComp; i++) {
		PhysicsComponent& comp = systemData.physicsData.components.data[i];

		glm::vec3 velocity = comp.physVel + comp.velocity;
		comp.physVel.y -= GRAV_A * comp.mass * dt;

		Transform transform;
		sendEntitySysMsg(systemData, comp.entity, SystemTypes::TRANSFORM, GET_TRANSFORM, &transform);


		for (int axis = 0; axis < 3; axis++) {
			if (velocity[axis] == 0.0f)
				continue;
			glm::vec3 lowCorn = comp.aabb.lowerCorner;
			glm::vec3 highCorn = comp.aabb.higherCorner;

			if (velocity[axis] > 0)
				lowCorn[axis] = highCorn[axis];
			else
				highCorn[axis] = lowCorn[axis];

			glm::vec3 newPosition = transform.position;
			newPosition[axis] += velocity[axis] * dt;

			lowCorn += newPosition;
			highCorn += newPosition;

			glm::ivec3 lowerVox = glm::floor(lowCorn);
			glm::ivec3 higherVox = glm::floor(highCorn);

			for (int x = lowerVox.x; x <= higherVox.x; x++)
			for (int y = lowerVox.y; y <= higherVox.y; y++)
			for (int z = lowerVox.z; z <= higherVox.z; z++) {
				BlockInfo blockInfo;
				blockInfo.position = glm::vec3(x, y, z);
				sendChunkMessage(systemData, INVALID_HNDL, GET_BLOCK, &blockInfo);
				if (blockInfo.type != 0) {
					// TODO: better way?
					velocity[axis] = 0;
					comp.physVel[axis] = 0;
					goto loop_exit;
				}
			}
loop_exit: // TODO: fix goto.
		;
		}
		transform.position += velocity * dt;
		sendEntitySysMsg(systemData, comp.entity, SystemTypes::TRANSFORM, SET_TRANSFORM, &transform);
	}
}

void sendPhysicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_VELOCITY: {
			glm::vec3* velocity = static_cast<glm::vec3*>(arg);
			getPhysComponent(systemData.physicsData, receiver).velocity = *velocity;
		} break;
		case ADD_FORCE: {
			glm::vec3* force = static_cast<glm::vec3*>(arg);
			getPhysComponent(systemData.physicsData, receiver).physVel += *force;
		} break;
		case JUMP: {
			float* vel = static_cast<float*>(arg);
			PhysicsComponent& comp = getPhysComponent(systemData.physicsData, receiver);
			if (comp.physVel.y == 0.0f) {
				comp.physVel.y += *vel;
			}
		} break;
		case SET_MASS: {
			float* mass = static_cast<float*>(arg);
			getPhysComponent(systemData.physicsData, receiver).mass = *mass;
		} break;
		case SET_AABB: {
			AABB* aabb = static_cast<AABB*>(arg);
			getPhysComponent(systemData.physicsData, receiver).aabb = *aabb;
		} break;
		case DESTROY:  {
			systemData.physicsData.components.remove(receiver);
		} break;
	}
}
