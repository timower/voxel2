#include "message.h"

void sendMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	messageHandlers[receiver.type](systemData, receiver, type, arg);
}
