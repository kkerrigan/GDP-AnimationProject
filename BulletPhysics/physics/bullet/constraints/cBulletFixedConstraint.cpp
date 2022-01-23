/**
 * @file cBulletFixedConstraint.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletFixedConstraint class.
 */

#include "cBulletFixedConstraint.h"
#include "../nConvert.h"
#include "../cBulletRigidBody.h"

nPhysics::cBulletFixedConstraint::cBulletFixedConstraint(const sFixedConstraintDef& def) {

	btRigidBody* rbA = static_cast<cBulletRigidBody*>(def.rigidBodyA)->getBulletBody();
	btRigidBody* rbB = static_cast<cBulletRigidBody*>(def.rigidBodyB)->getBulletBody();

	btMatrix3x3 mat;
	mat.setIdentity();
	
	btTransform frameInA = btTransform(mat, nConvert::toBullet(def.pivotInA));
	btTransform frameInB = btTransform(mat, nConvert::toBullet(def.pivotInB));

	this->mConstraint = new btFixedConstraint(*rbA, *rbB, frameInA, frameInB);

	return;
}

nPhysics::cBulletFixedConstraint::~cBulletFixedConstraint() {

	if (this->mConstraint) {

		delete this->mConstraint;
		this->mConstraint = nullptr;
	}
}
