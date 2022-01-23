/**
 * @file sSliderConstraintDef.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This struct defines the components needed for a Slider constraint in Bullet.
 */

#ifndef _sSliderConstraintDef_HG_
#define _sSliderConstraintDef_HG_

#include "../interfaces/eConstraintTypes.h"
#include "../interfaces/iRigidBody.h"

namespace nPhysics {

	struct sSliderConstraintDef {

		sSliderConstraintDef() :
			constraintType(eConstraintTypes::CONSTRAINT_TYPE_SLIDER),
			rigidBodyA(nullptr),
			rigidBodyB(nullptr),
			frameAEuler(glm::vec3(0.0f)),
			frameBEuler(glm::vec3(0.0f)),
			bUseLinearReferenceFrameA(false)
		{}

		eConstraintTypes constraintType;
		iRigidBody* rigidBodyA;
		iRigidBody* rigidBodyB;

		glm::vec3 frameAEuler;
		glm::vec3 frameBEuler;

		bool bUseLinearReferenceFrameA;
	};
}

#endif // !_sSliderConstraintDef_HG_

