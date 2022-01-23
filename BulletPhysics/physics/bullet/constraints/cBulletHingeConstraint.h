/**
 * @file cBulletHingeConstraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will allow you to create a Hinge constraint in Bullet.
 */

#ifndef _cBulletHingeConstraint_HG_
#define _cBulletHingeConstraint_HG_

#include <physics/interfaces/iHingeConstraint.h>
#include <physics/defs/sHingeConstraintDef.h>
#include <physics/interfaces/iRigidBody.h>

#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>

namespace nPhysics {

	class cBulletHingeConstraint : public iHingeConstraint {
	public:
		cBulletHingeConstraint(const sHingeConstraintDef& def);
		virtual ~cBulletHingeConstraint();

		inline btHingeConstraint* getConstraint() {

			return this->mConstraint;
		}

	private:
		btHingeConstraint* mConstraint;
	};
}

#endif // !_cBulletHingeConstraint_HG_
