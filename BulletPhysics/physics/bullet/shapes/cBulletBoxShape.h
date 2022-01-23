/**
 * @file cBulletBoxShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will create a box shape in bullet.
 */

#ifndef _cBulletBoxShape_HG_
#define _cBulletBoxShape_HG_

#include <gameMath.h>
#include <physics/interfaces/iBoxShape.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

namespace nPhysics {

	class cBulletBoxShape : public iBoxShape {
	public:
		cBulletBoxShape(glm::vec3 halfExtents);
		virtual ~cBulletBoxShape();

		inline btBoxShape* getShape() {

			return this->mBoxShape;
		}

	private:
		btBoxShape* mBoxShape;
	};
}

#endif // !_cBulletBoxShape_HG_

