/**
 * @file iConeShape.hpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This will be the interface for the cone shape.
 */

#ifndef _iConeShape_HG_
#define _iConeShape_HG_

#include "iShape.h"

namespace nPhysics {

	class iConeShape : public iShape {
	public:
		virtual ~iConeShape() {}
		virtual float getRadius() = 0;
		virtual float getHeight() = 0;

	protected:
		iConeShape() : iShape(eShapeType::SHAPE_TYPE_CONE) {}
		iConeShape(const iConeShape& copy) : iShape(copy) {}
		iConeShape& operator=(const iConeShape& copy) { return *this; }
	};
}

#endif // !_iConeShape_HG_

