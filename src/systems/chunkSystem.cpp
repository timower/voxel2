#include "chunkSystem.h"

#include "../message.h"
#include "../resources.h"

#include "systemData.h"

static GLuint indexBuffer[N_INDICES];

static Chunk& getChunkComponent(ChunkData& chunkData, Handle component) {
	return chunkData.chunks.get(component);
}

static Handle getChunk(ChunkData& chunkData, glm::ivec3 pos) {
	if (chunkData.chunkMap.count(pos) == 0)
		return {0, SystemTypes::INVALID, 0};
	else
		return chunkData.chunkMap.at(pos);
}

static uint8_t getBlock(ChunkData& chunkData, glm::ivec3 pos) {
	glm::ivec3 chunkPos = worldToChunk(pos);
	glm::ivec3 blockPos = worldToBlock(pos);
	Handle chunk = getChunk(chunkData, chunkPos);
	if (chunk.type == SystemTypes::INVALID)
		return 0; // TODO: non loaded chunks?
	return getChunkComponent(chunkData, chunk).blocks[blockPos.x][blockPos.y][blockPos.z];
}


static void setBlock(ChunkData& chunkData, glm::ivec3 pos, uint8_t val) {
	glm::ivec3 chunkPos = worldToChunk(pos);
	glm::ivec3 blockPos = worldToBlock(pos);
	Handle chunk = getChunk(chunkData, chunkPos);
	if (chunk.type == SystemTypes::INVALID)
		return; // TODO: non loaded chunks?
	Chunk& comp = getChunkComponent(chunkData, chunk);
	comp.blocks[blockPos.x][blockPos.y][blockPos.z] = val;
	comp.dirty = true;

	if (blockPos.x == 0 && comp.neighbours[NRIGHT].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NRIGHT]).dirty = true;
	else if (blockPos.x == CHUNK_SIZE-1 && comp.neighbours[NLEFT].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NLEFT]).dirty = true;

	if (blockPos.y == 0 && comp.neighbours[NDOWN].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NDOWN]).dirty = true;
	else if (blockPos.y == CHUNK_SIZE-1 && comp.neighbours[NUP].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NUP]).dirty = true;

	if (blockPos.z == 0 && comp.neighbours[NBACK].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NBACK]).dirty = true;
	else if (blockPos.z == CHUNK_SIZE-1 && comp.neighbours[NFRONT].type != INVALID)
		getChunkComponent(chunkData, comp.neighbours[NFRONT]).dirty = true;
}


static Handle addChunkComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.chunkData.chunks.add();
	getChunkComponent(systemData.chunkData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

void createChunkEntity(SystemData& systemData, glm::ivec3 chunkPos) {
	// create entity
	Handle entity = createEntity(systemData.entityData);
	addTransformComponent(systemData, entity);

	// add chunk comp.
	Handle chunkHndl = addChunkComponent(systemData, entity);
	// set terrain to full?
	Chunk& chunk = getChunkComponent(systemData.chunkData, chunkHndl);
	chunk.pos = chunkPos;
	for (uint16_t x = 0; x < CHUNK_SIZE; x++)
	for (uint16_t y = 0; y < CHUNK_SIZE; y++)
	for (uint16_t z = 0; z < CHUNK_SIZE; z++) {
			if (CHUNK_SIZE * chunkPos.y + y < 3.0f) {
				if (x == 0 || x == CHUNK_SIZE -1 || z == 0 || z == CHUNK_SIZE - 1)
					chunk.blocks[x][y][z] = 1;
				else
					chunk.blocks[x][y][z] = 2;
			} else {
				chunk.blocks[x][y][z] = 0;
			}
	}
	chunk.dirty = true;
	// fill neighbours:
	for (int i = 0; i < 6; i++) {
		Handle neighbour = getChunk(systemData.chunkData, chunkPos + normals[i]);
		chunk.neighbours[i] = neighbour;
		if (neighbour.type != INVALID) {
			Chunk& neighChunk = getChunkComponent(systemData.chunkData, neighbour);
			neighChunk.neighbours[5 - i] = chunkHndl;
			neighChunk.dirty = true;
		}
	}
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

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

static uint8_t getChunkBlock(ChunkData& chunkData, const Chunk& chunk, int8_t x, int8_t y, int8_t z) {
	if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)
		return chunk.blocks[x][y][z];
	else if (x < 0 && chunk.neighbours[NRIGHT].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NRIGHT]), x + CHUNK_SIZE, y, z);
	else if (x > CHUNK_SIZE-1 && chunk.neighbours[NLEFT].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NLEFT]), x - CHUNK_SIZE, y, z);
	else if (y < 0 && chunk.neighbours[NDOWN].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NDOWN]), x, y + CHUNK_SIZE, z);
	else if (y > CHUNK_SIZE-1 && chunk.neighbours[NUP].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NUP]), x, y - CHUNK_SIZE, z);
	else if (z < 0 && chunk.neighbours[NBACK].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NBACK]), x, y, z + CHUNK_SIZE);
	else if (z > CHUNK_SIZE-1 && chunk.neighbours[NFRONT].type != INVALID)
		return getChunkBlock(chunkData, getChunkComponent(chunkData, chunk.neighbours[NFRONT]), x, y, z - CHUNK_SIZE);
	return 0;
}

static void rebuildChunk(SystemData& systemData, const Chunk& chunk) {
	ChunkData& chunkData = systemData.chunkData;
	chunkData.vertexIdx = 0;

	for (int8_t x = 0; x < CHUNK_SIZE; x++)
	for (int8_t y = 0; y < CHUNK_SIZE; y++)
	for (int8_t z = 0; z < CHUNK_SIZE; z++) if (chunk.blocks[x][y][z] != 0) {
		uint8_t mat = chunk.blocks[x][y][z];
		uint8_t matX = mat % ATLAS_SIZE;
		uint8_t matY = mat / ATLAS_SIZE;

		// up
		if (getChunkBlock(chunkData, chunk, x, y+1, z) == 0) {
			addVertex(chunkData, x,   y+1, z,   matX,   matY);
			addVertex(chunkData, x+1, y+1, z,   matX+1, matY);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x,   y+1, z+1, matX,   matY+1);
		}

		// down
		if (getChunkBlock(chunkData, chunk, x, y - 1, z) == 0) {
			addVertex(chunkData, x,   y, z,   matX,   matY);
			addVertex(chunkData, x,   y, z+1, matX,   matY+1);
			addVertex(chunkData, x+1, y, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y, z,   matX+1, matY);
		}

		// left
		if (getChunkBlock(chunkData, chunk, x + 1, y, z) == 0) {
			addVertex(chunkData, x+1, y,   z,   matX,   matY);
			addVertex(chunkData, x+1, y,   z+1, matX+1, matY);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y+1, z,   matX,   matY+1);
		}

		// right
		if (getChunkBlock(chunkData, chunk, x - 1, y, z) == 0) {
			addVertex(chunkData, x, y,   z,   matX,   matY);
			addVertex(chunkData, x, y+1, z,   matX,   matY+1);
			addVertex(chunkData, x, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x, y,   z+1, matX+1, matY);
		}

		// front
		if (getChunkBlock(chunkData, chunk, x, y, z + 1) == 0) {
			addVertex(chunkData, x,   y,   z+1, matX,   matY);
			addVertex(chunkData, x,   y+1, z+1, matX,   matY+1);
			addVertex(chunkData, x+1, y+1, z+1, matX+1, matY+1);
			addVertex(chunkData, x+1, y,   z+1, matX+1, matY);
		}

		// back
		if (getChunkBlock(chunkData, chunk, x, y, z - 1) == 0) {
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
			createChunkEntity(systemData, cPos);
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

// TODO: optimize to use chunk neighbours -> one hash lookup!
static void castRay(ChunkData& chunkData, const RayInfo& rayInfo) {
	int X = static_cast<int>(floor(rayInfo.origin.x));
	int Y = static_cast<int>(floor(rayInfo.origin.y));
	int Z = static_cast<int>(floor(rayInfo.origin.z));
	int lastX = X;
	int lastY = Y;
	int lastZ = Z;

	int stepX = glm::sign(rayInfo.direction.x);
	int stepY = glm::sign(rayInfo.direction.y);
	int stepZ = glm::sign(rayInfo.direction.z);

	float tMaxX = ((float)X + (float)(rayInfo.direction.x > 0) - rayInfo.origin.x ) / rayInfo.direction.x;
	float tMaxY = ((float)Y + (float)(rayInfo.direction.y > 0) - rayInfo.origin.y ) / rayInfo.direction.y;
	float tMaxZ = ((float)Z + (float)(rayInfo.direction.z > 0) - rayInfo.origin.z ) / rayInfo.direction.z;

	float tDeltaX = static_cast<float>(stepX)/rayInfo.direction.x;
	float tDeltaY = static_cast<float>(stepY)/rayInfo.direction.y;
	float tDeltaZ = static_cast<float>(stepZ)/rayInfo.direction.z;

	int k = 0;
	while (k < 8) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				X += stepX;
				tMaxX += tDeltaX;
			} else {
				Z += stepZ;
				tMaxZ += tDeltaZ;
			}
		} else {
			if (tMaxY < tMaxZ) {
				Y += stepY;
				tMaxY += tDeltaY;
			} else {
				Z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		if (getBlock(chunkData, glm::ivec3(X, Y, Z)) != 0) {
			if (!rayInfo.add)
				setBlock(chunkData, glm::ivec3(X, Y, Z), rayInfo.val);
			else if (k != 0)
				setBlock(chunkData, glm::ivec3(lastX, lastY, lastZ), rayInfo.val);
			break;
		}
		lastX = X;
		lastY = Y;
		lastZ = Z;
		k++;
	}
}

static void destroyChunkComponent(ChunkData& chunkData, Handle chunkComp) {
	Chunk& chunk = getChunkComponent(chunkData, chunkComp);
	for (int i = 0; i < 6; i++) {
		Handle neighbour = chunk.neighbours[i];
		if (neighbour.type != INVALID) {
			Chunk& neighChunk = getChunkComponent(chunkData, neighbour);
			neighChunk.neighbours[5 - i].type = INVALID;
		}
	}
	chunkData.chunkMap.erase(chunk.pos);
	chunkData.chunks.remove(chunkComp);
	// TODO: save to disk..
}

void sendChunkMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case DESTROY: {
			destroyChunkComponent(systemData.chunkData, receiver);
		} break;
		case CAST_RAY: {
			RayInfo* rayInfo = static_cast<RayInfo*>(arg);
			castRay(systemData.chunkData, *rayInfo);
		} break;
		case GET_BLOCK: {
			BlockInfo* blockInfo = static_cast<BlockInfo*>(arg);
			blockInfo->type = getBlock(systemData.chunkData, blockInfo->position);
		} break;
	}
}
