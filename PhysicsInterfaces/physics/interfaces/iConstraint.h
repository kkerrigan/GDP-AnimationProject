/**
 * @file iConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for creating constraints.
 */

#ifndef _iConstraint_HG_
#define _iConstraint_HG_

#include "eConstraintTypes.h"

namespace nPhysics {

	class iConstraint {
	public:
		virtual ~iConstraint() {}

		inline eConstraintTypes getConstraintType() const { return this->mConstraintType; }

	protected:
		iConstraint(eConstraintTypes constraintType) : mConstraintType(constraintType) {}
		iConstraint(const iConstraint& copy) {}
		iConstraint& operator=(const iConstraint& copy) { return *this; }

	private:
		eConstraintTypes mConstraintType;
	};
}

#endif // !_iConstraint_HG_