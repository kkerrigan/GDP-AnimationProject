/**
 * @file iBoxShape.hpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * The interface for a Box shape.
 */

#ifndef _iBoxShape_HG_
#define _iBoxShape_HG_

#include "iShape.h"

namespace nPhysics {

	class iBoxShape : public iShape {
	public:
		virtual ~iBoxShape() {}

	protected:
		iBoxShape() : iShape(eShapeType::SHAPE_TYPE_BOX) {}
		iBoxShape(const iBoxShape& copy) : iShape(copy) {}
		iBoxShape& operator=(const iBoxShape& copy) { return *this; }
	};
}

#endif // !_iBoxShape_HG_

