/**
 * @file cBulletConeTwistConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a Cone Twist constraint in Bullet.
 */

#ifndef _cBulletConeTwistConstraint_HG_
#define _cBulletConeTwistConstraint_HG_

#include <physics/interfaces/iConeTwistConstraint.h>
#include <physics/defs/sConeTwistConstraintDef.h>
#include <physics/interfaces/iRigidBody.h>

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>

namespace nPhysics {

	class cBulletConeTwistConstraint : public iConeTwistConstraint {
	public:
		cBulletConeTwistConstraint(const sConeTwistConstraintDef& def);
		virtual ~cBulletConeTwistConstraint();

		inline btConeTwistConstraint* getConstraint() {

			return this->mConstraint;
		}

	private:
		btConeTwistConstraint* mConstraint;
	};
}


#endif // !)cBulletConeTwistConstraint_HG_

