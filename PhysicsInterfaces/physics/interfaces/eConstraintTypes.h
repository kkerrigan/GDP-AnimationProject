/**
 * @file eConstraintTypes.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * An enum for the possible constraint types.
 */

#ifndef _eConstraintTypes_HG_
#define _eConstraintTypes_HG_

namespace nPhysics {

	enum eConstraintTypes {

		CONSTRAINT_TYPE_HINGE = 0,
		CONSTRAINT_TYPE_BALLSOCKET = 1,
		CONSTRAINT_TYPE_SLIDER = 2,
		CONSTRAINT_TYPE_CONE_TWIST = 3,
		CONSTRAINT_TYPE_FIXED = 5
	};
}

#endif // !_eConstraintTypes_HG_
