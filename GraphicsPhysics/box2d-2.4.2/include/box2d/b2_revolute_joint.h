// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef B2_REVOLUTE_JOINT_H
#define B2_REVOLUTE_JOINT_H

#include "b2_api.h"
#include "b2_joint.h"

/// Revolute joint definition. This requires defining an anchor point where the
/// bodies are joined. The definition uses local anchor points so that the
/// initial configuration can violate the constraint slightly. You also need to
/// specify the initial relative angle for joint limits. This helps when saving
/// and loading a game.
/// The local anchor points are measured from the body's origin
/// rather than the center of mass because:
/// 1. you might not know where the center of mass will be.
/// 2. if you add/remove shapes from a body and recompute the mass,
///    the joints will be broken.
/// 转动关节的定义。这需要定义一个锚点
/// 身体是相连的。该定义使用本地锚点，以便
/// 初始配置可能稍微违反约束。你还需要
/// 指定关节极限的初始相对角度。这在储蓄时很有帮助
/// 加载游戏。
/// 局部锚点是从身体原点测量的
/// 而不是质心，因为：
/// 1. 你可能不知道质心在哪里。
/// 2. 如果你从一个物体上添加/删除形状并重新计算质量，
/// 关节会断的。
struct B2_API b2RevoluteJointDef : public b2JointDef
{
	b2RevoluteJointDef()
	{
		type = e_revoluteJoint;
		localAnchorA.Set(0.0f, 0.0f);
		localAnchorB.Set(0.0f, 0.0f);
		referenceAngle = 0.0f;
		lowerAngle = 0.0f;
		upperAngle = 0.0f;
		maxMotorTorque = 0.0f;
		motorSpeed = 0.0f;
		enableLimit = false;
		enableMotor = false;
	}

	/// Initialize the bodies, anchors, and reference angle using a world
	/// anchor point.
    /// 使用world初始化主体、锚点和参考角度
    /// 锚点。
	void Initialize(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor);

	/// The local anchor point relative to bodyA's origin.
    /// 相对于物体a原点的局部锚点。
	b2Vec2 localAnchorA;

	/// The local anchor point relative to bodyB's origin.
    /// 相对于物体b原点的局部锚点。
	b2Vec2 localAnchorB;

	/// The bodyB angle minus bodyA angle in the reference state (radians).
    /// 参考状态下的体b角减去体a角（弧度）。
	float referenceAngle;

	/// A flag to enable joint limits.
    /// 启用联合限制的标志
	bool enableLimit;

	/// The lower angle for the joint limit (radians).
    /// 关节极限的下角（弧度）。
	float lowerAngle;

	/// The upper angle for the joint limit (radians).
    /// 关节极限的上角（弧度）。
	float upperAngle;

	/// A flag to enable the joint motor.
    /// 一个标志，用于启用联合电机。
	bool enableMotor;

	/// The desired motor speed. Usually in radians per second.
    /// 所需的电机速度。通常以弧度每秒为单位。
	float motorSpeed;

	/// The maximum motor torque used to achieve the desired motor speed.
	/// Usually in N-m.
    /// 用于达到所需电机转速的最大电机转矩。
    /// 通常是N-m。
	float maxMotorTorque;
};

/// A revolute joint constrains two bodies to share a common point while they
/// are free to rotate about the point. The relative rotation about the shared
/// point is the joint angle. You can limit the relative rotation with
/// a joint limit that specifies a lower and upper angle. You can use a motor
/// to drive the relative rotation about the shared point. A maximum motor torque
/// is provided so that infinite forces are not generated.
/// 转动关节约束两个物体在运动时共用一个点
/// 绕点自由旋转。围绕共享点的相对旋转
/// 点是关节角。可以用来限制相对旋转
/// 指定上下角的关节极限。你可以用马达
/// 驱动围绕共享点的相对旋转。电机最大转矩
/// 这样就不会产生无限的力。
class B2_API b2RevoluteJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const override;
	b2Vec2 GetAnchorB() const override;

	/// The local anchor point relative to bodyA's origin.
    /// 相对于物体a原点的局部锚点。
	const b2Vec2& GetLocalAnchorA() const { return m_localAnchorA; }

	/// The local anchor point relative to bodyB's origin.
    /// 相对于物体b原点的局部锚点。
	const b2Vec2& GetLocalAnchorB() const  { return m_localAnchorB; }

	/// Get the reference angle.
    /// 得到参考角度。
	float GetReferenceAngle() const { return m_referenceAngle; }

	/// Get the current joint angle in radians.
    /// 得到以弧度为单位的当前关节角。
	float GetJointAngle() const;

	/// Get the current joint angle speed in radians per second.
    /// 得到当前关节的角速度，单位是弧度/秒。
	float GetJointSpeed() const;

	/// Is the joint limit enabled?
    /// 是否启用了联合限制？
	bool IsLimitEnabled() const;

	/// Enable/disable the joint limit.
    /// 启用/禁用联合限制。
	void EnableLimit(bool flag);

	/// Get the lower joint limit in radians.
    /// 得到以弧度为单位的关节下限。
	float GetLowerLimit() const;

	/// Get the upper joint limit in radians.
    /// 得到以弧度为单位的关节上限。
	float GetUpperLimit() const;

	/// Set the joint limits in radians.
    /// 以弧度为单位设置关节限制。
	void SetLimits(float lower, float upper);

	/// Is the joint motor enabled?
    /// 关节电机是否开启？
	bool IsMotorEnabled() const;

	/// Enable/disable the joint motor.
    /// 启用/禁用联合电机。
	void EnableMotor(bool flag);

	/// Set the motor speed in radians per second.
    /// 以弧度/秒为单位设置电机速度。
	void SetMotorSpeed(float speed);

	/// Get the motor speed in radians per second.
    /// 得到以弧度/秒为单位的电机速度。
	float GetMotorSpeed() const;

	/// Set the maximum motor torque, usually in N-m.
    /// 设定电机最大转矩，通常以N-m为单位。
	void SetMaxMotorTorque(float torque);
	float GetMaxMotorTorque() const { return m_maxMotorTorque; }

	/// Get the reaction force given the inverse time step.
	/// Unit is N.
    /// 得到反作用力给定逆时间步长。
	b2Vec2 GetReactionForce(float inv_dt) const override;

	/// Get the reaction torque due to the joint limit given the inverse time step.
	/// Unit is N*m.
    /// 给定逆时间步长，得到由关节极限引起的反作用力。
	float GetReactionTorque(float inv_dt) const override;

	/// Get the current motor torque given the inverse time step.
	/// Unit is N*m.
    /// 得到给定逆时间步长的当前电机转矩。
	float GetMotorTorque(float inv_dt) const;

	/// Dump to b2Log.
	void Dump() override;

	///
	void Draw(b2Draw* draw) const override;

protected:

	friend class b2Joint;
	friend class b2GearJoint;

	b2RevoluteJoint(const b2RevoluteJointDef* def);

	void InitVelocityConstraints(const b2SolverData& data) override;
	void SolveVelocityConstraints(const b2SolverData& data) override;
	bool SolvePositionConstraints(const b2SolverData& data) override;

	// Solver shared
	b2Vec2 m_localAnchorA;
	b2Vec2 m_localAnchorB;
	b2Vec2 m_impulse;
	float m_motorImpulse;
	float m_lowerImpulse;
	float m_upperImpulse;
	bool m_enableMotor;
	float m_maxMotorTorque;
	float m_motorSpeed;
	bool m_enableLimit;
	float m_referenceAngle;
	float m_lowerAngle;
	float m_upperAngle;

	// Solver temp
	int32 m_indexA;
	int32 m_indexB;
	b2Vec2 m_rA;
	b2Vec2 m_rB;
	b2Vec2 m_localCenterA;
	b2Vec2 m_localCenterB;
	float m_invMassA;
	float m_invMassB;
	float m_invIA;
	float m_invIB;
	b2Mat22 m_K;
	float m_angle;
	float m_axialMass;
};

inline float b2RevoluteJoint::GetMotorSpeed() const
{
	return m_motorSpeed;
}

#endif
