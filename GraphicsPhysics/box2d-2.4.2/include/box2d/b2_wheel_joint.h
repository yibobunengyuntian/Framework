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

#ifndef B2_WHEEL_JOINT_H
#define B2_WHEEL_JOINT_H

#include "b2_api.h"
#include "b2_joint.h"

/// Wheel joint definition. This requires defining a line of
/// motion using an axis and an anchor point. The definition uses local
/// anchor points and a local axis so that the initial configuration
/// can violate the constraint slightly. The joint translation is zero
/// when the local anchor points coincide in world space. Using local
/// anchors and a local axis helps when saving and loading a game.
/// 车轮关节定义。这需要定义一行
/// 使用轴和锚点的运动。定义使用local
/// 锚点和局部轴使初始配置
/// 可以稍微违反约束。关节的平动是零
/// 当局部锚点在世界空间重合时。使用本地
/// 锚点和本地轴有助于保存和加载游戏。
struct B2_API b2WheelJointDef : public b2JointDef
{
	b2WheelJointDef()
	{
		type = e_wheelJoint;
		localAnchorA.SetZero();
		localAnchorB.SetZero();
		localAxisA.Set(1.0f, 0.0f);
		enableLimit = false;
		lowerTranslation = 0.0f;
		upperTranslation = 0.0f;
		enableMotor = false;
		maxMotorTorque = 0.0f;
		motorSpeed = 0.0f;
		stiffness = 0.0f;
		damping = 0.0f;
	}

	/// Initialize the bodies, anchors, axis, and reference angle using the world
	/// anchor and world axis.
    /// 使用世界初始化物体、锚点、轴和参考角度
    /// 锚和世界轴。
	void Initialize(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor, const b2Vec2& axis);

	/// The local anchor point relative to bodyA's origin.
    /// 相对于物体a原点的局部锚点。
	b2Vec2 localAnchorA;

	/// The local anchor point relative to bodyB's origin.
    /// 相对于物体b原点的局部锚点。
	b2Vec2 localAnchorB;

	/// The local translation axis in bodyA.
    /// bodyA中的局部平移轴。
	b2Vec2 localAxisA;

	/// Enable/disable the joint limit.
    /// 启用/禁用联合限制。
	bool enableLimit;

	/// The lower translation limit, usually in meters.
    /// 下平移极限，通常以米为单位。
	float lowerTranslation;

	/// The upper translation limit, usually in meters.
    /// 平移上限，通常以米为单位。
	float upperTranslation;

	/// Enable/disable the joint motor.
    /// 启用/禁用联合电机。
	bool enableMotor;

	/// The maximum motor torque, usually in N-m.
    /// 电机的最大转矩，通常以N-m为单位。
	float maxMotorTorque;

	/// The desired motor speed in radians per second.
    /// 所需的电机速度，单位为弧度/秒。
	float motorSpeed;

	/// Suspension stiffness. Typically in units N/m.
    /// 悬架刚度。通常以N/m为单位。
	float stiffness;

	/// Suspension damping. Typically in units of N*s/m.
    /// 悬架阻尼。通常以N*s/m为单位。
	float damping;
};

/// A wheel joint. This joint provides two degrees of freedom: translation
/// along an axis fixed in bodyA and rotation in the plane. In other words, it is a point to
/// line constraint with a rotational motor and a linear spring/damper. The spring/damper is
/// initialized upon creation. This joint is designed for vehicle suspensions.
/// 车轮接头这个关节提供两个自由度：平移
/// 沿着固定在物体a中的轴并在平面上旋转。换句话说，它是指向
/// 带旋转马达和线性弹簧/阻尼器的直线约束。弹簧/阻尼器是
/// 创建时初始化。这种接头是为车辆悬架设计的。
class B2_API b2WheelJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const override;
	b2Vec2 GetAnchorB() const override;

	b2Vec2 GetReactionForce(float inv_dt) const override;
	float GetReactionTorque(float inv_dt) const override;

	/// The local anchor point relative to bodyA's origin.
    /// 相对于物体a原点的局部锚点。
	const b2Vec2& GetLocalAnchorA() const { return m_localAnchorA; }

	/// The local anchor point relative to bodyB's origin.
    /// 相对于物体b原点的局部锚点。
	const b2Vec2& GetLocalAnchorB() const  { return m_localAnchorB; }

	/// The local joint axis relative to bodyA.
    /// 相对于物体a的局部关节轴。
	const b2Vec2& GetLocalAxisA() const { return m_localXAxisA; }

	/// Get the current joint translation, usually in meters.
    /// 得到当前关节的平移，通常以米为单位。
	float GetJointTranslation() const;

	/// Get the current joint linear speed, usually in meters per second.
    /// 得到当前关节的线速度，通常以米每秒为单位。
	float GetJointLinearSpeed() const;

	/// Get the current joint angle in radians.
    /// 得到以弧度为单位的当前关节角。
	float GetJointAngle() const;

	/// Get the current joint angular speed in radians per second.
    /// 得到当前关节的角速度，单位是弧度每秒。
	float GetJointAngularSpeed() const;

	/// Is the joint limit enabled?
    /// 是否启用了联合限制？
	bool IsLimitEnabled() const;

	/// Enable/disable the joint translation limit.
    /// 启用/禁用联合翻译限制。
	void EnableLimit(bool flag);

	/// Get the lower joint translation limit, usually in meters.
    /// 得到关节下平移极限，通常以米为单位。
	float GetLowerLimit() const;

	/// Get the upper joint translation limit, usually in meters.
    /// 得到关节上平移极限，通常以米为单位。
	float GetUpperLimit() const;

	/// Set the joint translation limits, usually in meters.
    /// 设置关节平移极限，通常以米为单位。
	void SetLimits(float lower, float upper);

	/// Is the joint motor enabled?
    /// 关节电机是否开启？
	bool IsMotorEnabled() const;

	/// Enable/disable the joint motor.
    /// 启用/禁用联合电机。
	void EnableMotor(bool flag);

	/// Set the motor speed, usually in radians per second.
    /// 设置电机速度，通常以弧度每秒为单位。
	void SetMotorSpeed(float speed);

	/// Get the motor speed, usually in radians per second.
    /// 获取电机速度，通常以弧度每秒为单位。
	float GetMotorSpeed() const;

	/// Set/Get the maximum motor force, usually in N-m.
    /// 设置/获取最大电机力，通常以N-m为单位。
	void SetMaxMotorTorque(float torque);
	float GetMaxMotorTorque() const;

	/// Get the current motor torque given the inverse time step, usually in N-m.
    /// 给定逆时间步长，通常以N-m为单位，得到当前电机转矩。
	float GetMotorTorque(float inv_dt) const;

	/// Access spring stiffness
    /// 弹簧刚度
	void SetStiffness(float stiffness);
	float GetStiffness() const;

	/// Access damping
    /// 访问阻尼
	void SetDamping(float damping);
	float GetDamping() const;

	/// Dump to b2Log
	void Dump() override;

	///
	void Draw(b2Draw* draw) const override;

protected:

	friend class b2Joint;
	b2WheelJoint(const b2WheelJointDef* def);

	void InitVelocityConstraints(const b2SolverData& data) override;
	void SolveVelocityConstraints(const b2SolverData& data) override;
	bool SolvePositionConstraints(const b2SolverData& data) override;

	b2Vec2 m_localAnchorA;
	b2Vec2 m_localAnchorB;
	b2Vec2 m_localXAxisA;
	b2Vec2 m_localYAxisA;

	float m_impulse;
	float m_motorImpulse;
	float m_springImpulse;

	float m_lowerImpulse;
	float m_upperImpulse;
	float m_translation;
	float m_lowerTranslation;
	float m_upperTranslation;

	float m_maxMotorTorque;
	float m_motorSpeed;

	bool m_enableLimit;
	bool m_enableMotor;

	float m_stiffness;
	float m_damping;

	// Solver temp
	int32 m_indexA;
	int32 m_indexB;
	b2Vec2 m_localCenterA;
	b2Vec2 m_localCenterB;
	float m_invMassA;
	float m_invMassB;
	float m_invIA;
	float m_invIB;

	b2Vec2 m_ax, m_ay;
	float m_sAx, m_sBx;
	float m_sAy, m_sBy;

	float m_mass;
	float m_motorMass;
	float m_axialMass;
	float m_springMass;

	float m_bias;
	float m_gamma;

};

inline float b2WheelJoint::GetMotorSpeed() const
{
	return m_motorSpeed;
}

inline float b2WheelJoint::GetMaxMotorTorque() const
{
	return m_maxMotorTorque;
}

#endif
