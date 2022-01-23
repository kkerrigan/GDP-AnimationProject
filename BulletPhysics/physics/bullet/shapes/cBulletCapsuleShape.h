/**
 * @file cBulletCapsuleShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a capsule shape in Bullet.
 */

#ifndef _cBulletCapsuleShape_HG_
#define _cBulletCapsuleShape_HG_

#include <physics/interfaces/iCapsuleShape.h>

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace nPhysics {

	class cBulletCapsuleShape : public iCapsuleShape {
	public:
		cBulletCapsuleShape(float radius, float height);
		virtual ~cBulletCapsuleShape();

		virtual float getRadius() override;
		virtual float getHeight() override;

		inline btCapsuleShape* getShape() {

			return this->mShape;
		}

	private:
		btCapsuleShape* mShape;
	};
}

#endif // !_cBulletCapsuleShape_HG_
