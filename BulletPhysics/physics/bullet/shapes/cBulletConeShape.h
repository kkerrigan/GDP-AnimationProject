/**
 * @file cBulletConeShape.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a cone shape in Bullet.
 */

#ifndef _cBulletConeShape_HG_
#define _cBulletConeShape_HG_

#include <physics/interfaces/iConeShape.h>

#include <BulletCollision/CollisionShapes/btConeShape.h>

namespace nPhysics {

	class cBulletConeShape : public iConeShape {
	public:
		cBulletConeShape(float radius, float height);
		virtual ~cBulletConeShape();

		virtual float getRadius() override;
		virtual float getHeight() override;

		inline btConeShape* getShape() {

			return this->mShape;
		}

	private:
		btConeShape* mShape;
	};
}

#endif // !_cBulletConeShape_HG_

