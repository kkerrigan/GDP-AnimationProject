#include "cBulletPhysicsWorld.h"
#include "cBulletRigidBody.h"
#include "../bullet/constraints/cBulletHingeConstraint.h"
#include "../bullet/constraints/cBulletBallSocketContraint.h"
#include "../bullet/constraints/cBulletSliderConstraint.h"
#include "../bullet/constraints/cBulletConeTwistConstraint.h"
#include "../bullet/constraints/cBulletFixedConstraint.h"
#include "nConvert.h"

nPhysics::cBulletPhysicsWorld::cBulletPhysicsWorld() {

	this->mCollisionConfiguration = new btDefaultCollisionConfiguration();
	this->mDispatcher = new btCollisionDispatcher(this->mCollisionConfiguration);
	this->mOverlappingPairCache = new btDbvtBroadphase();
	this->mSolver = new btSequentialImpulseConstraintSolver;
	this->mDynamicsWorld = new btDiscreteDynamicsWorld(this->mDispatcher, this->mOverlappingPairCache,
	this->mSolver, this->mCollisionConfiguration);

	this->setGravity(glm::vec3(0.0f, -9.81f, 0.0f));

	return;
}

nPhysics::cBulletPhysicsWorld::~cBulletPhysicsWorld() {

	//cleanup the physics objects
	for (int i = this->mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {

		btCollisionObject* obj = this->mDynamicsWorld->getCollisionObjectArray()[i];
		
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		
		this->mDynamicsWorld->removeCollisionObject(obj);
		delete obj;

		for (int j = 0; j < this->mCollisionShapes.size(); j++) {
			
			btCollisionShape* shape = this->mCollisionShapes[j];
			this->mCollisionShapes[j] = nullptr;
			delete shape;
		}

		//delete dynamics world
		delete this->mDynamicsWorld;

		//delete solver
		delete this->mSolver;

		//delete broad phase
		delete this->mOverlappingPairCache;

		//delete dispatcher
		delete this->mDispatcher;

		//delete configuration
		delete this->mCollisionConfiguration;

		//clear the vector of shapes
		this->mCollisionShapes.clear();
	}

	return;
}

void nPhysics::cBulletPhysicsWorld::setGravity(const glm::vec3& grav) {

	this->mDynamicsWorld->setGravity(nConvert::toBullet(grav));

	return;
}

bool nPhysics::cBulletPhysicsWorld::addCollisionBody(iCollisionBody* pBody) {

	cBulletRigidBody* bulletBody = static_cast<cBulletRigidBody*>(pBody);
	if (!bulletBody) return false;

	mDynamicsWorld->addRigidBody(bulletBody->getBulletBody());

	return true;
}

bool nPhysics::cBulletPhysicsWorld::removeCollisionBody(iCollisionBody* pBody) {

	cBulletRigidBody* bulletBody = static_cast<cBulletRigidBody*>(pBody);
	if (!bulletBody) return false;

	btRigidBody* bulletRigidBody = bulletBody->getBulletBody();

	mDynamicsWorld->removeRigidBody(bulletRigidBody);

	return true;
}

void nPhysics::cBulletPhysicsWorld::addConstraint(iConstraint* constraint) {

	eConstraintTypes constraintType = constraint->getConstraintType();

	switch (constraintType)
	{
	case nPhysics::CONSTRAINT_TYPE_HINGE:
		this->mDynamicsWorld->addConstraint(static_cast<cBulletHingeConstraint*>(constraint)->getConstraint(), constraintType);
		break;
	case nPhysics::CONSTRAINT_TYPE_BALLSOCKET:
		this->mDynamicsWorld->addConstraint(static_cast<cBulletBallSocketContraint*>(constraint)->getConstraint(), constraintType);
		break;
	case nPhysics::CONSTRAINT_TYPE_SLIDER:
		this->mDynamicsWorld->addConstraint(static_cast<cBulletSliderConstraint*>(constraint)->getConstraint(), constraintType);
		break;
	case nPhysics::CONSTRAINT_TYPE_CONE_TWIST:
		this->mDynamicsWorld->addConstraint(static_cast<cBulletConeTwistConstraint*>(constraint)->getConstraint(), constraintType);
		break;
	case nPhysics::CONSTRAINT_TYPE_FIXED:
		this->mDynamicsWorld->addConstraint(static_cast<cBulletFixedConstraint*>(constraint)->getConstraint(), constraintType);
		break;
	default:
		break;
	}

	return;
}

void nPhysics::cBulletPhysicsWorld::update(float deltaTime) {

	this->mDynamicsWorld->stepSimulation(deltaTime, 0, 1.f / 60.f);

	return;
}
