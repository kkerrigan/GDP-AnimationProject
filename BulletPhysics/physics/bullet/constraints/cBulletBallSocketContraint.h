/**
 * @file cBulletBallSocketContraint.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will let you create a Ball and Socket (Point2Point) contraint in Bullet.
 */

#ifndef _cBulletBallSocketContraint_HG_
#define _cBulletBallSocketContraint_HG_

#include <gameMath.h>

#include <physics/interfaces/iBallSocketConstraint.h>
#include <physics/defs/sBallSocketConstraintDef.h>
#include <physics/interfaces/iRigidBody.h>

#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>

namespace nPhysics {

	class cBulletBallSocketContraint : public iBallSocketConstraint {
	public:
		cBulletBallSocketContraint(const sBallSocketConstraintDef& def);
		virtual ~cBulletBallSocketContraint();

		inline btPoint2PointConstraint* getConstraint() {

			return this->mConstraint;
		}

	private:
		btPoint2PointConstraint* mConstraint;
	};
}

#endif // !_cBulletBallSocketContraint_HG_

