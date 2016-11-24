#include "UISystem.h"

void initUISystem(UIData& UIdata) {
	UIdata.elements.init();
	UIdata.dirty = false;
	// TODO: load VBO & stuff.
}

static void addVertex(float x, float y, float z, float texX, float texY) {

}

void updateUISystem(UIData& UIdata) {
	if (!UIdata.dirty)
		return;

	size_t n = UIdata.elements.size;
	for (size_t i = 0; i < n; i++) {
		UIElement& element = UIdata.elements.data[i];

		float x = element.position.x;
		float y = element.position.y;
		float xh = x + element.size.x;
		float yh = y + element.size.y;

		float z = -(float)element.priority / 100.0f;

		float tx = element.texLow.x;
		float ty = element.texLow.y;
		float txh = element.texHigh.x;
		float tyh = element.texHigh.y;
		// quad:
		addVertex(x,  y,  z, tx,  ty);
		addVertex(x,  yh, z, tx,  tyh);
		addVertex(xh, y,  z, txh, ty);

		addVertex(xh, yh, z, txh, tyh);
		addVertex(xh, y,  z, txh, ty);
		addVertex(x,  yh, z, tx,  tyh);
	}

	// update VBO:
}
