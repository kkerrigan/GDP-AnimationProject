#include <Global.h>
#include <Common.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <cRigidBodyComponent.h>
#include <cSkinnedMeshComponent.h>
#include <cBehaviourComponent.h>
#include <cFlockBehaviour.h>
#include <iInputCommand.h>

#include <cMeshRenderSystem.h>

bool isShiftDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window) {
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void keyboardCallbackASYNC(GLFWwindow* window, float deltaTime) {
	/*
	* All keypresses get sent to the "mediator" (scene) and are handled there. The command do no know the
	* actual object they are going to act on until they are executed
	*/

	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	const float CAMERA_SPEED_SLOW = 0.1f;
	const float CAMERA_SPEED_FAST = 1.0f;

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	cEntity* pSelectedEntity = pScene->getSelectedEntity();
	std::vector<cEntity*> entities = pScene->getEntities();

	cSkinnedMeshComponent* pSkinMeshComp = pSelectedEntity->getComponent<cSkinnedMeshComponent>();
	cTransformComponent* pTransComp = pSelectedEntity->getComponent<cTransformComponent>();

	glm::vec3 forwardVec = glm::toMat3(pTransComp->getQOrientation()) * FORWARD;

	// If no keys are down, move the camera
	if (areAllModifiersUp(window)) {

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) { // Idle

			pSkinMeshComp->pAniState->defaultAnimation.name = "Idle";
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

			pTransComp->setPosition(pTransComp->getPosition() + 10.0f * forwardVec * deltaTime);

			// Set animation
			pSkinMeshComp->pAniState->defaultAnimation.name = "Walk";
			pSkinMeshComp->pAniState->defaultAnimation.frameStepTime += 0.01;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

			pTransComp->setPosition(pTransComp->getPosition() - 10.0f * forwardVec * deltaTime);

			// Set animation
			pSkinMeshComp->pAniState->defaultAnimation.name = "WalkBack";
			pSkinMeshComp->pAniState->defaultAnimation.frameStepTime += 0.01;

		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

			pTransComp->adjOrientationEulerAngles(glm::vec3(0.0, 3.0f, 0.0f), true);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

			pTransComp->adjOrientationEulerAngles(glm::vec3(0.0f, -3.0f, 0.0f), true);
		}

		// Camera Controls
		if (glfwGetKey(window, GLFW_KEY_I)) {

			pScene->handleCommand(new cMoveCameraForwardCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_K)) {

			pScene->handleCommand(new cMoveCameraForwardCommand(5.0f * -cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_J)) {

			pScene->handleCommand(new cMoveCameraLeftRightCommand(5.0f * -cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_L)) {

			pScene->handleCommand(new cMoveCameraLeftRightCommand(5.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(cameraSpeed));
		} // End of Camera controls
	}

	// Shift down? Manipulate light objects
	if (isShiftDown(window)) {

		sLight* selectedLight = pScene->getSelectedLight();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) { // Idle

			pSkinMeshComp->pAniState->defaultAnimation.name = "Idle";
		}

		if (glfwGetKey(window, GLFW_KEY_W)) { // Forward
			
			pTransComp->setPosition(pTransComp->getPosition() + (20.0f * forwardVec * deltaTime));

			pSkinMeshComp->pAniState->defaultAnimation.name = "Running";
			pSkinMeshComp->pAniState->defaultAnimation.frameStepTime += 0.01f;

			//pScene->handleCommand(new cMoveLightForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S)) { // Backward
			
			pTransComp->setPosition(pTransComp->getPosition() - (20.0f * forwardVec * deltaTime));

			pSkinMeshComp->pAniState->defaultAnimation.name = "RunningBack";
			pSkinMeshComp->pAniState->defaultAnimation.frameStepTime += 0.01f;

			//pScene->handleCommand(new cMoveLightForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {  // Left
			
			pTransComp->adjOrientationEulerAngles(glm::vec3(0.0, 3.0f, 0.0f), true);
			
			//pScene->handleCommand(new cMoveLightLeftRightCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_D)) { // Right
			
			pTransComp->adjOrientationEulerAngles(glm::vec3(0.0, -3.0f, 0.0f), true);

			//pScene->handleCommand(new cMoveLightLeftRightCommand(cameraSpeed));
		}


		// Reamining old light controls
		if (glfwGetKey(window, GLFW_KEY_Q)) { // Up
			
			pScene->handleCommand(new cMoveLightUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E)) { // Down
			pScene->handleCommand(new cMoveLightUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_1)) { // Decrease Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_2)) { //Increase Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_3)) { // Decrease Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_4)) { // Increase Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_5)) { // Decrease Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_6)) { // Increase Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(1.01f));
		}
	}

	// Alt down? Manipultate Enity objects
	if (isAltDown(window)) {


		// Strafing controls
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			
			glm::vec3 left = pTransComp->getQOrientation() * glm::vec3(1.0f, 0.0f, 0.0f);
			pTransComp->setPosition(pTransComp->getPosition() + 5.0f * left * deltaTime);
			
			pSkinMeshComp->pAniState->defaultAnimation.name = "StrafeLeft";
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			
			glm::vec3 left = pTransComp->getQOrientation() * glm::vec3(1.0f, 0.0f, 0.0f);
			pTransComp->setPosition(pTransComp->getPosition() + -5.0f * left * deltaTime);
			
			pSkinMeshComp->pAniState->defaultAnimation.name = "StrafeRight";
		}


		// Modifying model positions
		//if (glfwGetKey(window, GLFW_KEY_W)) {
		//	pScene->handleCommand(new cMoveEntiyForwardCommand(cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_S)) {
		//	pScene->handleCommand(new cMoveEntiyForwardCommand(-cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_A)) {
		//	pScene->handleCommand(new cMoveEntiyLeftRightCommand(cameraSpeed));
		//}
		//if (glfwGetKey(window, GLFW_KEY_D)) {
		//	pScene->handleCommand(new cMoveEntiyLeftRightCommand(-cameraSpeed));
		//}
		//if (glfwGetKey(window, GLFW_KEY_Q)) {
		//	pScene->handleCommand(new cMoveEntiyUpDownCommand(cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_E)) {
		//	pScene->handleCommand(new cMoveEntiyUpDownCommand(-cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_KP_ADD)) {
		//	pScene->handleCommand(new cChangeEntitySpecularPower(1.01f));
		//}

		//if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT)) {
		//	pScene->handleCommand(new cChangeEntitySpecularPower(0.99f));
		//}

		//if (glfwGetKey(window, GLFW_KEY_I)) {
		//	pScene->handleCommand(new cRotateEntiyXCommand(5.0f * cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_K)) {
		//	pScene->handleCommand(new cRotateEntiyXCommand(-5.0f * cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_U)) {
		//	pScene->handleCommand(new cRotateEntiyYCommand(5.0f * cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_O)) {
		//	pScene->handleCommand(new cRotateEntiyYCommand(-5.0f * cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_J)) {
		//	pScene->handleCommand(new cRotateEntiyZCommand(5.0f * cameraSpeed));
		//}

		//if (glfwGetKey(window, GLFW_KEY_L)) {
		//	pScene->handleCommand(new cRotateEntiyZCommand(-5.0f * cameraSpeed));
		//}

		//// Scale
		//if (glfwGetKey(window, GLFW_KEY_PERIOD)) {

		//	cEntity* pEntity = pScene->getSelectedEntity();
		//	cTransformComponent* pTransComp = pEntity->getComponent<cTransformComponent>();

		//	pTransComp->scale *= 1.01f;
		//}

		//if (glfwGetKey(window, GLFW_KEY_COMMA)) {

		//	cEntity* pEntity = pScene->getSelectedEntity();
		//	cTransformComponent* pTransComp = pEntity->getComponent<cTransformComponent>();

		//	pTransComp->scale *= 0.99f;
		//}
	}

	// Is Ctrl down? Other operations
	if (isCtrlDown(window)) {

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Roll camera to the left
			pScene->handleCommand(new cRollCameraCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Roll camera to the right
			pScene->handleCommand(new cRollCameraCommand(+cameraSpeed));
		}
	}
}