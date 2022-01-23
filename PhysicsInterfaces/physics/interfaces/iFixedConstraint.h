/**
 * @file iFixedConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class is the interface for the fixed constraint.
 */

#ifndef _iFixedConstraint_HG_
#define _iFixedConstraint_HG_

#include "iConstraint.h"

namespace nPhysics {

	class iFixedConstraint : public iConstraint {
	public:
		virtual ~iFixedConstraint() {}

	protected:
		iFixedConstraint() : iConstraint(eConstraintTypes::CONSTRAINT_TYPE_FIXED) {}
		iFixedConstraint(const iFixedConstraint& copy) : iConstraint(copy) {}
		iFixedConstraint& operator=(const iFixedConstraint& copy) { return *this; }
	};
}

#endif // !_iFixedConstraint_HG_

