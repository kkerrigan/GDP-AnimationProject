/**
 * @file cBulletCylinderShape.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletCylinderShape class.
 */

#include "cBulletCylinderShape.h"
#include "nConvert.h"

nPhysics::cBulletCylinderShape::cBulletCylinderShape(const glm::vec3& halfExtents) {

	this->mShape = new btCylinderShape(nConvert::toBullet(halfExtents));

	return;
}

nPhysics::cBulletCylinderShape::~cBulletCylinderShape() {

	if (this->mShape) {

		delete this->mShape;
		this->mShape = nullptr;
	}
}