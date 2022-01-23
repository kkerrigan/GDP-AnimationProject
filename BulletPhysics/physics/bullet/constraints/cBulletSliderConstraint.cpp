/**
 * @file cBulletSliderConstraint.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletSliderConstraint class.
 */

#include "cBulletSliderConstraint.h"
#include "../nConvert.h"
#include "../cBulletRigidBody.h"

nPhysics::cBulletSliderConstraint::cBulletSliderConstraint(const sSliderConstraintDef& def) {

	btRigidBody* rbA = dynamic_cast<cBulletRigidBody*>(def.rigidBodyA)->getBulletBody();
	rbA->setActivationState(DISABLE_DEACTIVATION);

	cBulletRigidBody* bodyB = dynamic_cast<cBulletRigidBody*>(def.rigidBodyB);

	// Two different kinds of Point 2 Point constraints can be created depending on the number of rigid bodies

	if (bodyB == nullptr) {

		this->mConstraint = new btSliderConstraint(
			*rbA,
			btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.frameAEuler))),
			def.bUseLinearReferenceFrameA
		);
	}
	else {

		btRigidBody* rbB = bodyB->getBulletBody();
		rbB->setActivationState(DISABLE_DEACTIVATION);

		this->mConstraint = new btSliderConstraint(
			*rbA,
			*rbB,
			btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.frameAEuler))),
			btTransform(nConvert::toBullet(nConvert::createOrientationEulerAngles(def.frameBEuler))),
			def.bUseLinearReferenceFrameA
		);
	}

	return;
}

nPhysics::cBulletSliderConstraint::~cBulletSliderConstraint() {

	if (this->mConstraint) {

		delete this->mConstraint;
		this->mConstraint = nullptr;
	}
}
