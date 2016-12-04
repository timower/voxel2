#ifndef CHUNK_SYSTEM_H
#define CHUNK_SYSTEM_H

#include <cstdint>
#include <unordered_map>
#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "../container.h"

#include "systemTypes.h"

#define CHUNK_SIZE 16
#define ATLAS_SIZE 16
#define MAX_CHUNKS 128
#define N_VERTICES CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*4*4 // TODO: make smaller
#define N_INDICES  CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*4*4 // TODO: make smaller

#define LOAD_DIST 1
#define CHUNK_UNLOAD_TIME 60

struct Chunk {
	Handle handle;
	Handle entity;

	glm::ivec3 pos;

	Handle neighbours[6];

	uint8_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	bool dirty, active;
	//float lastBuildTime;
	float lastActiveTime;

	GLuint VBO;
	GLuint VAO;
};

struct VecHash {
	size_t operator()(const glm::ivec3 key) const {
		return key.x * 31 + key.y * 163 + key.z * 317;
	}
};

struct ChunkData {
	Container<Chunk, MAX_CHUNKS, SystemTypes::CHUNK> chunks;
	std::unordered_map<glm::ivec3, Handle, VecHash> chunkMap; // TODO: custom allocator?

	Handle player;

	// buffer for rebuilding chunk
	GLuint vertexBuffer[N_VERTICES];
	size_t vertexIdx;

	// TODO: remove:
	Handle chunksToRemove[64];
	size_t nChunksToRemove;

	// shared index buffer.
	//GLuint indexBuffer[N_INDICES];
	GLuint IBO;
	GLuint texture;
	GLuint program;
};

struct SystemData;
struct UpdateInfo;

void createChunkEntity(SystemData& systemData, glm::ivec3 chunkPos); // TODO: remove?

void initChunkSystem(ChunkData& chunkData);
void updateChunkSystem(SystemData& systemData, UpdateInfo& updateInfo);

void setPlayerHandle(ChunkData& chunkData, Handle player);

void sendChunkMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

struct RayInfo {
	glm::vec3 origin;
	glm::vec3 direction;
	bool adjacent;
	glm::ivec3 result;
};

struct BlockInfo {
	glm::ivec3 position;
	uint8_t type;
};

inline glm::ivec3 worldToChunk(glm::ivec3 world) {
	int tmp_x = world.x < 0;
    int tmp_y = world.y < 0;
    int tmp_z = world.z < 0;
    return glm::ivec3(
		(world.x + tmp_x) / CHUNK_SIZE -tmp_x,
		(world.y + tmp_y) / CHUNK_SIZE -tmp_y,
		(world.z + tmp_z) / CHUNK_SIZE -tmp_z
	);
}

inline glm::ivec3 worldToBlock(glm::ivec3 world) {
	return glm::ivec3(
		(world.x % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE,
		(world.y % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE,
		(world.z % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE
	);
}

#endif
