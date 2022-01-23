/**
 * @file cInstancedSystem.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * The class is responsible for drawing objects using instanced drawing.
 */

#ifndef _cInstancedSystem_HG_
#define _cInstancedSystem_HG_

#include <gameMath.h>
#include <vector>

#include "Global.h"
#include "cInstancedMeshComponent.h"

class cInstancedSystem {
public:
	struct sGPUData {
		sGPUData() :
			cubeVAO(0),
			cubeVBO(0),
			numVertices(0),
			modelMatVBO(0),
			colorVBO(0),
			numInstances(0)
		{}

		GLuint cubeVAO;
		GLuint cubeVBO;
		GLuint numVertices;

		GLuint modelMatVBO;
		GLuint colorVBO;
		GLuint numInstances;
	};

	sGPUData renderData;

	sGPUData generateVBO(cInstancedMeshComponent::sDrawInfo drawInfo);
	void drawInstances(sGPUData renderData);
};

#endif // !_cInstancedSystem_HG_

