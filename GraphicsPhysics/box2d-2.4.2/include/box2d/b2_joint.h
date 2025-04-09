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

#ifndef B2_JOINT_H
#define B2_JOINT_H

#include "b2_api.h"
#include "b2_math.h"

class b2Body;
class b2Draw;
class b2Joint;
struct b2SolverData;
class b2BlockAllocator;

enum b2JointType
{
	e_unknownJoint,
	e_revoluteJoint,
	e_prismaticJoint,
	e_distanceJoint,
	e_pulleyJoint,
	e_mouseJoint,
	e_gearJoint,
	e_wheelJoint,
    e_weldJoint,
	e_frictionJoint,
	e_motorJoint
};

struct B2_API b2Jacobian
{
	b2Vec2 linear;
	float angularA;
	float angularB;
};

/// A joint edge is used to connect bodies and joints together
/// in a joint graph where each body is a node and each joint
/// is an edge. A joint edge belongs to a doubly linked list
/// maintained in each attached body. Each joint has two joint
/// nodes, one for each attached body.
/// 连接边用于连接主体和接头
/// 在一个关节图中，每个主体是一个节点，每个关节
/// 是一条边。一个联合边属于一个双链表
/// 在每个附加的主体中维护。每个关节有两个关节
/// 每个节点对应一个附加的主体。
struct B2_API b2JointEdge
{
	b2Body* other;			///< provides quick access to the other body attached.
	b2Joint* joint;			///< the joint
	b2JointEdge* prev;		///< the previous joint edge in the body's joint list
	b2JointEdge* next;		///< the next joint edge in the body's joint list
};

/// Joint definitions are used to construct joints.
/// 关节定义用于构造关节。
struct B2_API b2JointDef
{
	b2JointDef()
	{
		type = e_unknownJoint;
		bodyA = nullptr;
		bodyB = nullptr;
		collideConnected = false;
	}

	/// The joint type is set automatically for concrete joint types.
    /// 根据具体的接缝类型自动设置接缝类型。
	b2JointType type;

	/// Use this to attach application specific data to your joints.
    /// 使用这个将应用程序特定的数据附加到您的关节。
	b2JointUserData userData;

	/// The first attached body.
    /// 第一附着体。
	b2Body* bodyA;

	/// The second attached body.
    /// 第二附着体。
	b2Body* bodyB;

	/// Set this flag to true if the attached bodies should collide.
    /// 如果附加的物体发生碰撞，将此标志设置为true。
	bool collideConnected;
};

/// Utility to compute linear stiffness values from frequency and damping ratio.
/// 实用程序计算线性刚度值从频率和阻尼比。
B2_API void b2LinearStiffness(float& stiffness, float& damping,
	float frequencyHertz, float dampingRatio,
	const b2Body* bodyA, const b2Body* bodyB);

/// Utility to compute rotational stiffness values frequency and damping ratio
/// 实用程序计算旋转刚度值，频率和阻尼比
B2_API void b2AngularStiffness(float& stiffness, float& damping,
	float frequencyHertz, float dampingRatio,
	const b2Body* bodyA, const b2Body* bodyB);

/// The base joint class. Joints are used to constraint two bodies together in
/// various fashions. Some joints also feature limits and motors.
/// 基关节类。关节用于以各种方式将两个物体约束在一起。一些接头还具有限制和电机。
class B2_API b2Joint
{
public:

	/// Get the type of the concrete joint.
    /// 关节类型。
	b2JointType GetType() const;

	/// Get the first body attached to this joint.
    /// 链接的第一个物体。
	b2Body* GetBodyA();

	/// Get the second body attached to this joint.
    /// 链接的第二个物体。
	b2Body* GetBodyB();

	/// Get the anchor point on bodyA in world coordinates.
    /// 在世界坐标中获取bodyA上的锚点。
	virtual b2Vec2 GetAnchorA() const = 0;

	/// Get the anchor point on bodyB in world coordinates.
    /// 在世界坐标中获取bodyB上的锚点。
	virtual b2Vec2 GetAnchorB() const = 0;

	/// Get the reaction force on bodyB at the joint anchor in Newtons.
    /// 得到物体b在关节锚处的反作用力，单位是牛顿。
	virtual b2Vec2 GetReactionForce(float inv_dt) const = 0;

	/// Get the reaction torque on bodyB in N*m.
    /// 得到物体b上N*m的反作用力。
	virtual float GetReactionTorque(float inv_dt) const = 0;

	/// Get the next joint the world joint list.
    /// 得到下一个关节，世界关节列表。
	b2Joint* GetNext();
	const b2Joint* GetNext() const;

	/// Get the user data pointer.
    /// 获取用户数据指针。
	b2JointUserData& GetUserData();
	const b2JointUserData& GetUserData() const;

	/// Short-cut function to determine if either body is enabled.
    /// 用于确定是否启用任何主体的快捷功能。
	bool IsEnabled() const;

	/// Get collide connected.
	/// Note: modifying the collide connect flag won't work correctly because
	/// the flag is only checked when fixture AABBs begin to overlap.
    /// 得到碰撞连接。
    /// 注意：修改碰撞连接标志将无法正常工作，因为该标志仅在夹具aabb开始重叠时才被检查。
	bool GetCollideConnected() const;

	/// Dump this joint to the log file.
    /// 将该接头转储到日志文件中。
	virtual void Dump() { b2Dump("// Dump is not supported for this joint type.\n"); }

	/// Shift the origin for any points stored in world coordinates.
    /// 移动存储在世界坐标中的任何点的原点。
	virtual void ShiftOrigin(const b2Vec2& newOrigin) { B2_NOT_USED(newOrigin);  }

	/// Debug draw this joint
    /// 调试绘制这个接头
	virtual void Draw(b2Draw* draw) const;

protected:
	friend class b2World;
	friend class b2Body;
	friend class b2Island;
	friend class b2GearJoint;

	static b2Joint* Create(const b2JointDef* def, b2BlockAllocator* allocator);
	static void Destroy(b2Joint* joint, b2BlockAllocator* allocator);

	b2Joint(const b2JointDef* def);
	virtual ~b2Joint() {}

	virtual void InitVelocityConstraints(const b2SolverData& data) = 0;
	virtual void SolveVelocityConstraints(const b2SolverData& data) = 0;

	// This returns true if the position errors are within tolerance.
	virtual bool SolvePositionConstraints(const b2SolverData& data) = 0;

	b2JointType m_type;
	b2Joint* m_prev;
	b2Joint* m_next;
	b2JointEdge m_edgeA;
	b2JointEdge m_edgeB;
	b2Body* m_bodyA;
	b2Body* m_bodyB;

	int32 m_index;

	bool m_islandFlag;
	bool m_collideConnected;

	b2JointUserData m_userData;
};

inline b2JointType b2Joint::GetType() const
{
	return m_type;
}

inline b2Body* b2Joint::GetBodyA()
{
	return m_bodyA;
}

inline b2Body* b2Joint::GetBodyB()
{
	return m_bodyB;
}

inline b2Joint* b2Joint::GetNext()
{
	return m_next;
}

inline const b2Joint* b2Joint::GetNext() const
{
	return m_next;
}

inline b2JointUserData& b2Joint::GetUserData()
{
	return m_userData;
}

inline const b2JointUserData& b2Joint::GetUserData() const
{
	return m_userData;
}

inline bool b2Joint::GetCollideConnected() const
{
	return m_collideConnected;
}

#endif
