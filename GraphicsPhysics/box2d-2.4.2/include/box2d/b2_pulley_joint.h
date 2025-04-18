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

#ifndef B2_PULLEY_JOINT_H
#define B2_PULLEY_JOINT_H

#include "b2_api.h"
#include "b2_joint.h"

const float b2_minPulleyLength = 2.0f;

/// Pulley joint definition. This requires two ground anchors,
/// two dynamic body anchor points, and a pulley ratio.
/// 皮带轮接头定义。这需要两个地锚
/// 两个动态体锚点，和一个皮带轮比。
struct B2_API b2PulleyJointDef : public b2JointDef
{
	b2PulleyJointDef()
	{
		type = e_pulleyJoint;
		groundAnchorA.Set(-1.0f, 1.0f);
		groundAnchorB.Set(1.0f, 1.0f);
		localAnchorA.Set(-1.0f, 0.0f);
		localAnchorB.Set(1.0f, 0.0f);
		lengthA = 0.0f;
		lengthB = 0.0f;
		ratio = 1.0f;
		collideConnected = true;
	}

	/// Initialize the bodies, anchors, lengths, max lengths, and ratio using the world anchors.
    /// 使用世界锚初始化主体、锚、长度、最大长度和比率。
	void Initialize(b2Body* bodyA, b2Body* bodyB,
					const b2Vec2& groundAnchorA, const b2Vec2& groundAnchorB,
					const b2Vec2& anchorA, const b2Vec2& anchorB,
					float ratio);

	/// The first ground anchor in world coordinates. This point never moves.
    /// 世界坐标中的第一个地锚。这个点不动。
	b2Vec2 groundAnchorA;

	/// The second ground anchor in world coordinates. This point never moves.
    /// 世界坐标中的第二个地锚。这个点不动。
	b2Vec2 groundAnchorB;

	/// The local anchor point relative to bodyA's origin.
    /// 相对于物体a原点的局部锚点。
	b2Vec2 localAnchorA;

	/// The local anchor point relative to bodyB's origin.
    /// 相对于物体b原点的局部锚点。
	b2Vec2 localAnchorB;

	/// The a reference length for the segment attached to bodyA.
    /// 附在主体a上的线段的参考长度。
	float lengthA;

	/// The a reference length for the segment attached to bodyB.
    /// 附在主体b上的线段的参考长度。
	float lengthB;

	/// The pulley ratio, used to simulate a block-and-tackle.
    /// 滑轮比，用于模拟滑车和滑车。
	float ratio;
};

/// The pulley joint is connected to two bodies and two fixed ground points.
/// The pulley supports a ratio such that:
/// length1 + ratio * length2 <= constant
/// Yes, the force transmitted is scaled by the ratio.
/// Warning: the pulley joint can get a bit squirrelly by itself. They often
/// work better when combined with prismatic joints. You should also cover the
/// the anchor points with static shapes to prevent one side from going to
/// zero length.
/// 滑轮接头连接两个主体和两个固定接地点。
/// 滑轮支撑的比率如下：
/// 长度1 +比率*长度2 <=常数
/// 是的，传递的力是按比例缩放的。
/// 警告：皮带轮接头本身可能会有点不正常。他们经常
/// 与移动关节结合使用效果更好。你也应该覆盖
/// 锚点与静态形状，以防止一侧去
/// 零长度。
class B2_API b2PulleyJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const override;
	b2Vec2 GetAnchorB() const override;

	b2Vec2 GetReactionForce(float inv_dt) const override;
	float GetReactionTorque(float inv_dt) const override;

	/// Get the first ground anchor.
    /// 找第一个地锚。
	b2Vec2 GetGroundAnchorA() const;

	/// Get the second ground anchor.
    /// 找第二个地锚。
	b2Vec2 GetGroundAnchorB() const;

	/// Get the current length of the segment attached to bodyA.
    /// 获取连接到bodyA的线段的当前长度。
	float GetLengthA() const;

	/// Get the current length of the segment attached to bodyB.
    /// 获取连接到bodyB的线段的当前长度。
	float GetLengthB() const;

	/// Get the pulley ratio.
    /// 得到滑轮比。
	float GetRatio() const;

	/// Get the current length of the segment attached to bodyA.
    /// 获取连接到bodyA的线段的当前长度。
	float GetCurrentLengthA() const;

	/// Get the current length of the segment attached to bodyB.
    /// 获取连接到bodyB的线段的当前长度。
	float GetCurrentLengthB() const;

	/// Dump joint to dmLog
	void Dump() override;

	/// Implement b2Joint::ShiftOrigin
	void ShiftOrigin(const b2Vec2& newOrigin) override;

protected:

	friend class b2Joint;
	b2PulleyJoint(const b2PulleyJointDef* data);

	void InitVelocityConstraints(const b2SolverData& data) override;
	void SolveVelocityConstraints(const b2SolverData& data) override;
	bool SolvePositionConstraints(const b2SolverData& data) override;

	b2Vec2 m_groundAnchorA;
	b2Vec2 m_groundAnchorB;
	float m_lengthA;
	float m_lengthB;

	// Solver shared
	b2Vec2 m_localAnchorA;
	b2Vec2 m_localAnchorB;
	float m_constant;
	float m_ratio;
	float m_impulse;

	// Solver temp
	int32 m_indexA;
	int32 m_indexB;
	b2Vec2 m_uA;
	b2Vec2 m_uB;
	b2Vec2 m_rA;
	b2Vec2 m_rB;
	b2Vec2 m_localCenterA;
	b2Vec2 m_localCenterB;
	float m_invMassA;
	float m_invMassB;
	float m_invIA;
	float m_invIB;
	float m_mass;
};

#endif
