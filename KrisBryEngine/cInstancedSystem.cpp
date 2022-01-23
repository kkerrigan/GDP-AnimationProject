/**
 * @file cInstancedSystem.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cInstancedSystem class.
 */

#include <gameMath.h>

#include "cInstancedSystem.h"
#include "cEngine.h"
#include "cShaderManager.h"

cInstancedSystem::sGPUData cInstancedSystem::generateVBO(cInstancedMeshComponent::sDrawInfo drawInfo) {

	this->renderData.numInstances = drawInfo.numInstances;

	glm::mat4* matrices = new glm::mat4[this->renderData.numInstances];
	float* colors = new float[this->renderData.numInstances * 4];
	for (size_t index = 0; index < this->renderData.numInstances; ++index) {

		glm::mat4 matModel(1.0f);
		glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), drawInfo.instances[index]->position);
		matModel *= matTrans;
		matrices[index] = matModel;

		// Set the colors
		glm::vec4 color = drawInfo.instances[index]->color;
		colors[index * 4] = color.r;
		colors[index * 4 + 1] = color.g;
		colors[index * 4 + 2] = color.b;
		colors[index * 4 + 3] = color.a;
	}

	// Check if the the buffers have been created or not
	if (this->renderData.modelMatVBO == 0) {
		
		glGenBuffers(1, &this->renderData.modelMatVBO);
	}

	if (this->renderData.cubeVBO == 0) {

		glGenBuffers(1, &this->renderData.cubeVBO);
	}

	if (this->renderData.colorVBO == 0) {

		glGenBuffers(1, &this->renderData.colorVBO);
	}


	glBindBuffer(GL_ARRAY_BUFFER, this->renderData.modelMatVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->renderData.numInstances, &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, this->renderData.colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->renderData.numInstances * 4, &colors[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// done with the matrices and colors array, clean up
	delete[] matrices;
	delete[] colors;

	const float g_vertexBufferData[108] = {
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
	};
	this->renderData.numVertices = 108;

	glGenVertexArrays(1, &this->renderData.cubeVAO);
	glBindVertexArray(this->renderData.cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->renderData.cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexBufferData), g_vertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// also set instance data
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->renderData.modelMatVBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)sizeof(glm::vec4));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
	
	glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
	glVertexAttribDivisor(2, 1); 
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	// Now set the color attribule
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, this->renderData.colorVBO);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribDivisor(5, 1); // tell OpenGL this is an instanced vertex attribute.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return this->renderData;
}

void cInstancedSystem::drawInstances(sGPUData renderData) {

	cShaderManager::cShaderProgram* program = cShaderManager::getInstance()->pGetShaderProgramFromFriendlyName("InstanceShader");

	glUseProgram(program->ID);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(renderData.cubeVBO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, renderData.numVertices, renderData.numInstances);
	glBindVertexArray(0);

	return;
}
