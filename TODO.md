- Make systems a class.
- Fix physics.
- Fix jump.
- Add extra indirection, allow entities to be removed.

- Store components per system in entity.
- Message specific component of entity.

- Basic chunk.

IDEA:

´´´
// systemData.cpp:
void initSystemData(data) {
	initSystem1();
	initSystem2();
	....
}
// game.cpp:
void init(data, config) {
    initSystemData(data)

    loadScene(data)
}


void update(data, update) {
	updateSystemData(data, update);
}

// message.cpp:
void sendMessage(data, Handle receiver, MessageType type, void* msg) {
    messageHandles[type.system](data, receiver, type, msg);
}
´´´
