/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _shapeTypes_HG_
#define _shapeTypes_HG_

namespace nPhysics {

	enum eShapeType {
		SHAPE_TYPE_PLANE = 0,
		SHAPE_TYPE_SPHERE = 1,
		SHAPE_TYPE_BOX = 2,
		SHAPE_TYPE_CONE = 3,
		SHAPE_TYPE_CAPSULE = 4,
		SHAPE_TYPE_CYLINDER = 5
	};
}

#endif