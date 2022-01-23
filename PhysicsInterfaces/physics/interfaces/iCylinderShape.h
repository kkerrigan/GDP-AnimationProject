/**
 * @file iCylinderShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for creating cylinders.
 */

#ifndef _iCylinderShape_HG_
#define _iCylinderShape_HG_

#include "iShape.h"

namespace nPhysics {

	class iCylinderShape : public iShape {
	public:
		virtual ~iCylinderShape() {}

	protected:
		iCylinderShape() : iShape(eShapeType::SHAPE_TYPE_CYLINDER) {}
		iCylinderShape(const iCylinderShape& copy) : iShape(copy) {}
		iCylinderShape& operator=(const iCylinderShape& copy) { return *this; }
	};
}

#endif // !_iCylinderShape_HG_
