/**
 * @file iConeTwistConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class is the interface for the Cone Twist Constraint.
 */

#ifndef _iConeTwistConstraint_HG_
#define _iConeTwistConstraint_HG_

#include "iConstraint.h"

namespace nPhysics {

	class iConeTwistConstraint : public iConstraint {
	public:
		virtual ~iConeTwistConstraint() {}

	protected:
		iConeTwistConstraint() : iConstraint(eConstraintTypes::CONSTRAINT_TYPE_CONE_TWIST) {}
		iConeTwistConstraint(const iConeTwistConstraint& copy) : iConstraint(copy) {}
		iConeTwistConstraint& operator=(const iConeTwistConstraint& copy) { return *this; }
	};
}

#endif // !_iConeTwistConstraint_HG_

