#include "message.h"

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	assert(receiver.type < sizeof(messageHandlers) / sizeof(0[messageHandlers]));
	messageHandlers[receiver.type](systemData, receiver, type, arg);
}

void sendMessage(SystemData& systemData, SystemTypes system, uint32_t type, void* arg) {
	assert(system < sizeof(messageHandlers) / sizeof(0[messageHandlers]));
	messageHandlers[system](systemData, INVALID_HNDL, type, arg);
}
