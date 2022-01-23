/**
 * @file cBulletHingeConstraint.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletHingeConstraint class.
 */

#include "cBulletHingeConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

nPhysics::cBulletHingeConstraint::cBulletHingeConstraint(const sHingeConstraintDef& def) {

	btRigidBody* rbA = static_cast<cBulletRigidBody*>(def.rigidBodyA)->getBulletBody();
	rbA->setActivationState(DISABLE_DEACTIVATION);

	btRigidBody* rbB = static_cast<cBulletRigidBody*>(def.rigidBodyB)->getBulletBody();
	rbB->setActivationState(DISABLE_DEACTIVATION);

	this->mConstraint = new btHingeConstraint(
		*rbB,
		*rbA,
		nConvert::toBullet(def.pivotInA),
		nConvert::toBullet(def.pivotInB),
		nConvert::toBullet(def.axisInA),
		nConvert::toBullet(def.axisInB)
	);
	//this->mConstraint->setLimit(-SIMD_HALF_PI, SIMD_HALF_PI);

	return;
}

nPhysics::cBulletHingeConstraint::~cBulletHingeConstraint() {

	if (this->mConstraint) {

		delete this->mConstraint;
		this->mConstraint = nullptr;
	}

	return;
}