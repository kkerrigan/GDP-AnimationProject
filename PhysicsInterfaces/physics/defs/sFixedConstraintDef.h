/**
 * @file sFixedConstraintDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _sFixedConstraintDef_HG_
#define _sFixedConstraintDef_HG_

#include "../interfaces/eConstraintTypes.h"
#include "../interfaces/iRigidBody.h"

namespace nPhysics {

	struct sFixedConstraintDef {

		sFixedConstraintDef() :
			constraintType(eConstraintTypes::CONSTRAINT_TYPE_FIXED),
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

#endif // !_sFixedConstraintDef_HG_

