#include "cBulletRigidBody.h"
#include "../bullet/shapes/cBulletSphereShape.h"
#include "../bullet/shapes/cBulletPlaneShape.h"
#include "nConvert.h"

nPhysics::cBulletRigidBody::cBulletRigidBody() {

	return;
}

nPhysics::cBulletRigidBody::cBulletRigidBody(btRigidBody* bulletBody, iShape* shape) {

	this->mShape = shape;
	this->mBody = bulletBody;

	return;
}


nPhysics::cBulletRigidBody::cBulletRigidBody(const cBulletRigidBody& copy) :iRigidBody(copy) {

	return;
}


nPhysics::cBulletRigidBody& nPhysics::cBulletRigidBody::operator=(const cBulletRigidBody& copy) {

	return *this;
}

nPhysics::cBulletRigidBody::~cBulletRigidBody() {

	delete this->mBody;
	this->mBody = nullptr;

	return;
}

void nPhysics::cBulletRigidBody::getPosition(glm::vec3& positionOut) {

	positionOut = nConvert::toGlm(this->mBody->getCenterOfMassPosition());

	return;
}

void nPhysics::cBulletRigidBody::setPosition(glm::vec3 position) {

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(nConvert::toBullet(position));

	this->mBody->setWorldTransform(transform);
	this->mBody->activate();

	return;
}

void nPhysics::cBulletRigidBody::getPreviousPosition(glm::vec3& positionOut) {

	// TODO: Physics Project 3 - implement

}

void nPhysics::cBulletRigidBody::setPreviousPosition(glm::vec3 position) {

	// TODO: Physics Project 3 - implement

}

void nPhysics::cBulletRigidBody::getOrientation(glm::quat& orientationOut) {

	orientationOut = nConvert::toGlm(this->mBody->getOrientation());

	return;
}

void nPhysics::cBulletRigidBody::setOrientation(glm::quat orientation) {

	btQuaternion ori = nConvert::toBullet(orientation);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(this->mBody->getCenterOfMassPosition());
	transform.setRotation(ori);

	this->mBody->setWorldTransform(transform);
	this->mBody->activate();

	return;
}

void nPhysics::cBulletRigidBody::adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees) {

	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	glm::quat currentOrientation;
	
	this->getOrientation(currentOrientation);
	currentOrientation *= rotationAdjust;
	this->setOrientation(currentOrientation);

	return;
}

void nPhysics::cBulletRigidBody::getVelocity(glm::vec3& velocityOut) {

	velocityOut = nConvert::toGlm(this->mBody->getLinearVelocity());

	return;
}

void nPhysics::cBulletRigidBody::setVelocity(glm::vec3 velocity) {

	this->mBody->setLinearVelocity(nConvert::toBullet(velocity));

	return;
}

void nPhysics::cBulletRigidBody::getAcceleration(glm::vec3& accelerationOut) {

	accelerationOut = nConvert::toGlm(this->mBody->getLinearFactor());

	return;
}

void nPhysics::cBulletRigidBody::setAcceleration(glm::vec3 acceleration) {

	this->mBody->setLinearFactor(nConvert::toBullet(acceleration));

	return;
}

void nPhysics::cBulletRigidBody::getMass(float& massOut) {


	return;
}

void nPhysics::cBulletRigidBody::setMass(float mass) {



	return;
}

void nPhysics::cBulletRigidBody::applyFoce(glm::vec3 forceAccel) {

	this->mBody->activate();
	this->mBody->applyCentralForce(nConvert::toBullet(forceAccel));

	return;
}

nPhysics::iShape* nPhysics::cBulletRigidBody::getShape() {

	return this->mShape;
}

void nPhysics::cBulletRigidBody::getAABB(glm::vec3& minOut, glm::vec3& maxOut) {

	return;
}

