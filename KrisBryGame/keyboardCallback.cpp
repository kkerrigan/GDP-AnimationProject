#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cRenderMeshComponent.h>
#include <cParticleComponent.h>
#include <cClothComponent.h>
#include <cLuaBrain.h>
#include <cScriptingCommandSystem.h>
#include <cSerialization.h>
#include <cRigidBodyComponent.h>
#include <cSkinnedMeshComponent.h>
#include <cFormationBehaviour.h>
#include <cCrowdFollowBehaviour.h>

bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);
void startFlockBoids(bool toggle);

extern std::map<std::string, cCommandGroup*> commandGroups;
extern std::map<std::string, iCommand*> commands;
extern cFormationBehaviour* pFormation;
extern cCrowdFollowBehaviour* pCrowdFollow;

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pScene = pSceneManager->getActiveScene();
	sLight* selectedLight = pScene->getSelectedLight();
	cEntity* selectedEntity = pScene->getSelectedEntity();

	cSkinnedMeshComponent* pSkinComp = selectedEntity->getComponent<cSkinnedMeshComponent>();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		pScene->setIsScenePaused(!pScene->getIsScenePaused());
	}

	if (areAllModifiersUp(window)) {

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			pScene->changeSelectedRigidBody();
		}

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			
			pScene->changeSelectedRigidBody(false);
		}

		// More Animation Controls
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

			if (pSkinComp->pAniState->animationQueue.empty() || pSkinComp->pAniState->animationQueue.front().name != "RunningJump") {
				
				cAnimationState::sStateDetails nextAnimation;
				nextAnimation.name = "RunningJump";
				nextAnimation.totalTime = pSkinComp->findAnimationDuration(nextAnimation.name);
				nextAnimation.frameStepTime = 0.01f;
				
				pSkinComp->pAniState->animationQueue.push(nextAnimation);
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			
			if (pSkinComp->pAniState->animationQueue.empty() || pSkinComp->pAniState->animationQueue.front().name != "RunningJump") {
				
				cAnimationState::sStateDetails nextAnimation;
				nextAnimation.name = "Jump";
				nextAnimation.totalTime = pSkinComp->findAnimationDuration(nextAnimation.name);
				nextAnimation.frameStepTime = 0.01f;
				
				pSkinComp->pAniState->animationQueue.push(nextAnimation);
			}
		}

		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			
			cAnimationState::sStateDetails nextAnimation;
			nextAnimation.name = "Action";
			nextAnimation.totalTime = pSkinComp->findAnimationDuration(nextAnimation.name);
			nextAnimation.frameStepTime = 0.01f;
			
			pSkinComp->pAniState->animationQueue.push(nextAnimation);
		}
	}

	// Shift down? Manipulate light objects
	if (isShiftDown(window)) {

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (pScene->selectedLight == pScene->getNumberOfLights() - 1) {
				pScene->selectedLight = 0;
			}
			else {
				pScene->selectedLight++;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			if (pScene->selectedLight != 0) {
				pScene->selectedLight--;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_7)) { // Turn on/off debug light

			pScene->bIsLightDebug = !pScene->bIsLightDebug;

		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

			if (pSkinComp->pAniState->animationQueue.empty() || pSkinComp->pAniState->animationQueue.front().name != "RunningJump") {

				cAnimationState::sStateDetails nextAnimation;
				nextAnimation.name = "RunningJump";
				nextAnimation.totalTime = pSkinComp->findAnimationDuration(nextAnimation.name);
				nextAnimation.frameStepTime = 0.01f;

				pSkinComp->pAniState->animationQueue.push(nextAnimation);
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

			if (pSkinComp->pAniState->animationQueue.empty() || pSkinComp->pAniState->animationQueue.front().name != "RunningJump") {

				cAnimationState::sStateDetails nextAnimation;
				nextAnimation.name = "Jump";
				nextAnimation.totalTime = pSkinComp->findAnimationDuration(nextAnimation.name);
				nextAnimation.frameStepTime = 0.01f;

				pSkinComp->pAniState->animationQueue.push(nextAnimation);
			}
		}
	}

	// Alt down? Manipultate Enity objects
	if (isAltDown(window)) {

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			cRenderMeshComponent* meshComonent = selectedEntity->getComponent<cRenderMeshComponent>();

			meshComonent->bIsWireFrame = !meshComonent->bIsWireFrame;
		}
	}

	// Ctrl down? Other options
	if (isCtrlDown(window)) {
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			
			cSerialization::serializeSceneCamera("cameras.json");
			cSerialization::serializeSceneLights("lights.json");
			cSerialization::serializeSceneSounds("sounds.json");
			//cSerialization::serializeSceneEntities("entities.json");
		}

		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			pScene->bIsRenderDebug = !pScene->bIsRenderDebug;
		}

		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			pScene->bDisplayAABBS = !pScene->bDisplayAABBS;
		}

		if (key == GLFW_KEY_C && action == GLFW_PRESS) {

			pScene->bDisplayCamInfo = !pScene->bDisplayCamInfo;
		}

		if (key == GLFW_KEY_F && action == GLFW_PRESS) {

			pScene->bFollowSelectedEntity = !pScene->bFollowSelectedEntity;
		}
	}
}