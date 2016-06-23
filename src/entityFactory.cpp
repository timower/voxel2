#include "entityFactory.h"
#include "resources.h"
#include "message.h"

GLuint indices[] = {
	// top
	0, 1, 2,
	2, 1, 3,
	// bottom
	4, 5, 6,
	6, 5, 7,
	// left
	8, 9, 10,
	10, 11, 8,
	// right
	12, 13, 14,
	13, 12, 15,
	// front
	16, 17, 18,
	18, 17, 19,
	// back
	20, 21, 22,
	22, 21, 23,
};

GLfloat vertices[] = {
	//top
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,
	//bottom
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f,
	//right
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f,
	//left
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f,
	//front
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,
	//back
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f,
};

static GLuint cubeIBO = 0;
static GLuint cubeVBO = 0;

// TODO: scale & use shared VBO
Handle createCube(SystemData& systemData) {
	Handle entity = createEntity(systemData.entityData);
	addTransformComponent(systemData, entity);

	Handle drawHndl = addDrawComponent(systemData, entity);
	GraphicsComponent& drawComp = getGraphicsComponent(systemData.graphicsData, drawHndl);

	if (!cubeIBO) {
		glGenBuffers(1, &cubeIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	if (!cubeVBO) {
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glGenVertexArrays(1, &drawComp.VAO);
	glBindVertexArray(drawComp.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	drawComp.VBO = cubeVBO;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	drawComp.IBO = cubeIBO;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenTextures(1, &drawComp.TEX);
	glBindTexture(GL_TEXTURE_2D, drawComp.TEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widht, height;
	loadTextureFile("assets/wood.jpg", GL_TEXTURE_2D, &widht, &height);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	drawComp.nVertices = 36;

	drawComp.program = loadShaderFile("assets/cube.vert", "assets/cube.frag");

	sendMessage(systemData, entity, SET_TRANSFORM, &ZERO);
	return entity;
}
