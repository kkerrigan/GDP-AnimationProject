/**
 * @file iBallSocketConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for creating a Ball and Socket (Point2Point) contraint.
 */

#ifndef _iBallSocketConstraint_HG_
#define _iBallSocketConstraint_HG_

#include "iConstraint.h"

namespace nPhysics {

	class iBallSocketConstraint : public iConstraint {
	public:
		virtual ~iBallSocketConstraint() {}

	protected:
		iBallSocketConstraint() : iConstraint(eConstraintTypes::CONSTRAINT_TYPE_BALLSOCKET) {}
		iBallSocketConstraint(const iBallSocketConstraint& copy) : iConstraint(copy) {}
		iBallSocketConstraint& operator=(const iBallSocketConstraint& copy) { return *this; }
	};
}

#endif // !_iBallSocketConstraint_HG_
