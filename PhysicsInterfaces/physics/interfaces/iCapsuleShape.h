/**
 * @file iCapsuleShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will be the interface for creating a capsule shape.
 */

#ifndef _iCapsuleShape_HG_
#define _iCapsuleShape_HG_

#include "iShape.h"

namespace nPhysics {

	class iCapsuleShape : public iShape {
	public:
		virtual ~iCapsuleShape() {}
		virtual float getRadius() = 0;
		virtual float getHeight() = 0;

	protected:
		iCapsuleShape() : iShape(eShapeType::SHAPE_TYPE_CAPSULE) {}
		iCapsuleShape(const iCapsuleShape& copy) : iShape(copy) {}
		iCapsuleShape& operator=(const iCapsuleShape& copy) { return *this; }
	};
}

#endif // !_iCapsuleShape_HG_
