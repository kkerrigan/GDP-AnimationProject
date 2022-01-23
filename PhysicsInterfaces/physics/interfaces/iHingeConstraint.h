/**
 * @file iHingeConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for creating a Hinge contraint.
 */

#ifndef _iHingeConstraint_HG_
#define _iHingeConstraint_HH_

#include "iConstraint.h"

namespace nPhysics {

	class iHingeConstraint : public iConstraint {
	public:
		virtual ~iHingeConstraint() {}

	protected:
		iHingeConstraint() : iConstraint(eConstraintTypes::CONSTRAINT_TYPE_HINGE) {}
		iHingeConstraint(const iHingeConstraint& copy) : iConstraint(copy) {}
		iHingeConstraint& operator=(const iHingeConstraint& copy) { return *this; }
	};
}

#endif // !_iHingeConstraint_HG_
