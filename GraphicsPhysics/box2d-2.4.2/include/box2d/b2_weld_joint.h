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

#ifndef B2_WELD_JOINT_H
#define B2_WELD_JOINT_H

#include "b2_api.h"
#include "b2_joint.h"

/// Weld joint definition. You need to specify local anchor points
/// where they are attached and the relative body angle. The position
/// of the anchor points is important for computing the reaction torque.
/// 焊接接头定义。您需要指定本地锚点
/// 它们附着的位置和相对的身体角度。这个职位
/// 锚点的选取是计算反力的重要依据。
struct B2_API b2WeldJointDef : public b2JointDef
{
	b2WeldJointDef()
	{
		type = e_weldJoint;
		localAnchorA.Set(0.0f, 0.0f);
		localAnchorB.Set(0.0f, 0.0f);
		referenceAngle = 0.0f;
		stiffness = 0.0f;
		damping = 0.0f;
	}

	/// Initialize the bodies, anchors, reference angle, stiffness, and damping.
	/// @param bodyA the first body connected by this joint
	/// @param bodyB the second body connected by this joint
    /// @param anchor the point of connection in world coordinates.
    /// 初始化物体、锚点、参考角度、刚度和阻尼。
    /// @param bodyA这个关节连接的第一个主体
    /// @param bodyB这个关节连接的第二个主体
    /// @param 在世界坐标中锚定连接点
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

	/// The rotational stiffness in N*m
	/// Disable softness with a value of 0
    /// 转动刚度单位为N*m
    /// 禁用软性，值为0
	float stiffness;

	/// The rotational damping in N*m*s
    /// N*m*s的转动阻尼
	float damping;
};

/// A weld joint essentially glues two bodies together. A weld joint may
/// distort somewhat because the island constraint solver is approximate.
/// 焊接接头实质上是把两个物体粘合在一起。焊接接头可以
/// 由于孤岛约束解算器是近似的，所以会有一些扭曲。
class B2_API b2WeldJoint : public b2Joint
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

	/// Get the reference angle.
    /// 得到参考角度。
	float GetReferenceAngle() const { return m_referenceAngle; }

	/// Set/get stiffness in N*m
    /// 设/取N*m的刚度
	void SetStiffness(float stiffness) { m_stiffness = stiffness; }
	float GetStiffness() const { return m_stiffness; }

	/// Set/get damping in N*m*s
    /// 设置/获取N*m*s的阻尼
	void SetDamping(float damping) { m_damping = damping; }
	float GetDamping() const { return m_damping; }

	/// Dump to b2Log
	void Dump() override;

protected:

	friend class b2Joint;

	b2WeldJoint(const b2WeldJointDef* def);

	void InitVelocityConstraints(const b2SolverData& data) override;
	void SolveVelocityConstraints(const b2SolverData& data) override;
	bool SolvePositionConstraints(const b2SolverData& data) override;

	float m_stiffness;
	float m_damping;
	float m_bias;

	// Solver shared
	b2Vec2 m_localAnchorA;
	b2Vec2 m_localAnchorB;
	float m_referenceAngle;
	float m_gamma;
	b2Vec3 m_impulse;

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
	b2Mat33 m_mass;
};

#endif
