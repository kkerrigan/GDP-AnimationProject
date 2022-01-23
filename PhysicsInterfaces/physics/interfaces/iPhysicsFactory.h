/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _iPhysicsFactory_HG_
#define _iPhysicsFactory_HG_

#define DLL_EXPORT extern "C" _declspec(dllexport)

#include <gameMath.h>

#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "../defs/sRigidBodyDef.h"
#include "../defs/sSoftBodyDef.h"
#include "../defs/sConeTwistConstraintDef.h"
#include "../defs/sFixedConstraintDef.h"
#include "AllShapeInterfaces.h"

#include "iConstraint.h"
#include "../defs/sHingeConstraintDef.h"
#include "../defs/sBallSocketConstraintDef.h"
#include "../defs/sSliderConstraintDef.h"

namespace nPhysics {

	class iPhysicsFactory {
	public:
		virtual ~iPhysicsFactory() {}
		virtual iPhysicsWorld* createPhysicsWorld() = 0;
		
		// Bodies
		virtual iRigidBody* createRigidBody(const sRigidBodyDef& def, iShape* shape) = 0;
		virtual iSoftBody* createSoftBody(const sSoftBodyDef& def) = 0;
		
		// SHapes
		virtual iSphereShape* createSphere(float radius) = 0;
		virtual iPlaneShape* createPlane(const glm::vec3& normal, float constant) = 0;
		virtual iBoxShape* createBoxShape(const glm::vec3& halfLength) = 0;
		virtual iConeShape* createConeShape(float radius, float height) = 0;
		virtual iCapsuleShape* createCapsuleShape(float radius, float height) = 0;
		virtual iCylinderShape* createCylinderShape(const glm::vec3& halfExtents) = 0;
		

		// Constraints
		virtual iConstraint* createHingeConstraint(const sHingeConstraintDef& def) = 0;
		virtual iConstraint* createBallSocketConstraint(const sBallSocketConstraintDef& def) = 0;
		virtual iConstraint* createSliderConstraint(const sSliderConstraintDef& def) = 0;
		virtual iConstraint* createConeTwistConstraint(const sConeTwistConstraintDef& def) = 0;
		virtual iConstraint* createFixedConstraint(const sFixedConstraintDef& def) = 0;
	};

	DLL_EXPORT iPhysicsFactory* createPhysicsFactory();
	using fCreatePhysicsFactory = iPhysicsFactory* (*)();
}

#endif