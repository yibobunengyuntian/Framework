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

#ifndef B2_WORLD_H
#define B2_WORLD_H

#include "b2_api.h"
#include "b2_block_allocator.h"
#include "b2_contact_manager.h"
#include "b2_math.h"
#include "b2_stack_allocator.h"
#include "b2_time_step.h"
#include "b2_world_callbacks.h"

struct b2AABB;
struct b2BodyDef;
struct b2Color;
struct b2JointDef;
class b2Body;
class b2Draw;
class b2Fixture;
class b2Joint;

/// The world class manages all physics entities, dynamic simulation,
/// and asynchronous queries. The world also contains efficient memory
/// management facilities.
/// 世界一流的管理所有物理实体，动态模拟，
/// 以及异步查询。这个世界也有高效的记忆
/// 管理设施。
class B2_API b2World
{
public:
	/// Construct a world object.
	/// @param gravity the world gravity vector.
    /// 构造一个世界对象。
    /// @param gravity世界重力矢量。
	b2World(const b2Vec2& gravity);

	/// Destruct the world. All physics entities are destroyed and all heap memory is released.
    /// 毁灭世界。所有物理实体被销毁，所有堆内存被释放。
	~b2World();

	/// Register a destruction listener. The listener is owned by you and must
	/// remain in scope.
    /// 注册一个销毁侦听器。听众是你的，而且必须是你的
    /// 保持在范围内。
	void SetDestructionListener(b2DestructionListener* listener);

	/// Register a contact filter to provide specific control over collision.
	/// Otherwise the default filter is used (b2_defaultFilter). The listener is
	/// owned by you and must remain in scope.
    /// 注册一个接触过滤器以提供对碰撞的特定控制。
    /// 否则使用默认过滤器（b2_defaultFilter）。听众是
    /// 归你所有，必须保持在范围内。
	void SetContactFilter(b2ContactFilter* filter);

	/// Register a contact event listener. The listener is owned by you and must
	/// remain in scope.
    /// 注册一个联系人事件监听器。听众是你的，而且必须是你的
    /// 保持在范围内。
	void SetContactListener(b2ContactListener* listener);

	/// Register a routine for debug drawing. The debug draw functions are called
	/// inside with b2World::DebugDraw method. The debug draw object is owned
	/// by you and must remain in scope.
    /// 为调试绘图注册一个例程。调用调试绘制函数
    /// 内部与b2World::DebugDraw方法。调试绘制对象是私有的
    /// 由你和必须留在范围内。
	void SetDebugDraw(b2Draw* debugDraw);

	/// Create a rigid body given a definition. No reference to the definition
	/// is retained.
	/// @warning This function is locked during callbacks.
    /// 创建一个给定定义的刚体。没有提到定义
    /// 是保留。
    /// 该函数在回调期间被锁定。
	b2Body* CreateBody(const b2BodyDef* def);

	/// Destroy a rigid body given a definition. No reference to the definition
	/// is retained. This function is locked during callbacks.
	/// @warning This automatically deletes all associated shapes and joints.
	/// @warning This function is locked during callbacks.
    /// 破坏一个给定定义的刚体。没有提到定义
    /// 是保留。这个函数在回调期间被锁定。
    /// @warning自动删除所有关联的形状和关节。
    /// 该函数在回调期间被锁定。
	void DestroyBody(b2Body* body);

	/// Create a joint to constrain bodies together. No reference to the definition
	/// is retained. This may cause the connected bodies to cease colliding.
	/// @warning This function is locked during callbacks.
    /// 创建一个关节将物体约束在一起。没有提到定义
    /// 是保留。这可能导致连接的物体停止碰撞。
    /// 该函数在回调期间被锁定。
	b2Joint* CreateJoint(const b2JointDef* def);

	/// Destroy a joint. This may cause the connected bodies to begin colliding.
	/// @warning This function is locked during callbacks.
    /// 破坏一个关节。这可能导致连接的物体开始碰撞。
    /// 该函数在回调期间被锁定。
	void DestroyJoint(b2Joint* joint);

	/// Take a time step. This performs collision detection, integration,
	/// and constraint solution.
	/// @param timeStep the amount of time to simulate, this should not vary.
	/// @param velocityIterations for the velocity constraint solver.
	/// @param positionIterations for the position constraint solver.
    /// 采取时间步骤。它执行碰撞检测，集成，
    /// 约束解。
    /// @param timeStep模拟的时间量，这个不应该变化。
    /// @param velocityIterations用于速度约束求解器。
    /// @param positionIterations用于位置约束求解器。
	void Step(	float timeStep,
				int32 velocityIterations,
				int32 positionIterations);

	/// Manually clear the force buffer on all bodies. By default, forces are cleared automatically
	/// after each call to Step. The default behavior is modified by calling SetAutoClearForces.
	/// The purpose of this function is to support sub-stepping. Sub-stepping is often used to maintain
	/// a fixed sized time step under a variable frame-rate.
	/// When you perform sub-stepping you will disable auto clearing of forces and instead call
	/// ClearForces after all sub-steps are complete in one pass of your game loop.
	/// @see SetAutoClearForces
    /// 手动清除所有物体上的力缓冲。缺省情况下，自动清除强制
    /// 在每次调用Step之后。默认行为是通过调用SetAutoClearForces来修改的。
    /// 这个函数的目的是支持子步进。子步骤通常用于维护
    /// 可变帧率下的固定大小的时间步长。
    /// 当您执行子步进时，您将禁用自动清除力，而是调用
    /// 在你的游戏循环中完成所有的子步骤后清理力量。
    /// @see SetAutoClearForces
	void ClearForces();

	/// Call this to draw shapes and other debug draw data. This is intentionally non-const.
    /// 调用它来绘制形状和其他调试绘制数据。这是故意非const的。
	void DebugDraw();

	/// Query the world for all fixtures that potentially overlap the
	/// provided AABB.
	/// @param callback a user implemented callback class.
	/// @param aabb the query box.
    /// 查询所有可能重叠的固定装置的世界
    /// AABB提供。
    /// 一个用户实现的回调类。
    /// @param aabb查询框。
	void QueryAABB(b2QueryCallback* callback, const b2AABB& aabb) const;

	/// Ray-cast the world for all fixtures in the path of the ray. Your callback
	/// controls whether you get the closest point, any point, or n-points.
	/// The ray-cast ignores shapes that contain the starting point.
	/// @param callback a user implemented callback class.
	/// @param point1 the ray starting point
    /// @param point2 the ray ending point.
    /// 对光线路径上的所有固定装置进行光线投射。你的回调
    /// 控制你是得到最近的点，任意点，还是n个点。
    /// 光线投射忽略包含起始点的形状。
    /// 一个用户实现的回调类。
    /// @param point1射线起始点
    /// @param point2射线结束点
	void RayCast(b2RayCastCallback* callback, const b2Vec2& point1, const b2Vec2& point2) const;

	/// Get the world body list. With the returned body, use b2Body::GetNext to get
	/// the next body in the world list. A nullptr body indicates the end of the list.
	/// @return the head of the world body list.
    /// 获取世界机构名单。对于返回的物体，使用b2Body::GetNext来获取
    /// 世界名单上的下一个物体。nullptr体表示列表的结束。
    /// @返回世界机构名单的首位。
	b2Body* GetBodyList();
	const b2Body* GetBodyList() const;

	/// Get the world joint list. With the returned joint, use b2Joint::GetNext to get
	/// the next joint in the world list. A nullptr joint indicates the end of the list.
	/// @return the head of the world joint list.
    /// 获得世界联合名单。对于返回的关节，使用b2Joint::GetNext来获取
    /// 世界名单上的下一个关节。nullptr接头表示列表的结束。
    /// @返回世界联合名单的头部。
	b2Joint* GetJointList();
	const b2Joint* GetJointList() const;

	/// Get the world contact list. With the returned contact, use b2Contact::GetNext to get
	/// the next contact in the world list. A nullptr contact indicates the end of the list.
	/// @return the head of the world contact list.
	/// @warning contacts are created and destroyed in the middle of a time step.
	/// Use b2ContactListener to avoid missing contacts.
    /// 获取全球联系人列表。对于返回的联系人，使用b2Contact::GetNext来获取
    /// 世界名单上的下一个联系人。nullptr联系人表示列表的结束。
    /// @返回世界联系人列表的头部。
    /// @warning联系人在时间步骤的中间被创建和销毁。
    /// 使用b2ContactListener来避免丢失联系人。
	b2Contact* GetContactList();
	const b2Contact* GetContactList() const;

	/// Enable/disable sleep.
    /// 启用/禁用睡眠。
	void SetAllowSleeping(bool flag);
	bool GetAllowSleeping() const { return m_allowSleep; }

	/// Enable/disable warm starting. For testing.
    /// 启用/禁用热启动。进行测试。
	void SetWarmStarting(bool flag) { m_warmStarting = flag; }
	bool GetWarmStarting() const { return m_warmStarting; }

	/// Enable/disable continuous physics. For testing.
    /// 启用/禁用连续物理。进行测试。
	void SetContinuousPhysics(bool flag) { m_continuousPhysics = flag; }
	bool GetContinuousPhysics() const { return m_continuousPhysics; }

	/// Enable/disable single stepped continuous physics. For testing.
    /// 启用/禁用单步连续物理。进行测试。
	void SetSubStepping(bool flag) { m_subStepping = flag; }
	bool GetSubStepping() const { return m_subStepping; }

	/// Get the number of broad-phase proxies.
    /// 获取宽相位代理的数量。
	int32 GetProxyCount() const;

	/// Get the number of bodies.
    /// 得到物体的数量。
	int32 GetBodyCount() const;

	/// Get the number of joints.
    /// 得到关节的数量。
	int32 GetJointCount() const;

	/// Get the number of contacts (each may have 0 or more contact points).
    /// 获取联系人的数量（每个联系人可以有0个或更多的联系人点）。
	int32 GetContactCount() const;

	/// Get the height of the dynamic tree.
    /// 获取动态树的高度。
	int32 GetTreeHeight() const;

	/// Get the balance of the dynamic tree.
    /// 得到动态树的平衡。
	int32 GetTreeBalance() const;

	/// Get the quality metric of the dynamic tree. The smaller the better.
	/// The minimum is 1.
    /// 获得动态树的质量度量。越小越好。
    /// 最小值是1。
	float GetTreeQuality() const;

	/// Change the global gravity vector.
    /// 改变全局重力矢量。
	void SetGravity(const b2Vec2& gravity);

	/// Get the global gravity vector.
    /// 得到全局重力向量。
	b2Vec2 GetGravity() const;

	/// Is the world locked (in the middle of a time step).
    /// 世界是否被锁住了（在时间步骤的中间）？
	bool IsLocked() const;

	/// Set flag to control automatic clearing of forces after each time step.
    /// 设置标志来控制每个时间步后力的自动清除。
	void SetAutoClearForces(bool flag);

	/// Get the flag that controls automatic clearing of forces after each time step.
    /// 获取控制每个时间步后自动清除力的标志。
	bool GetAutoClearForces() const;

	/// Shift the world origin. Useful for large worlds.
	/// The body shift formula is: position -= newOrigin
    /// @param newOrigin the new origin with respect to the old origin.
    /// 移动世界原点。对大的世界很有用。
    /// 身体位移公式为：position -= newOrigin
    /// @param newOrigin新原点相对于旧原点
	void ShiftOrigin(const b2Vec2& newOrigin);

	/// Get the contact manager for testing.
    /// 让联系人管理器进行测试。
	const b2ContactManager& GetContactManager() const;

	/// Get the current profile.
    /// 获取当前配置文件。
	const b2Profile& GetProfile() const;

	/// Dump the world into the log file.
	/// @warning this should be called outside of a time step.
    /// 将世界转储到日志文件中。
    /// @warning应该在时间步长之外调用。
	void Dump();

private:

	friend class b2Body;
	friend class b2Fixture;
	friend class b2ContactManager;
	friend class b2Controller;

	b2World(const b2World&) = delete;
	void operator=(const b2World&) = delete;

	void Solve(const b2TimeStep& step);
	void SolveTOI(const b2TimeStep& step);

	void DrawShape(b2Fixture* shape, const b2Transform& xf, const b2Color& color);

	b2BlockAllocator m_blockAllocator;
	b2StackAllocator m_stackAllocator;

	b2ContactManager m_contactManager;

	b2Body* m_bodyList;
	b2Joint* m_jointList;

	int32 m_bodyCount;
	int32 m_jointCount;

	b2Vec2 m_gravity;
	bool m_allowSleep;

	b2DestructionListener* m_destructionListener;
	b2Draw* m_debugDraw;

	// This is used to compute the time step ratio to
	// support a variable time step.
	float m_inv_dt0;

	bool m_newContacts;
	bool m_locked;
	bool m_clearForces;

	// These are for debugging the solver.
	bool m_warmStarting;
	bool m_continuousPhysics;
	bool m_subStepping;

	bool m_stepComplete;

	b2Profile m_profile;
};

inline b2Body* b2World::GetBodyList()
{
	return m_bodyList;
}

inline const b2Body* b2World::GetBodyList() const
{
	return m_bodyList;
}

inline b2Joint* b2World::GetJointList()
{
	return m_jointList;
}

inline const b2Joint* b2World::GetJointList() const
{
	return m_jointList;
}

inline b2Contact* b2World::GetContactList()
{
	return m_contactManager.m_contactList;
}

inline const b2Contact* b2World::GetContactList() const
{
	return m_contactManager.m_contactList;
}

inline int32 b2World::GetBodyCount() const
{
	return m_bodyCount;
}

inline int32 b2World::GetJointCount() const
{
	return m_jointCount;
}

inline int32 b2World::GetContactCount() const
{
	return m_contactManager.m_contactCount;
}

inline void b2World::SetGravity(const b2Vec2& gravity)
{
	m_gravity = gravity;
}

inline b2Vec2 b2World::GetGravity() const
{
	return m_gravity;
}

inline bool b2World::IsLocked() const
{
	return m_locked;
}

inline void b2World::SetAutoClearForces(bool flag)
{
	m_clearForces = flag;
}

/// Get the flag that controls automatic clearing of forces after each time step.
inline bool b2World::GetAutoClearForces() const
{
	return m_clearForces;
}

inline const b2ContactManager& b2World::GetContactManager() const
{
	return m_contactManager;
}

inline const b2Profile& b2World::GetProfile() const
{
	return m_profile;
}

#endif
