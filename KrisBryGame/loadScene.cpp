#include <Global.h>
#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cSceneManager.h>
#include <cLightManager.h>
#include <cSoundManager.h>
#include <cLuaBrain.h>
#include <cScriptingCommandSystem.h>
#include <cColliderSystem.h>
#include <cSerialization.h>
#include <cMapLoader.h>
#include <cFrameBufferComponent.h>
#include <cRigidBodyComponent.h>
#include <cThreadComponent.h>
#include <cInstancedMeshComponent.h>
#include <fstream>

#include <cBehaviourSystem.h>
#include <cBehaviourComponent.h>
#include <cIdleBehaviour.h>
#include <cFormationBehaviour.h>
#include <cCrowdFollowBehaviour.h>
#include <cFlockBehaviour.h>

#include <cPathFinding.h>
#include <PathingStructs.h>
#include <cPathFindingBehaviour.h>
#include <cGathererBehaviour.h>

cEntity* pSphere = nullptr;
cEntity* pCube = nullptr;

sLight* pMovingSpotLight = nullptr;

extern std::map<std::string, cCommandGroup*> commandGroups;
extern std::map<std::string, iCommand*> commands;
extern cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;
cFormationBehaviour* pFormation;
cCrowdFollowBehaviour* pCrowdFollow;

cInstancedMeshComponent* instanceRenderer;

void set_rand_position(cTransformComponent* transform_component) {

	float x = rand() % 200 - 100.0f;
	float z = rand() % 200 - 100.0f;

	transform_component->setPosition(glm::vec3(x, 0.0f, z));
}

void loadScene() {

	// create a scene for the entities
	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pSandboxScene = pSceneManager->createScene("sandbox.json");
	pSceneManager->setActiveScene("sandbox.json");

	//initialize the light manager before loading them
	cLightManager::getInstance()->initializeLights();

	pSphere = cEntityBuilder::getInstance()->createEntity(0);
	cRenderMeshComponent* sphereMesh = pSphere->getComponent<cRenderMeshComponent>();
	sphereMesh->bIsVisible = false;
	sphereMesh->friendlyName = "Sphere";
	sphereMesh->bIsWireFrame = false;

	pCube = cEntityBuilder::getInstance()->createEntity(3);
	cRenderMeshComponent* cubeMesh = pCube->getComponent<cRenderMeshComponent>();
	cubeMesh->bIsVisible = false;
	cubeMesh->friendlyName = "Cube";
	cubeMesh->bIsWireFrame = true;

	cColliderSystem::getInstance()->pColliderSphere = pSphere;
	cColliderSystem::getInstance()->pAABBCube = pCube;

	//load from the file
	cSerialization::deserializeSceneCamera("cameras.json");
	cSerialization::deserializeSceneLights("lights.json");
	cSerialization::deserializeSceneSounds("sounds.json");
	cSerialization::deserializeSceneEntities("entities.json");

	cEntityBuilder* pBuilder = cEntityBuilder::getInstance();

	int debug = 0;
	
	return;
}