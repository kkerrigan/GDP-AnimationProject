/**
 * @file cBulletSliderConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a Slider constraint in Bullet.
 */

#ifndef _cBulletSliderConstraint_HG_
#define _cBulletSliderConstraint_HG_

#include <physics/interfaces/iSliderConstraint.h>
#include <physics/defs/sSliderConstraintDef.h>
#include <physics/interfaces/iRigidBody.h>

#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

namespace nPhysics {

	class cBulletSliderConstraint : public iSliderConstraint {
	public:
		cBulletSliderConstraint(const sSliderConstraintDef& def);
		virtual ~cBulletSliderConstraint();

		inline btSliderConstraint* getConstraint() {

			return this->mConstraint;
		}

	private:
		btSliderConstraint* mConstraint;
	};
}

#endif // !_cBulletSliderConstraint_HG_
