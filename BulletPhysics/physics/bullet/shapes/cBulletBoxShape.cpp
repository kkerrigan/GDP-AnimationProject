/**
 * @file cBulletBoxShape.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletBoxShape class.
 */

#include "cBulletBoxShape.h"
#include "nConvert.h"

nPhysics::cBulletBoxShape::cBulletBoxShape(glm::vec3 halfExtents) {

	this->mBoxShape = new btBoxShape(nConvert::toBullet(halfExtents));

	return;
}

nPhysics::cBulletBoxShape::~cBulletBoxShape() {

	if (this->mBoxShape) {

		delete this->mBoxShape;
		this->mBoxShape = nullptr;
	}

	return;
}
