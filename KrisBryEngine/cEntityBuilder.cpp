#include <iostream>

#include "cEntityBuilder.h"

#include "cEulerMovementSystem.h"
#include "cMeshRenderSystem.h"
#include "cColliderSystem.h"
#include "cBehaviourSystem.h"
#include "cFBOSystem.h"

#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cSkinnedMeshComponent.h"
#include "cCollider.h"
#include "cRigidBodyComponent.h"
#include "cFrameBufferComponent.h"
#include "cThreadComponent.h"
#include "cClothComponent.h"
#include "cInstancedMeshComponent.h"

#include "cBehaviourComponent.h"
#include "cFollowBehaviour.h"

#include "physicsShapes.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cSceneManager.h"
#include "c3dSoundComponent.h"
#include "cSoundManager.h"
#include "cGLColourHelper.h"

bool assimpToVAOConverter(cSkinnedMeshComponent* pTheAssimpSM);

cEntityBuilder::cEntityBuilder() {

	return;
}


cEntityBuilder::~cEntityBuilder(){
	/*for (cEntity* entity : this->allocatedEntites) {
		delete entity;
	}*/
}

cEntityBuilder* cEntityBuilder::getInstance() {
	static cEntityBuilder instance;

	return &instance;
}

void cEntityBuilder::setEntityPosition(cEntity* entity, glm::vec3 position) {

	entity->getComponent<cTransformComponent>()->setPosition(position);

	return;
}

cEntity* cEntityBuilder::createEntity(int entityType) {
	cEntity* result = nullptr;

	switch (entityType) {
	
		//allowed entity types
		case 0: // sphere
		case 1: // Sky box object
		case 2: // Flat plane
		case 3: // Cube
		case 4: // Rectangle
		case 5: // FBO 2-sided quad
		case 6: // Aqua Skinned Mesh
		case 7: // Shadow Heartless Skinned Mesh
		case 8: // Cloth
		case 9: // Cone shape
		case 10: // Capsule shape
		case 11: // Cylinder shape
		
			result = new cEntity(entityType);
			break;
	}

	if (result != nullptr) {
		// add the components required
		addRequiredComponents(result, entityType);

		this->allocatedEntites.push_back(result);
	}

	return result;
}

cEntity* cEntityBuilder::createEntity(int entityType, glm::vec3 position) {
	cEntity* result = nullptr;

	result = this->createEntity(entityType);

	this->setEntityPosition(result, position);

	return result;
}


cEntity* cEntityBuilder::createEntityRandomPosColor(int entityType) {
	cEntity* result = this->createEntity(entityType);

	cTransformComponent* pTransformComponent = result->getComponent<cTransformComponent>();
	pTransformComponent->setPosition(glm::vec3(this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f)));

	cRenderMeshComponent* pRenderMeshComponent = result->getComponent<cRenderMeshComponent>();
	pRenderMeshComponent->materialDiffuse = glm::vec4(this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>() , 1.0f);
	
	return result;
}

void cEntityBuilder::addRequiredComponents(cEntity* entity, int entityType) {

	cGLColourHelper* pColour = cGLColourHelper::getInstance();

	switch (entityType)
	{
	case 0:
	{
		entity->addComponent<cTransformComponent>();
		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
			meshComp->friendlyName = "Sphere";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->materialDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		entity->addComponent<cRigidBodyComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 1:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			pTransformComponent->scale = glm::vec3(5000.0f);
		}

		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
			meshComp->friendlyName = "SkyBoxObject";
			meshComp->bIsVisible = true;
			meshComp->materialDiffuse = glm::vec4(1.0f, 105.0f / 255.0f, 180.0f / 255.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 2:
	{
		entity->addComponent<cTransformComponent>();
		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "flat.ply";
			meshComp->friendlyName = "Plane";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		entity->addComponent<cRigidBodyComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);

	}
	break;

	case 3:
	{

		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "cube_flat_shaded_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->friendlyName = "Cube";
		}

		entity->addComponent<cInstancedMeshComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 4:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			pTransformComponent->scale = glm::vec3(1.0f);
		}

		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "rect1_xyz_n_uv.ply";
			meshComp->friendlyName = "rec1";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = true;
			meshComp->materialDiffuse = glm::vec4(1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 5:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			glm::vec3 pos = pTransformComponent->getPosition();
			pos.z = 0.0f;
			pTransformComponent->setPosition(pos);
			pTransformComponent->setOrientationEulerAngles(glm::vec3(90.0f, 0.0f, 90.0f), true);
			pTransformComponent->scale = glm::vec3(25.0f);
		}

		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
			meshComp->friendlyName = "2SidedQuad";
			meshComp->bIsVisible = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			meshComp->setAlphaTransparency(1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		entity->addComponent<cFrameBufferComponent>();

		// register the entity with the component systems
		cFBOSystem::getInstance()->registerEntity(entity);
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 6:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* transformComponent = entity->getComponent<cTransformComponent>();
			transformComponent->setUniformScale(1.0f);
		}

		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "Aqua";
			meshComp->friendlyName = "Aqua";
			meshComp->bIsVisible = true;
			meshComp->setDiffuseColour(pColour->getColourRGB(cGLColourHelper::LIGHT_BLUE));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = true;
			meshComp->bIsWireFrame = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f);

			/*{
				sTextureInfo texture;
				texture.name = "sand.bmp";
				texture.strength = 1.0f;

				meshComp->textures.push_back(texture);
			}*/

			// Register with the Mesh system
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}

		entity->addComponent<cSkinnedMeshComponent>();
		{
			cSkinnedMeshComponent* skinMeshComp = entity->getComponent<cSkinnedMeshComponent>();
			skinMeshComp->setBasePath("./assets/modelsFBX/");
			skinMeshComp->meshName = "Aqua.fbx"; // TODO: add base path

			if (!skinMeshComp->loadMeshFromFile()) {
				std::cout << "ERROR: Problem loading the skinned mesh!" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Idle", "Aqua_Idle.fbx")) {

				std::cout << "Aqua Idle animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Walk", "Aqua_Walking.fbx")) {

				std::cout << "Aqua Walk animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("WalkBack", "Aqua_Walking_Backwards.fbx")) {

				std::cout << "Aqua Backwards Walk animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Jump", "Aqua_Jump.fbx")) {

				std::cout << "Aqua Jump animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("RunningJump", "Aqua_Running_Jump.fbx")) {

				std::cout << "Aqua Running Jump animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Action", "Aqua_Hook_Punch.fbx")) {

				std::cout << "Aqua Hook Punch animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("StrafeRight", "Aqua_Walk_Strafe_Right.fbx")) {

				std::cout << "Aqua Strafe Right animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("StrafeLeft", "Aqua_Walk_Strafe_Left.fbx")) {

				std::cout << "Aqua Strafe Right animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Running", "Aqua_Running.fbx")) {

				std::cout << "Aqua Running animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("RunningBack", "Aqua_Running_Backward.fbx")) {

				std::cout << "Aqua Running Backwards animation loaded" << std::endl;
			}

			skinMeshComp->friendlyName = "Aqua";

			// NOTE: Not sure if next line is necessary 
			cMesh* pMesh = skinMeshComp->createMeshObjectFromCurrentModel();
			if (pMesh) {
				std::cout << "Aqua was loaded" << std::endl;
			}
			else {
				std::cout << "ERROR: Didn't load Aqua" << std::endl;
			}

			// Convert assimp model to VAO
			if (!assimpToVAOConverter(skinMeshComp)) {
				std::cout << "ERROR: Didn't copy Aqua to VAO Format" << std::endl;
			}
			else {

				skinMeshComp->pAniState = new cAnimationState();

				skinMeshComp->pAniState->defaultAnimation.name = "Idle";
			}
		}
	}
	break;

	case 7:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* transformComponent = entity->getComponent<cTransformComponent>();
			transformComponent->setUniformScale(1.0f);
		}

		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "Aqua";
			meshComp->friendlyName = "Aqua";
			meshComp->bIsVisible = true;
			meshComp->setDiffuseColour(pColour->getColourRGB(cGLColourHelper::LIGHT_BLUE));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = true;
			meshComp->bIsWireFrame = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f);

			/*{
				sTextureInfo texture;
				texture.name = "sand.bmp";
				texture.strength = 1.0f;

				meshComp->textures.push_back(texture);
			}*/

			// Register with the Mesh system
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}

		entity->addComponent<cSkinnedMeshComponent>();
		{
			cSkinnedMeshComponent* skinMeshComp = entity->getComponent<cSkinnedMeshComponent>();
			skinMeshComp->setBasePath("./assets/modelsFBX/");
			skinMeshComp->meshName = "ShadowHeartless.fbx"; // TODO: add base path

			if (!skinMeshComp->loadMeshFromFile()) {
				std::cout << "ERROR: Problem loading the skinned mesh!" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Idle", "ShadowHeartless_Idle.fbx")) {

				std::cout << "Shadow Idle animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Walk", "ShadowHeartless_Walking.fbx")) {

				std::cout << "Shadow Walk animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("WalkBack", "ShadowHeartless_Walk_Backwards.fbx")) {

				std::cout << "Shadow Backwards Walk animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Jump", "ShadowHeartless_Jump.fbx")) {

				std::cout << "Shadow Jump animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("RunningJump", "ShadowHeartless_Running_Jump.fbx")) {

				std::cout << "Shadow Running Jump animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Running", "ShadowHeartless_Running.fbx")) {

				std::cout << "Shadow Running animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("RunningBack", "ShadowHeartless_Run_Backward.fbx")) {

				std::cout << "Aqua Running Backwards animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("StrafeRight", "ShadowHeartless_Walk_Strafe_Right.fbx")) {

				std::cout << "Shadow Strafe Right animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("StrafeLeft", "ShadowHeartless_Walk_Strafe_Left.fbx")) {

				std::cout << "Shadow Strafe Right animation loaded" << std::endl;
			}

			if (skinMeshComp->loadMeshAnimation("Action", "ShadowHeartless_Attack.fbx")) {

				std::cout << "Shadow Attack animation loaded" << std::endl;
			}

			skinMeshComp->friendlyName = "Shadow";

			// NOTE: Not sure if next line is necessary 
			cMesh* pMesh = skinMeshComp->createMeshObjectFromCurrentModel();
			if (pMesh) {
				std::cout << "Shadow was loaded" << std::endl;
			}
			else {
				std::cout << "ERROR: Didn't load Shadow" << std::endl;
			}

			// Convert assimp model to VAO
			if (!assimpToVAOConverter(skinMeshComp)) {
				std::cout << "ERROR: Didn't copy Shadow to VAO Format" << std::endl;
			}
			else {

				skinMeshComp->pAniState = new cAnimationState();

				skinMeshComp->pAniState->defaultAnimation.name = "Idle";
			}
		}

	}
	break;

	case 8:
	{
		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			pTransformComponent->setUniformScale(1.0f);
		}

		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
			meshComp->bIsVisible = false;
			meshComp->bIsWireFrame = false;
			meshComp->setDiffuseColour(pColour->getColourRGB(cGLColourHelper::BLUE_VIOLET));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		entity->addComponent<cClothComponent>();

		cMeshRenderSystem::getInstance()->registerEntity(entity);

	}
	break;

	case 9:
	{

		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "cone_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->friendlyName = "Cone";
		}

		entity->addComponent<cRigidBodyComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 10:
	{

		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "capsule_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->friendlyName = "Capsule";
		}

		entity->addComponent<cRigidBodyComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 11:
	{

		entity->addComponent<cTransformComponent>();
		{
			cTransformComponent* pTransformComponent = entity->getComponent<cTransformComponent>();
			pTransformComponent->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		entity->addComponent<cEulerMovementComponent>();
		entity->addComponent<cRenderMeshComponent>();
		{
			cRenderMeshComponent* meshComp = entity->getComponent<cRenderMeshComponent>();
			meshComp->meshName = "cylinder_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->friendlyName = "Cylinder";
		}

		entity->addComponent<cRigidBodyComponent>();

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	}
}

template<typename T>
T cEntityBuilder::getRandBetween0and1(void)
{
	return (T)((double)rand() / (RAND_MAX));
}

template <class T>
T cEntityBuilder::getRandInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
}