#include "graphicsSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../platform.h"
#include "../message.h"

Handle addDrawComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.graphicsData.drawables.add();
	getGraphicsComponent(systemData.graphicsData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

GraphicsComponent& getGraphicsComponent(GraphicsData& graphicsData, Handle component) {
	return graphicsData.drawables.get(component);
}

Handle addCameraComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = {0, SystemTypes::CAMERA, 0};
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

void initGraphicsSystem(GraphicsData& graphicsData) {
	graphicsData.projectionMatrix = glm::perspective(
		glm::radians(80.0f),
		(float) WIDTH / (float)HEIGHT,
		0.01f,
		100.0f
	);
	graphicsData.drawables.init();
}

struct Plane {
	glm::vec3 normal;
	float d;
};

struct Frustum {
	Plane planes[6];
};

float sdistanceToPoint(const Plane& plane, const glm::vec3 &pos) {
    return glm::dot(plane.normal, pos) + plane.d;
}

static bool inFrustum(const Frustum& frustum, glm::vec3 pos, glm::vec3 size) {
	for (int i = 0; i < 6; i++) {
        int out = 0;

        glm::vec3 test = pos;
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(size.x, 0, 0);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(0, size.y, 0);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(size.x, size.y, 0);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(0, 0, size.z);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(size.x, 0, size.z);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + glm::vec3(0, size.y, size.z);
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        test = pos + size;
        if (sdistanceToPoint(frustum.planes[i], test) < 0.0f) {
            out++;
        }

        if (out == 8) {
            // all points are outside:
            return false;
        }
    }
    // we reach here -> only intersections and outsiders
    return true;
}

void calcFrustum(Frustum& frustum, glm::mat4& pv) {
    frustum.planes[NUP].normal.x = pv[0][3] - pv[0][1];
    frustum.planes[NUP].normal.y = pv[1][3] - pv[1][1];
    frustum.planes[NUP].normal.z = pv[2][3] - pv[2][1];
    frustum.planes[NUP].d        = pv[3][3] - pv[3][1];

    frustum.planes[NDOWN].normal.x = pv[0][3] + pv[0][1];
    frustum.planes[NDOWN].normal.y = pv[1][3] + pv[1][1];
    frustum.planes[NDOWN].normal.z = pv[2][3] + pv[2][1];
    frustum.planes[NDOWN].d        = pv[3][3] + pv[3][1];

    frustum.planes[NLEFT].normal.x = pv[0][3] + pv[0][0];
    frustum.planes[NLEFT].normal.y = pv[1][3] + pv[1][0];
    frustum.planes[NLEFT].normal.z = pv[2][3] + pv[2][0];
    frustum.planes[NLEFT].d        = pv[3][3] + pv[3][0];

    frustum.planes[NRIGHT].normal.x = pv[0][3] - pv[0][0];
    frustum.planes[NRIGHT].normal.y = pv[1][3] - pv[1][0];
    frustum.planes[NRIGHT].normal.z = pv[2][3] - pv[2][0];
    frustum.planes[NRIGHT].d        = pv[3][3] - pv[3][0];

    frustum.planes[NFRONT].normal.x = pv[0][3] + pv[0][2];
    frustum.planes[NFRONT].normal.y = pv[1][3] + pv[1][2];
    frustum.planes[NFRONT].normal.z = pv[2][3] + pv[2][2];
    frustum.planes[NFRONT].d        = pv[3][3] + pv[3][2];

    frustum.planes[NBACK].normal.x = pv[0][3] - pv[0][2];
    frustum.planes[NBACK].normal.y = pv[1][3] - pv[1][2];
    frustum.planes[NBACK].normal.z = pv[2][3] - pv[2][2];
    frustum.planes[NBACK].d        = pv[3][3] - pv[3][2];

    for (int i = 0; i < 6; i++) {
        float length = glm::length(frustum.planes[i].normal);
        frustum.planes[i].normal /= length;
        frustum.planes[i].d /= length;
    }
}

void updateGraphicsSystem(GraphicsData& gData) {

	// TODO: sort drawables.

	glClearColor(0.2, 0.5, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 pv = gData.projectionMatrix * gData.viewMatrix;

	Frustum frustum;
	calcFrustum(frustum, pv);

	GLint currentProgram = 0;
	GLint mvpLoc = 0;

	GLuint currentTex = 0;
	GLuint currentVAO = 0;

	size_t nDrawables = gData.drawables.size;
	for (size_t i = 0; i < nDrawables; ++i) {
		const GraphicsComponent& drawable = gData.drawables.data[i];
		if (!inFrustum(frustum, drawable.pos, drawable.size))
			continue;

		if (currentProgram != drawable.program) {
			glUseProgram(drawable.program);
			mvpLoc = glGetUniformLocation(drawable.program, "MVP");
		}

		if (currentTex != drawable.TEX) {
			glBindTexture(GL_TEXTURE_2D, drawable.TEX);
			currentTex = drawable.TEX;
		}

		if (currentVAO != drawable.VAO) {
			glBindVertexArray(drawable.VAO);
			currentVAO = drawable.VAO;
		}

		glm::mat4 pvm = pv * drawable.modelMat;
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(pvm));

		glDrawElements(GL_TRIANGLES, drawable.nVertices, GL_UNSIGNED_INT, (GLvoid*)0);
		//glBindVertexArray(0);
	}
}

void sendGraphicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_GRAPH_TRANS: {
			Transform* transform = static_cast<Transform*>(arg);
			if (receiver.type == SystemTypes::CAMERA) {
				systemData.graphicsData.viewMatrix = transformToView(*transform);
			} else {
				GraphicsComponent& comp = getGraphicsComponent(systemData.graphicsData, receiver);
				comp.modelMat = transformToModel(*transform);
				comp.pos = transform->position;
			}
		} break;
		case GET_VBO: {
			GLuint* VBO = static_cast<GLuint*>(arg);
			*VBO = getGraphicsComponent(systemData.graphicsData, receiver).VBO;
		} break;
		case SET_NVERTICES: {
			size_t* nverts = static_cast<size_t*>(arg);
			getGraphicsComponent(systemData.graphicsData, receiver).nVertices = *nverts;
		} break;
		case SET_BOUNDING_SIZE: {
			glm::vec3* size = static_cast<glm::vec3*>(arg);
			getGraphicsComponent(systemData.graphicsData, receiver).size = *size;
		} break;
		case DESTROY: {
			systemData.graphicsData.drawables.remove(receiver);
		} break;
	}
}
