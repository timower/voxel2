#include "chunkSystem.h"

#include "../message.h"
#include "../resources.h"

#include "systemData.h"

static GLuint indexBuffer[N_INDICES];

static Chunk& getChunkComponent(ChunkData& chunkData, Handle component) {
	return chunkData.chunks.get(component);
}

static Handle addChunkComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.chunkData.chunks.add();
	getChunkComponent(systemData.chunkData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

void createChunk(SystemData& systemData, glm::ivec3 chunkPos) {
	// create entity
	Handle entity = createEntity(systemData.entityData);
	// add transform
	addTransformComponent(systemData, entity);

	// add chunk comp.
	Handle chunkHndl = addChunkComponent(systemData, entity);
	// set terrain to full?
	Chunk& chunk = getChunkComponent(systemData.chunkData, chunkHndl);
	chunk.pos = chunkPos;
	for (uint16_t x = 0; x < CHUNK_SIZE; x++)
	for (uint16_t y = 0; y < CHUNK_SIZE; y++)
	for (uint16_t z = 0; z < CHUNK_SIZE; z++) {
			if (CHUNK_SIZE * chunkPos.y + y < 3.0f)
				chunk.blocks[x][y][z] = rand() % 10;
			else
				chunk.blocks[x][y][z] = 0;
	}
	chunk.dirty = true;
	// add graphics
	Handle drawable = addDrawComponent(systemData, entity);
	// set graphics VBO & IBO & TBO ...
	GraphicsComponent& drawComp = getGraphicsComponent(systemData.graphicsData, drawable);
	drawComp.IBO = systemData.chunkData.IBO; // is still unused.

	glGenVertexArrays(1, &drawComp.VAO);
	glBindVertexArray(drawComp.VAO);

	glGenBuffers(1, &drawComp.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, drawComp.VBO);

	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawComp.IBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	drawComp.nVertices = 0;

	drawComp.TEX = systemData.chunkData.texture;
	drawComp.program = systemData.chunkData.program;

	// send setPosition msg.
	Transform trans;
	trans.scale = glm::vec3(1, 1, 1);
	trans.pitch = trans.roll = trans.yaw = 0;
	trans.position = glm::vec3(chunkPos * CHUNK_SIZE);
	sendMessage(systemData, entity, SET_TRANSFORM, &trans);

	systemData.chunkData.chunkMap[chunkPos] = chunkHndl;
}

void initChunkSystem(ChunkData& chunkData) {
	// create IBO.
	chunkData.chunks.init();

	for (size_t i = 0, j = 0; i < N_INDICES / 6; i++, j += 4) {
		size_t idx = i * 6;
		indexBuffer[idx++] = j;
		indexBuffer[idx++] = j+1;
		indexBuffer[idx++] = j+2;

		indexBuffer[idx++] = j;
		indexBuffer[idx++] = j+2;
		indexBuffer[idx++] = j+3;
	}


	glGenBuffers(1, &chunkData.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkData.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), &indexBuffer[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// load texture:
	glGenTextures(1, &chunkData.texture);
	glBindTexture(GL_TEXTURE_2D, chunkData.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int widht, height;
	loadTextureFile("assets/block.png", GL_TEXTURE_2D, &widht, &height);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// load shader:
	chunkData.program = loadShaderFile("assets/chunk.vert", "assets/chunk.frag");

	chunkData.chunkMap.clear();
	chunkData.nChunksToRemove = 0;
}

static void addVertex(ChunkData& chunkData, uint8_t x, uint8_t y, uint8_t z, uint8_t matX, uint8_t matY) {
	assert(chunkData.vertexIdx < N_VERTICES);
	GLuint vertex = (x & 0x1F) | ((y & 0x1F) << 5) | ((z & 0x1F) << 10) | ((matX & 0x1F) << 15) | ((matY & 0x1F) << 20);
	chunkData.vertexBuffer[chunkData.vertexIdx++] = vertex;
}

static void rebuildChunk(SystemData& systemData, const Chunk& chunk) {
	ChunkData& chunkData = systemData.chunkData;
	chunkData.vertexIdx = 0;

	for (uint8_t x = 0; x < CHUNK_SIZE; x++)
	for (uint8_t y = 0; y < CHUNK_SIZE; y++)
	for (uint8_t z = 0; z < CHUNK_SIZE; z++) if (chunk.blocks[x][y][z] != 0) {
		uint8_t mat = chunk.blocks[x][y][z];
		uint8_t matX = mat % ATLAS_SIZE;
		uint8_t matY = mat / ATLAS_SIZE;

		// up
		if (y == CHUNK_SIZE - 1 || chunk.blocks[x][y + 1][z] == 0) {
			addVertex(chunkData, x,   y+1, z,   matX,   matY);
			addVertex(chunkData, x+1, y+1, z,   matX+1, matY);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x,   y+1, z+1, matX,   matY+1);
		}

		// down
		if (y == 0 || chunk.blocks[x][y - 1][z] == 0) {
			addVertex(chunkData, x,   y, z,   matX,   matY);
			addVertex(chunkData, x,   y, z+1, matX,   matY+1);
			addVertex(chunkData, x+1, y, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y, z,   matX+1, matY);
		}

		// left
		if (x == CHUNK_SIZE - 1 || chunk.blocks[x+1][y][z] == 0) {
			addVertex(chunkData, x+1, y,   z,   matX,   matY);
			addVertex(chunkData, x+1, y,   z+1, matX+1, matY);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y+1, z,   matX,   matY+1);
		}

		// right
		if (x == 0 || chunk.blocks[x-1][y][z] == 0) {
			addVertex(chunkData, x, y,   z,   matX,   matY);
			addVertex(chunkData, x, y+1, z,   matX,   matY+1);
			addVertex(chunkData, x, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x, y,   z+1, matX+1, matY);
		}

		// front
		if (z == CHUNK_SIZE - 1 || chunk.blocks[x][y][z+1] == 0) {
			addVertex(chunkData, x,   y,   z+1, matX,   matY);
			addVertex(chunkData, x,   y+1, z+1, matX,   matY+1);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y,   z+1, matX+1, matY);
		}

		// back
		if (z == 0 || chunk.blocks[x][y][z-1] == 0) {
			addVertex(chunkData, x,   y,   z, matX,   matY);
			addVertex(chunkData, x+1, y,   z, matX+1, matY);
			addVertex(chunkData, x+1, y+1, z, matX+1, matY+1);
			addVertex(chunkData, x,   y+1, z, matX,   matY+1);
		}
	}

	// get graphics component
	GLuint VBO;
	sendEntitySysMsg(systemData, chunk.entity, SystemTypes::GRAPHICS, GET_VBO, &VBO);
	// set buffer data:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, chunkData.vertexIdx * sizeof(GLuint), chunkData.vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// set nverteices:
	size_t nElements = chunkData.vertexIdx / 4 * 6;
	sendEntitySysMsg(systemData, chunk.entity, SystemTypes::GRAPHICS, SET_NVERTICES, &nElements);
}

void updateChunkSystem(SystemData& systemData) {
	// load chunks:
	Transform playerTrans;
	sendEntitySysMsg(systemData, systemData.chunkData.player, SystemTypes::TRANSFORM, GET_TRANSFORM, &playerTrans);
	glm::ivec3 playerChunk = worldToChunk(playerTrans.position);
	for (int x = playerChunk.x - LOAD_DIST; x <= playerChunk.x + LOAD_DIST; x++)
	for (int y = playerChunk.y - LOAD_DIST; y <= playerChunk.y + LOAD_DIST; y++)
	for (int z = playerChunk.z - LOAD_DIST; z <= playerChunk.z + LOAD_DIST; z++) {
		glm::ivec3 cPos = glm::ivec3(x, y, z);
		if (systemData.chunkData.chunkMap.count(cPos) == 0) {
			createChunk(systemData, cPos);
		}
	}

	// for each chunk: rebuild if dirty.
	ChunkData& chunkData = systemData.chunkData;
	size_t nChunks = chunkData.chunks.size;
	for (size_t i = 0; i < nChunks; i++) {
		Chunk& chunk = chunkData.chunks.data[i];

		int playerDist = glm::max(
			glm::abs(chunk.pos.x - playerChunk.x),
			glm::max(
				glm::abs(chunk.pos.y - playerChunk.y),
				glm::abs(chunk.pos.z - playerChunk.z)));
		if (playerDist > LOAD_DIST) {
			// TODO: use async messages or something?
			chunkData.chunksToRemove[chunkData.nChunksToRemove++] = chunk.entity;
		} else if (chunk.dirty) {
			rebuildChunk(systemData, chunk);
			chunk.dirty = false;
		}
	}

	for (size_t i = 0; i < chunkData.nChunksToRemove; i++) {
		sendMessage(systemData, chunkData.chunksToRemove[i], DESTROY, nullptr);
	}
	chunkData.nChunksToRemove = 0;
}


void setPlayerHandle(ChunkData& chunkData, Handle player) {
	chunkData.player = player;
}

void sendChunkMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case DESTROY: {
			glm::ivec3 pos = getChunkComponent(systemData.chunkData, receiver).pos;
			systemData.chunkData.chunkMap.erase(pos);
			systemData.chunkData.chunks.remove(receiver);
		} break;
	}
}
