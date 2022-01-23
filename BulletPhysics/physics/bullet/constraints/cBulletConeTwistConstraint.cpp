/**
 * @file cBulletConeTwistConstraint.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletConeTwistConstraint class.
 */

#include "cBulletConeTwistConstraint.h"
#include "../btBulletDynamicsCommon.h"
#include "../nConvert.h"
#include "../cBulletRigidBody.h"

#define SIMD_PI_2 ((SIMD_PI)*0.5f)
#define SIMD_PI_4 ((SIMD_PI)*0.25f)

nPhysics::cBulletConeTwistConstraint::cBulletConeTwistConstraint(const sConeTwistConstraintDef& def) {

	btRigidBody* rbA = dynamic_cast<cBulletRigidBody*>(def.rigidBodyA)->getBulletBody();
	rbA->setActivationState(DISABLE_DEACTIVATION);

	btRigidBody* rbB = dynamic_cast<cBulletRigidBody*>(def.rigidBodyB)->getBulletBody();

	btTransform frameInA, frameInB;
	
	frameInA = btTransform::getIdentity();
	frameInA.getBasis().setEulerZYX(0, 0, SIMD_PI_2);
	frameInA.setOrigin(nConvert::toBullet(def.frameAEuler));
	
	frameInB = btTransform::getIdentity();
	frameInB.getBasis().setEulerZYX(0, 0, SIMD_PI_2);
	frameInB.setOrigin(nConvert::toBullet(def.frameBEuler));


	this->mConstraint = new btConeTwistConstraint(*rbA, *rbB, frameInA, frameInB);
	this->mConstraint->setLimit(btScalar(SIMD_PI * 0.6f), btScalar(SIMD_PI_4), btScalar(SIMD_PI) * 0.8f, 0.5f);

	return;
}

nPhysics::cBulletConeTwistConstraint::~cBulletConeTwistConstraint() {

	if (this->mConstraint) {

		delete this->mConstraint;
		this->mConstraint = nullptr;
	}

	return;
}
