#include "message.h"

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	assert(receiver.type < sizeof(messageHandlers) / sizeof(0[messageHandlers]));
	messageHandlers[receiver.type](systemData, receiver, type, arg);
}
