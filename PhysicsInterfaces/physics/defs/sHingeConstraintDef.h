/**
 * @file sHingeConstraintDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This struct defines the components needed for a Ball and Socket (Point2Point) constraint in Bullet.
 */

#ifndef _sHingeConstraintDef_HG_
#define _sHingeConstraintDef_HG_

#include "../interfaces/eConstraintTypes.h"
#include "../interfaces/iRigidBody.h"

namespace nPhysics {

	struct sHingeConstraintDef {

		sHingeConstraintDef() :
			constraintType(eConstraintTypes::CONSTRAINT_TYPE_HINGE),
			rigidBodyA(nullptr),
			rigidBodyB(nullptr),
			pivotInA(glm::vec3(0.0f)),
			pivotInB(glm::vec3(0.0f)),
			axisInA(glm::vec3(0.0f)),
			axisInB(glm::vec3(0.0f))
		{}

		eConstraintTypes constraintType;
		iRigidBody* rigidBodyA;
		iRigidBody* rigidBodyB;

		glm::vec3 pivotInA;
		glm::vec3 pivotInB;

		glm::vec3 axisInA;
		glm::vec3 axisInB;
	};
}

#endif // !_sBallSocketConstraintDef_HG_