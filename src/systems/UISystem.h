#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../container.h"

#include "systemTypes.h"

#define MAX_UI_ELEMENTS 64

struct UIElement {
	// location on screen.
	glm::vec2 position;
	glm::vec2 size;
	// location in texture.
	glm::vec2 texLow;
	glm::vec2 texHigh;
	// z layer. 0 - 100
	int priority;
};

struct UIData {
	GLuint VBO;
	GLuint TEX;
	GLint PROG;

	bool dirty;

	Container<UIElement, MAX_UI_ELEMENTS, SystemTypes::UI> elements;
};

void initUISystem(UIData& UIdata);
void updateUISystem(UIData& UIdata);

#endif
