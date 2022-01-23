#include "cBulletPhysicsFactory.h"
#include "cBulletPhysicsWorld.h"
#include "cBulletRigidBody.h"
#include "AllBulletShapes.h"
#include "../bullet/constraints/cBulletHingeConstraint.h"
#include "../bullet/constraints/cBulletBallSocketContraint.h"
#include "../bullet/constraints/cBulletSliderConstraint.h"
#include "../bullet/constraints/cBulletConeTwistConstraint.h"
#include "../bullet/constraints/cBulletFixedConstraint.h"
#include "nConvert.h"

nPhysics::iPhysicsWorld* nPhysics::cBulletPhysicsFactory::createPhysicsWorld() {

	return new nPhysics::cBulletPhysicsWorld();
}

nPhysics::iRigidBody* nPhysics::cBulletPhysicsFactory::createRigidBody(const sRigidBodyDef& def, iShape* shape) {

	btRigidBody* bulletBody = nullptr;

	if (shape->getShapeType() == nPhysics::SHAPE_TYPE_SPHERE) {

		cBulletSphereShape* sphere = static_cast<cBulletSphereShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position)));

		btScalar mass = def.mass;
		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			sphere->getShape()->calculateLocalInertia(mass, localInertia);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, sphere->getShape(), localInertia);
		rbInfo.m_restitution = 1.0f;
		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_PLANE) {

		cBulletPlaneShape* plane = static_cast<cBulletPlaneShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position)));

		btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, plane->getShape(), btVector3(0, 0, 0));
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_BOX) {

		cBulletBoxShape* box = static_cast<cBulletBoxShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position)));

		btVector3 localInertia(0, 0, 0);
		box->getShape()->calculateLocalInertia(def.mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(
			def.mass,
			motionState,
			box->getShape(),
			localInertia // Inertia
		);
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_CONE) {

		cBulletConeShape* cone = static_cast<cBulletConeShape*>(shape);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position))
		);

		btVector3 localInertia(0, 0, 0);
		cone->getShape()->calculateLocalInertia(def.mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(
			def.mass,
			motion_state,
			cone->getShape(),
			localInertia
		);
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_CAPSULE) {

		cBulletCapsuleShape* capsule = static_cast<cBulletCapsuleShape*>(shape);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position))
		);

		btVector3 localInertia(0, 0, 0);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(
			def.mass,
			motion_state,
			capsule->getShape(),
			localInertia
		);
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}
	else if (shape->getShapeType() == nPhysics::SHAPE_TYPE_CYLINDER) {

		cBulletCylinderShape* cylinder = static_cast<cBulletCylinderShape*>(shape);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.orientation)),
			nConvert::toBullet(def.position)));

		btVector3 localInertia(0, 0, 0);
		cylinder->getShape()->calculateLocalInertia(def.mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(
			def.mass,
			motionState,
			cylinder->getShape(),
			localInertia // Inertia
		);
		rbInfo.m_restitution = 0.5f;

		bulletBody = new btRigidBody(rbInfo);
	}

	cBulletRigidBody* rigidBody = new cBulletRigidBody(bulletBody, shape);
	return rigidBody;
}

nPhysics::iSphereShape* nPhysics::cBulletPhysicsFactory::createSphere(float radius) {

	return new cBulletSphereShape(radius);
}

nPhysics::iPlaneShape* nPhysics::cBulletPhysicsFactory::createPlane(const glm::vec3& normal, float constant) {

	return new cBulletPlaneShape(normal, constant);
}

nPhysics::iSoftBody* nPhysics::cBulletPhysicsFactory::createSoftBody(const sSoftBodyDef& def) {

	return nullptr;
}

nPhysics::iBoxShape* nPhysics::cBulletPhysicsFactory::createBoxShape(const glm::vec3& halfLength) {

	return new cBulletBoxShape(halfLength);
}

nPhysics::iConeShape* nPhysics::cBulletPhysicsFactory::createConeShape(float radius, float height) {

	return new cBulletConeShape(radius, height);
}

nPhysics::iCapsuleShape* nPhysics::cBulletPhysicsFactory::createCapsuleShape(float radius, float height) {
	
	return new cBulletCapsuleShape(radius, height);
}

nPhysics::iCylinderShape* nPhysics::cBulletPhysicsFactory::createCylinderShape(const glm::vec3& halfExtents) {
	
	return new cBulletCylinderShape(halfExtents);
}

nPhysics::iConstraint* nPhysics::cBulletPhysicsFactory::createHingeConstraint(const sHingeConstraintDef& def) {
	
	return new cBulletHingeConstraint(def);
}

nPhysics::iConstraint* nPhysics::cBulletPhysicsFactory::createBallSocketConstraint(const sBallSocketConstraintDef& def) {
	
	return new cBulletBallSocketContraint(def);
}

nPhysics::iConstraint* nPhysics::cBulletPhysicsFactory::createSliderConstraint(const sSliderConstraintDef& def) {
	
	return new cBulletSliderConstraint(def);
}

nPhysics::iConstraint* nPhysics::cBulletPhysicsFactory::createConeTwistConstraint(const sConeTwistConstraintDef& def) {
	
	return new cBulletConeTwistConstraint(def);
}

nPhysics::iConstraint * nPhysics::cBulletPhysicsFactory::createFixedConstraint(const sFixedConstraintDef& def) {
	
	return new cBulletFixedConstraint(def);
}


DLL_EXPORT nPhysics::iPhysicsFactory* createPhysicsFactory() {

	return new nPhysics::cBulletPhysicsFactory();
}