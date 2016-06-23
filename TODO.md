- fix entity factory.

- Basic chunk.

- sub entities via entity component zero in entity or remove component zero?

IDEA:

```cpp
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
```
