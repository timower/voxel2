#ifndef CHUNK_SYSTEM_H
#define CHUNK_SYSTEM_H

#include <cstdint>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../container.h"

#include "systemTypes.h"

#define CHUNK_SIZE 16
#define ATLAS_SIZE 16
#define MAX_CHUNKS 128
#define N_VERTICES CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*4*4 // TODO: make smaller
#define N_INDICES  CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*4*4 // TODO: make smaller

struct Chunk {
	Handle handle;
	Handle entity;

	uint8_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	bool dirty;
};

struct ChunkData {
	Container<Chunk, MAX_CHUNKS, SystemTypes::CHUNK> chunks;

	// buffer for rebuilding chunk
	GLuint vertexBuffer[N_VERTICES];
	size_t vertexIdx;

	// shared index buffer.
	//GLuint indexBuffer[N_INDICES];
	GLuint IBO;
	GLuint texture;
	GLuint program;
};

struct SystemData;

void createChunk(SystemData& systemData, glm::ivec3 chunkPos); // TODO: remove?

void initChunkSystem(ChunkData& chunkData);
void updateChunkSystem(SystemData& systemData);

void sendChunkMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);

#endif
