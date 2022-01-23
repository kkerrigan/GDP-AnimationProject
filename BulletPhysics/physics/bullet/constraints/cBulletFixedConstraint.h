/**
 * @file cBulletFixedConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a Fixed constraint in Bullet.
 */

#ifndef _cBulletFixedConstraint_HG_
#define _cBulletFixedConstraint_HG_

#include <physics/interfaces/iFixedConstraint.h>
#include <physics/defs/sFixedConstraintDef.h>
#include <physics/interfaces/iRigidBody.h>

#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>

namespace nPhysics {

	class cBulletFixedConstraint : public iFixedConstraint {
	public:
		cBulletFixedConstraint(const sFixedConstraintDef& def);
		virtual ~cBulletFixedConstraint();

		inline btFixedConstraint* getConstraint() {

			return this->mConstraint;
		}

	private:
		btFixedConstraint* mConstraint;
	};
}

#endif // !_cBulletFixedConstraint_HG_

