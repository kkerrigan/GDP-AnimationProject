/**
 * @file sConeTwistConstraintDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _sConeTwistConstraintDef_HG_
#define _sConeTwistConstraintDef_HG_

#include "../interfaces/eConstraintTypes.h"
#include "../interfaces/iRigidBody.h"

namespace nPhysics {

	struct sConeTwistConstraintDef {

		sConeTwistConstraintDef() :
			constraintType(eConstraintTypes::CONSTRAINT_TYPE_CONE_TWIST),
			rigidBodyA(nullptr),
			rigidBodyB(nullptr),
			frameAEuler(glm::vec3(0.0f)),
			frameBEuler(glm::vec3(0.0f))
		{}

		eConstraintTypes constraintType;
		iRigidBody* rigidBodyA;
		iRigidBody* rigidBodyB;

		glm::vec3 frameAEuler;
		glm::vec3 frameBEuler;
	};
}


#endif // !_sConeTwistConstraintDef_HG_

