/**
 * @file cBulletCapsuleShape.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletCapsuleShape class.
 */

#include "cBulletCapsuleShape.h"
#include "nConvert.h"

nPhysics::cBulletCapsuleShape::cBulletCapsuleShape(float radius, float height) {

	this->mShape = new btCapsuleShape(nConvert::toBullet(radius), nConvert::toBullet(height));

	return;
}

nPhysics::cBulletCapsuleShape::~cBulletCapsuleShape() {

	if (this->mShape) {

		delete this->mShape;
		this->mShape = nullptr;
	}

}

float nPhysics::cBulletCapsuleShape::getRadius() {
	
	return nConvert::toGlm(this->mShape->getRadius());
}

float nPhysics::cBulletCapsuleShape::getHeight() {
	
	return nConvert::toGlm(this->mShape->getHalfHeight()) * 2.0f;
}