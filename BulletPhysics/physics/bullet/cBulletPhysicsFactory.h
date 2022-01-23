/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cBulletPhysicsFactory_HG_
#define _cBulletPhysicsFactory_HG_

#include <physics/interfaces/iPhysicsFactory.h>
#include <physics/interfaces/iConstraint.h>

namespace nPhysics {

	class cBulletPhysicsFactory : public iPhysicsFactory {
	public:
		virtual iPhysicsWorld* createPhysicsWorld() override;
		
		// Bodies
		virtual iRigidBody* createRigidBody(const sRigidBodyDef& def, iShape* shape) override;
		virtual iSoftBody* createSoftBody(const sSoftBodyDef& def) override;

		// Shapes
		virtual iSphereShape* createSphere(float radius) override;
		virtual iPlaneShape* createPlane(const glm::vec3& normal, float constant) override;
		virtual iBoxShape* createBoxShape(const glm::vec3& halfLength) override;
		virtual iConeShape* createConeShape(float radius, float height) override;
		virtual iCapsuleShape* createCapsuleShape(float radius, float height) override;
		virtual iCylinderShape* createCylinderShape(const glm::vec3& halfExtents) override;
		
		// Constraints
		virtual iConstraint* createHingeConstraint(const sHingeConstraintDef& def) override;
		virtual iConstraint* createBallSocketConstraint(const sBallSocketConstraintDef& def) override;
		virtual iConstraint* createSliderConstraint(const sSliderConstraintDef& def) override;
		virtual iConstraint* createConeTwistConstraint(const sConeTwistConstraintDef& def) override;
		virtual iConstraint* createFixedConstraint(const sFixedConstraintDef& def) override;
		
	};
}

#endif // !_cBulletPhysicsFactory_HG_
