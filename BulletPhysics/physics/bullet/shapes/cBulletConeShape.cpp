/**
 * @file cBulletConeShape.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBulletConeShape class.
 */

#include "cBulletConeShape.h"
#include "nConvert.h"

nPhysics::cBulletConeShape::cBulletConeShape(float radius, float height) {

	this->mShape = new btConeShape(nConvert::toBullet(radius), nConvert::toBullet(height));

	return;
}

nPhysics::cBulletConeShape::~cBulletConeShape() {

	if (this->mShape) {

		delete this->mShape;
		this->mShape = nullptr;
	}

}

float nPhysics::cBulletConeShape::getRadius() {
	
	return nConvert::toGlm(this->mShape->getRadius());
}

float nPhysics::cBulletConeShape::getHeight() {
	
	return nConvert::toGlm(this->mShape->getHeight());
}
