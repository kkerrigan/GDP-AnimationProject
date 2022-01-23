/**
 * @file sBallSocketConstraintDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This struct defines the components needed for a Hinge constraint in Bullet.
 */

#ifndef _sBallSocketConstraintDef_HG_
#define _sBallSocketConstraintDef_HG_

#include "../interfaces/eConstraintTypes.h"
#include "../interfaces/iRigidBody.h"

namespace nPhysics {

	struct sBallSocketConstraintDef {

		sBallSocketConstraintDef() :
			constraintType(eConstraintTypes::CONSTRAINT_TYPE_BALLSOCKET),
			rigidBodyA(nullptr),
			rigidBodyB(nullptr),
			pivotInA(glm::vec3(0.0f)),
			pivotInB(glm::vec3(0.0f))
		{}

		eConstraintTypes constraintType;
		iRigidBody* rigidBodyA;
		iRigidBody* rigidBodyB;

		glm::vec3 pivotInA;
		glm::vec3 pivotInB;
	};
}

#endif // !_sBallSocketConstraintDef_HG_