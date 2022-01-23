/**
 * @file cBulletBallSocketContraint.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletBallSocketContraint class.
 */

#include "cBulletBallSocketContraint.h"
#include "../cBulletRigidBody.h"
#include "nConvert.h"

nPhysics::cBulletBallSocketContraint::cBulletBallSocketContraint(const sBallSocketConstraintDef& def) {

	btRigidBody* rbA = dynamic_cast<cBulletRigidBody*>(def.rigidBodyA)->getBulletBody();
	rbA->setActivationState(DISABLE_DEACTIVATION);

	btRigidBody* rbB = dynamic_cast<cBulletRigidBody*>(def.rigidBodyB)->getBulletBody();
	rbB->setActivationState(DISABLE_DEACTIVATION);

	// Two different kinds of Point 2 Point constraints can be created depending on the number of rigid bodies

	if (rbB == nullptr) {

		this->mConstraint = new btPoint2PointConstraint(
			*rbA,
			nConvert::toBullet(def.pivotInA)
		);
	}
	else {

		this->mConstraint = new btPoint2PointConstraint(
			*rbA,
			*rbB,
			nConvert::toBullet(def.pivotInA),
			nConvert::toBullet(def.pivotInB)
		);
	}

	return;
}

nPhysics::cBulletBallSocketContraint::~cBulletBallSocketContraint() {

	if (this->mConstraint) {

		delete this->mConstraint;
		this->mConstraint = nullptr;
	}

	return;
}
