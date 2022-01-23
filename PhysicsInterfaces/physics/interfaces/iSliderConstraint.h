/**
 * @file iSliderConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for a Slider constraint.
 */

#ifndef _iSliderConstraint_HG_
#define _iSliderConstraint_HG_

#include "iConstraint.h"

namespace nPhysics {

	class iSliderConstraint : public iConstraint {
	public:
		virtual ~iSliderConstraint() {}

	protected:
		iSliderConstraint() : iConstraint(eConstraintTypes::CONSTRAINT_TYPE_SLIDER) {}
		iSliderConstraint(const iSliderConstraint& copy) : iConstraint(copy) {}
		iSliderConstraint& operator=(const iSliderConstraint& copy) { return *this; }
	};
}

#endif // !_iSliderConstraint_HG_

