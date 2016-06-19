#include "message.h"

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	if (receiver.type == SystemTypes::ENTITY) {
		Entity& entity = getEntity(systemData.entityData, receiver);
		for (uint16_t i = 0; i < entity.nComponents; i++) {
			sendMessage(systemData, entity.components[i], type, arg);
		}
	} else {
		messageHandlers[receiver.type](systemData, receiver, type, arg);
	}
}
