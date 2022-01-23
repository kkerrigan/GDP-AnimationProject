/**
 * @file cInstancedMeshComponent.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cInstancedMeshComponent class.
 */

#include "cInstancedMeshComponent.h"

cInstancedMeshComponent::cInstancedMeshComponent() : cComponent() {


	return;
}

cInstancedMeshComponent::~cInstancedMeshComponent() {

	return;
}

/**
* generateDrawInfo
* @brief This function creates the draw info groups for the different map tile types.
* @return A map<string, sDrawInfo> that represents the number and positions of each tile type.
*/
#define MAX_DRAW_INFO_SIZE 1000
std::vector<cInstancedMeshComponent::sDrawInfo> cInstancedMeshComponent::generateDrawInfo() {
	
	// Setup each drawIfo
	size_t drawInfoID = 0;
	size_t maxSize = this->instances.size() < MAX_DRAW_INFO_SIZE ? this->instances.size() : MAX_DRAW_INFO_SIZE;
	size_t numGroups = this->instances.size() / maxSize + 1;
	size_t processedGroups = 0;
	std::vector<sDrawInfo> groups;

	for (size_t index = 0; index < numGroups; ++index) {

		sDrawInfo currGroup;

		// Process each instance in the group
		size_t end = processedGroups + maxSize;
		for (size_t j = processedGroups; j < end; ++j) {

			currGroup.instances.push_back(&this->instances[j]);
			currGroup.numInstances++;
			++processedGroups;

			if (processedGroups == this->instances.size()) break;
		}

		groups.push_back(currGroup);

		if (processedGroups == this->instances.size()) return groups;
	}

	return groups;
}

/**
* updateInstances
* @brief This function updates the instances of the component.
* @param vector<sPathFindingNode*> nodes - represents the nodes for the path finding.
* @param bool renderTraversable - represents whether or not to display the empty spaces (Optional)
* @return void.
*/
void cInstancedMeshComponent::updateInstances(std::vector<sPathFindingNode*> nodes, bool renderTraversable) {

	instances.clear();

	for (size_t i = 0; i < nodes.size(); ++i) {
		
		sMesh newMesh;
		newMesh.position = nodes[i]->position;
		newMesh.position.y -= 0.98f;
		
		if (!nodes[i]->bIsTraversable) {
			
			newMesh.position.y += 2.0f; // Make the walls of the maze slightly raised
			newMesh.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsResource) {
			
			newMesh.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsGatherer) {
			
			newMesh.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsHomeBase) {
			
			newMesh.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (renderTraversable && nodes[i]->bIsTraversable) {
			
			newMesh.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			this->instances.push_back(newMesh);
		}
	}

	return;
}
