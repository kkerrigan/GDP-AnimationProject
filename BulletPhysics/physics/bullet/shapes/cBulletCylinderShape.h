/**
 * @file cBulletCylinderShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a cylinder shape in Bullet.
 */

#ifndef _cBulletCylinderShape_HG_
#define _cBulletCylinderShape_HG_

#include <gameMath.h>
#include <physics/interfaces/iCylinderShape.h>

#include <BulletCollision/CollisionShapes/btCylinderShape.h>

namespace nPhysics {

	class cBulletCylinderShape : public iCylinderShape {
	public:
		cBulletCylinderShape(const glm::vec3& halfExtents);
		virtual ~cBulletCylinderShape();

		inline btCylinderShape* getShape() {

			return this->mShape;
		}

	private:
		btCylinderShape* mShape;
	};
}

#endif // !_cBulletCylinderShape_HG_
