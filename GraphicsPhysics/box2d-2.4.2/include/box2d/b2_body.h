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

#ifndef B2_BODY_H
#define B2_BODY_H

#include "b2_api.h"
#include "b2_math.h"
#include "b2_shape.h"

class b2Fixture;
class b2Joint;
class b2Contact;
class b2Controller;
class b2World;
struct b2FixtureDef;
struct b2JointEdge;
struct b2ContactEdge;

/// 物体类型。
/// static: 零质量，零速度，可手动移动
/// kinematic: 零质量，非零速度由用户设定，由求解器移动
/// dynamic: 正质量，非零速度由力决定，由求解器移动
enum b2BodyType
{
	b2_staticBody = 0,
	b2_kinematicBody,
	b2_dynamicBody
};

/// 物体定义包含构造刚体所需的所有数据。
/// 可以安全地重用体定义。构造完成后，形状被添加到主体中。
struct B2_API b2BodyDef
{
    /// 这个构造函数设置体定义的默认值。
	b2BodyDef()
	{
		position.Set(0.0f, 0.0f);
		angle = 0.0f;
		linearVelocity.Set(0.0f, 0.0f);
		angularVelocity = 0.0f;
		linearDamping = 0.0f;
		angularDamping = 0.0f;
		allowSleep = true;
		awake = true;
		fixedRotation = false;
		bullet = false;
		type = b2_staticBody;
		enabled = true;
		gravityScale = 1.0f;
	}

    /// 物体类型: static, kinematic, or dynamic.
    /// 注：如果一个动态物体的质量为零，则质量设为1。
	b2BodyType type;

    /// 身体的世界位置。避免在原点处创建物体
    /// 因为这会导致许多重叠的形状。
	b2Vec2 position;

    /// 以弧度为单位的物体的世界角。
	float angle;

    /// 物体在世界坐标中原点的线速度。
	b2Vec2 linearVelocity;

    /// 物体的角速度。
	float angularVelocity;

	/// Linear damping is use to reduce the linear velocity. The damping parameter
	/// can be larger than 1.0f but the damping effect becomes sensitive to the
	/// time step when the damping parameter is large.
    /// Units are 1/time.
    /// 线性阻尼是用来降低线速度的。阻尼参数
    /// 可大于1.0f，但阻尼效应对
    /// 阻尼参数较大时的时间步长。
    /// 单位是1/时间
	float linearDamping;

	/// Angular damping is use to reduce the angular velocity. The damping parameter
	/// can be larger than 1.0f but the damping effect becomes sensitive to the
	/// time step when the damping parameter is large.
    /// Units are 1/time.
    /// 角阻尼用于降低角速度。阻尼参数
    /// 可大于1.0f，但阻尼效应对
    /// 阻尼参数较大时的时间步长。
    /// 单位是1/时间
	float angularDamping;

	/// Set this flag to false if this body should never fall asleep. Note that
	/// this increases CPU usage.
    /// 如果此body不应该休眠，则将此标志设置为false。请注意,
    /// 这会增加CPU使用率。
	bool allowSleep;

    /// Is this body initially awake or sleeping?
    /// 这个身体最初是醒着的还是睡着的？
	bool awake;

	/// Should this body be prevented from rotating? Useful for characters.
    /// 应该阻止这个机构旋转吗？对字符很有用。
	bool fixedRotation;

	/// Is this a fast moving body that should be prevented from tunneling through
	/// other moving bodies? Note that all bodies are prevented from tunneling through
	/// kinematic and static bodies. This setting is only considered on dynamic bodies.
	/// @warning You should use this flag sparingly since it increases processing time.
    /// 这是一个快速移动的物体，应该阻止它穿过隧道吗
    /// 其他移动的物体？请注意，所有的物体都被阻止穿过隧道
    /// 运动学和静力体。这个设置只在动态物体上考虑。
    /// 您应该谨慎使用此标志，因为它会增加处理时间。
	bool bullet;

	/// Does this body start out enabled?
    /// 这个主体一开始是启用的吗？
	bool enabled;

	/// Use this to store application specific body data.
    /// 使用它来存储特定于应用程序的主体数据。
	b2BodyUserData userData;

	/// Scale the gravity applied to this body.
    /// 测量作用在这个物体上的重力。
	float gravityScale;
};

/// A rigid body. These are created via b2World::CreateBody.
/// 刚体。这些是通过b2World::CreateBody创建的。
class B2_API b2Body
{
public:
	/// Creates a fixture and attach it to this body. Use this function if you need
	/// to set some fixture parameters, like friction. Otherwise you can create the
	/// fixture directly from a shape.
	/// If the density is non-zero, this function automatically updates the mass of the body.
	/// Contacts are not created until the next time step.
	/// @param def the fixture definition.
	/// @warning This function is locked during callbacks.
    /// 创建一个固定装置并将其附加到此主体。如果需要，请使用此功能
    /// 设置一些夹具参数，如摩擦。否则，可以创建
    /// 夹具直接从一个形状。
    /// 如果密度不为零，这个函数会自动更新物体的质量。
    /// 直到下一个时间步骤才创建联系人。
    /// @param定义fixture定义。
    /// 该函数在回调期间被锁定。
	b2Fixture* CreateFixture(const b2FixtureDef* def);

	/// Creates a fixture from a shape and attach it to this body.
	/// This is a convenience function. Use b2FixtureDef if you need to set parameters
	/// like friction, restitution, user data, or filtering.
	/// If the density is non-zero, this function automatically updates the mass of the body.
	/// @param shape the shape to be cloned.
	/// @param density the shape density (set to zero for static bodies).
	/// @warning This function is locked during callbacks.
    /// 从一个形状创建一个夹具，并将其附加到这个主体。
    /// 这是一个方便函数。如果需要设置参数，请使用b2FixtureDef
    /// 比如摩擦、赔偿、用户数据或过滤。
    /// 如果密度不为零，这个函数会自动更新物体的质量。
    /// @param shape要克隆的形状。
    /// @param density形状密度（静态物体设置为零）。
    /// 该函数在回调期间被锁定。
	b2Fixture* CreateFixture(const b2Shape* shape, float density);

	/// Destroy a fixture. This removes the fixture from the broad-phase and
	/// destroys all contacts associated with this fixture. This will
	/// automatically adjust the mass of the body if the body is dynamic and the
	/// fixture has positive density.
	/// All fixtures attached to a body are implicitly destroyed when the body is destroyed.
	/// @param fixture the fixture to be removed.
	/// @warning This function is locked during callbacks.
    /// 破坏固定装置。这将夹具从宽相和
    /// 销毁与此装置关联的所有触点。这将
    /// 如果身体是动态的，自动调整身体的质量
    /// 夹具具有正密度。
    /// 当主体被销毁时，附属于主体的所有fixture都被隐式地销毁。
    /// @param fixture要移除的fixture。
    /// 该函数在回调期间被锁定。
	void DestroyFixture(b2Fixture* fixture);

	/// Set the position of the body's origin and rotation.
	/// Manipulating a body's transform may cause non-physical behavior.
	/// Note: contacts are updated on the next call to b2World::Step.
	/// @param position the world position of the body's local origin.
	/// @param angle the world rotation in radians.
    /// 设置物体原点和旋转的位置。
    /// 操纵身体的变形可能会导致非物理行为。
    /// 注意：联系人将在下次调用b2World::Step时更新。
    /// @param position物体局部原点的世界位置。
    /// 以弧度为单位的世界旋转角度。
	void SetTransform(const b2Vec2& position, float angle);

	/// Get the body transform for the body's origin.
	/// @return the world transform of the body's origin.
    /// 获取物体原点的物体变换。
    /// @返回身体原点的世界变换。
	const b2Transform& GetTransform() const;

	/// Get the world body origin position.
	/// @return the world position of the body's origin.
    /// 获得世界体原点位置。
    /// @返回物体原点的世界位置。
	const b2Vec2& GetPosition() const;

	/// Get the angle in radians.
	/// @return the current world rotation angle in radians.
    /// 得到以弧度为单位的角度。
    /// @返回当前以弧度为单位的世界旋转角度。
	float GetAngle() const;

	/// Get the world position of the center of mass.
    /// 得到质心的世界位置。
	const b2Vec2& GetWorldCenter() const;

	/// Get the local position of the center of mass.
    /// 得到局部质心的位置。
	const b2Vec2& GetLocalCenter() const;

	/// Set the linear velocity of the center of mass.
	/// @param v the new linear velocity of the center of mass.
    /// 设置质心的线速度。
    /// @param v新的质心线速度。
	void SetLinearVelocity(const b2Vec2& v);

	/// Get the linear velocity of the center of mass.
	/// @return the linear velocity of the center of mass.
    /// 得到质心的线速度。
    /// @返回质心的线速度。
	const b2Vec2& GetLinearVelocity() const;

	/// Set the angular velocity.
	/// @param omega the new angular velocity in radians/second.
    /// 设置角速度。
    /// 新的角速度，单位是弧度/秒。
	void SetAngularVelocity(float omega);

	/// Get the angular velocity.
	/// @return the angular velocity in radians/second.
    /// 得到角速度。
    /// @以弧度/秒为单位返回角速度。
	float GetAngularVelocity() const;

	/// Apply a force at a world point. If the force is not
	/// applied at the center of mass, it will generate a torque and
	/// affect the angular velocity. This wakes up the body.
	/// @param force the world force vector, usually in Newtons (N).
	/// @param point the world position of the point of application.
    /// @param wake also wake up the body.
    /// 在世界点上施加一个力。如果力不是
    /// 施加在质心处，它会产生一个力矩
    /// 影响角速度。这唤醒了身体。
    /// @param 力世界力矢量，通常以牛顿(N)为单位。
    /// @param point应用程序点的世界位置。
    /// @param wake也唤醒身体
	void ApplyForce(const b2Vec2& force, const b2Vec2& point, bool wake);

	/// Apply a force to the center of mass. This wakes up the body.
	/// @param force the world force vector, usually in Newtons (N).
    /// @param wake also wake up the body.
    /// 对质心施加一个力。这唤醒了身体。
    /// @param力世界力矢量，通常以牛顿(N)为单位。
    /// @param wake也唤醒身体
	void ApplyForceToCenter(const b2Vec2& force, bool wake);

	/// Apply a torque. This affects the angular velocity
	/// without affecting the linear velocity of the center of mass.
	/// @param torque about the z-axis (out of the screen), usually in N-m.
    /// @param wake also wake up the body.
    /// 施加一个扭矩。这影响角速度
    /// 不影响质心的线速度。
    /// @param扭矩绕z轴（屏幕外），通常以N-m为单位。
    /// @param wake也唤醒身体。
	void ApplyTorque(float torque, bool wake);

	/// Apply an impulse at a point. This immediately modifies the velocity.
	/// It also modifies the angular velocity if the point of application
	/// is not at the center of mass. This wakes up the body.
	/// @param impulse the world impulse vector, usually in N-seconds or kg-m/s.
	/// @param point the world position of the point of application.
    /// @param wake also wake up the body.
    /// 在一点上施加一个脉冲。这马上改变了速度。
    /// 它也改变了应用点的角速度
    /// 不在质心处。这唤醒了身体。
    /// @param impulse世界脉冲矢量，通常以n秒或kg-m/s为单位。
    /// @param point应用程序点的世界位置。
    /// @param wake也唤醒身体
	void ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake);

	/// Apply an impulse to the center of mass. This immediately modifies the velocity.
	/// @param impulse the world impulse vector, usually in N-seconds or kg-m/s.
    /// @param wake also wake up the body.
    /// 对质心施加一个脉冲。这马上改变了速度。
    /// @param impulse世界脉冲矢量，通常以n秒或kg-m/s为单位。
    /// @param wake也唤醒身体
	void ApplyLinearImpulseToCenter(const b2Vec2& impulse, bool wake);

	/// Apply an angular impulse.
	/// @param impulse the angular impulse in units of kg*m*m/s
    /// @param wake also wake up the body.
    /// 施加一个角脉冲。
    /// @param冲量角冲量，单位为kg*m*m/s
    /// @param wake也唤醒身体
	void ApplyAngularImpulse(float impulse, bool wake);

	/// Get the total mass of the body.
	/// @return the mass, usually in kilograms (kg).
    /// 求出物体的总质量。
    /// 返回质量，通常以千克（kg）为单位。
	float GetMass() const;

	/// Get the rotational inertia of the body about the local origin.
	/// @return the rotational inertia, usually in kg-m^2.
    /// 得到物体关于局部原点的转动惯量。
    /// 返回转动惯量，通常以kg-m^2为单位。
	float GetInertia() const;

	/// Get the mass data of the body.
	/// @return a struct containing the mass, inertia and center of the body.
    /// 获取物体的质量数据。
    /// 返回一个包含质量、惯性和物体中心的结构体。
	b2MassData GetMassData() const;

	/// Set the mass properties to override the mass properties of the fixtures.
	/// Note that this changes the center of mass position.
	/// Note that creating or destroying fixtures can also alter the mass.
	/// This function has no effect if the body isn't dynamic.
	/// @param data the mass properties.
    /// 设置质量属性以覆盖夹具的质量属性。
    /// 注意这改变了质心的位置。
    /// 注意，创建或销毁固定装置也可以改变质量。
    /// 如果身体不是动态的，这个功能就没有作用。
    /// @param数据质量属性。
	void SetMassData(const b2MassData* data);

	/// This resets the mass properties to the sum of the mass properties of the fixtures.
	/// This normally does not need to be called unless you called SetMassData to override
	/// the mass and you later want to reset the mass.
    /// 这将质量属性重置为夹具质量属性的总和。
    /// 这通常不需要被调用，除非你调用SetMassData来重写
    /// 质量，之后你想重置质量。
	void ResetMassData();

	/// Get the world coordinates of a point given the local coordinates.
	/// @param localPoint a point on the body measured relative the the body's origin.
	/// @return the same point expressed in world coordinates.
    /// 给定局部坐标，获取点的世界坐标。
    /// @param localPoint物体上相对于物体原点测量的点。
    /// @返回用世界坐标表示的相同点。
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint) const;

	/// Get the world coordinates of a vector given the local coordinates.
	/// @param localVector a vector fixed in the body.
	/// @return the same vector expressed in world coordinates.
    /// 给定局部坐标，获取矢量的世界坐标。
    /// @param localVector一个固定在主体中的向量。
    /// @返回用世界坐标表示的相同向量。
	b2Vec2 GetWorldVector(const b2Vec2& localVector) const;

	/// Gets a local point relative to the body's origin given a world point.
	/// @param worldPoint a point in world coordinates.
	/// @return the corresponding local point relative to the body's origin.
    /// 给定世界点，获取相对于物体原点的局部点。
    /// @param worldPoint世界坐标中的一个点。
    /// @返回相对于物体原点的相应局部点。
	b2Vec2 GetLocalPoint(const b2Vec2& worldPoint) const;

	/// Gets a local vector given a world vector.
	/// @param worldVector a vector in world coordinates.
	/// @return the corresponding local vector.
    /// 给定一个世界向量，获取一个局部向量。
    /// @param worldVector世界坐标中的矢量。
    /// @返回相应的本地向量。
	b2Vec2 GetLocalVector(const b2Vec2& worldVector) const;

	/// Get the world linear velocity of a world point attached to this body.
	/// @param worldPoint a point in world coordinates.
	/// @return the world velocity of a point.
    /// 得到附着在这个物体上的一个世界点的世界线速度。
    /// @param worldPoint世界坐标中的一个点。
    /// @返回点的世界速度。
	b2Vec2 GetLinearVelocityFromWorldPoint(const b2Vec2& worldPoint) const;

	/// Get the world velocity of a local point.
	/// @param localPoint a point in local coordinates.
	/// @return the world velocity of a point.
    /// 得到一个局部点的世界速度。
    /// @param localPoint本地坐标中的一个点。
    /// @返回点的世界速度。
	b2Vec2 GetLinearVelocityFromLocalPoint(const b2Vec2& localPoint) const;

	/// Get the linear damping of the body.
    /// 得到物体的线性阻尼。
	float GetLinearDamping() const;

	/// Set the linear damping of the body.
    /// 设置物体的线性阻尼。
	void SetLinearDamping(float linearDamping);

	/// Get the angular damping of the body.
    /// 得到物体的角阻尼。
	float GetAngularDamping() const;

	/// Set the angular damping of the body.
    /// 设置物体的角阻尼。
	void SetAngularDamping(float angularDamping);

	/// Get the gravity scale of the body.
    /// 得到物体的重力刻度。
	float GetGravityScale() const;

	/// Set the gravity scale of the body.
    /// 设置物体的重力比例。
	void SetGravityScale(float scale);

	/// Set the type of this body. This may alter the mass and velocity.
    /// 设置该主体的类型。这可能会改变质量和速度。
	void SetType(b2BodyType type);

	/// Get the type of this body.
    /// 得到该主体的类型。
	b2BodyType GetType() const;

	/// Should this body be treated like a bullet for continuous collision detection?
    /// 这个物体是否应该被当作子弹来进行连续碰撞检测？
	void SetBullet(bool flag);

	/// Is this body treated like a bullet for continuous collision detection?
    /// 这个物体被当做子弹来进行连续碰撞检测吗？
	bool IsBullet() const;

	/// You can disable sleeping on this body. If you disable sleeping, the
	/// body will be woken.
    /// 设置是否允许物体进入睡眠状态。
	void SetSleepingAllowed(bool flag);

	/// Is this body allowed to sleep
    /// 是否允许物体进入睡眠状态。
	bool IsSleepingAllowed() const;

	/// Set the sleep state of the body. A sleeping body has very
	/// low CPU cost.
	/// @param flag set to true to wake the body, false to put it to sleep.
    /// 设置身体的睡眠状态。一个睡觉的身体有很多
    /// 低CPU成本。
    /// @param标志设置为true唤醒主体，false使其进入睡眠状态。
	void SetAwake(bool flag);

	/// Get the sleeping state of this body.
	/// @return true if the body is awake.
    /// 得到这个身体的睡眠状态。
    /// 如果身体是清醒的，返回true。
	bool IsAwake() const;

	/// Allow a body to be disabled. A disabled body is not simulated and cannot
	/// be collided with or woken up.
	/// If you pass a flag of true, all fixtures will be added to the broad-phase.
	/// If you pass a flag of false, all fixtures will be removed from the
	/// broad-phase and all contacts will be destroyed.
	/// Fixtures and joints are otherwise unaffected. You may continue
	/// to create/destroy fixtures and joints on disabled bodies.
	/// Fixtures on a disabled body are implicitly disabled and will
	/// not participate in collisions, ray-casts, or queries.
	/// Joints connected to a disabled body are implicitly disabled.
	/// An diabled body is still owned by a b2World object and remains
	/// in the body list.
    /// 允许身体残疾。残疾的身体不是模拟的，也不能模拟
    /// 他撞了一下或醒了过来。
    /// 如果传递的标志为true，则所有装置都将添加到宽相位。
    /// 如果传递的标志为false，则所有fixture将从
    /// 宽相位和所有触点都将被摧毁。
    /// 夹具和接头在其他方面不受影响。你可以继续
    /// 创建/销毁残疾物体上的固定装置和关节。
    /// 残疾身体上的固定装置被隐式地禁用，并将被禁用
    /// 不参与碰撞、光线投射或查询。
    /// 连接到残疾身体的关节被隐式禁用。
    /// 残疾的身体仍然由b2World对象拥有并保留
    /// 在body列表中。
	void SetEnabled(bool flag);

	/// Get the active state of the body.
    /// 身体处于活跃状态。
	bool IsEnabled() const;

	/// Set this body to have fixed rotation. This causes the mass
	/// to be reset.
    /// 将这个物体设置为固定旋转。这就产生了质量
    /// 待重置。
	void SetFixedRotation(bool flag);

	/// Does this body have fixed rotation?
    /// 这个物体有固定的旋转吗？
	bool IsFixedRotation() const;

	/// Get the list of all fixtures attached to this body.
    /// 把附在这个物体上的所有装置的清单拿出来。
	b2Fixture* GetFixtureList();
	const b2Fixture* GetFixtureList() const;

	/// Get the list of all joints attached to this body.
    /// 把这个身体上所有关节的清单拿出来。
	b2JointEdge* GetJointList();
	const b2JointEdge* GetJointList() const;

	/// Get the list of all contacts attached to this body.
	/// @warning this list changes during the time step and you may
	/// miss some collisions if you don't use b2ContactListener.
    /// 把这个人的所有联系人的名单拿来。
    /// @警告此列表在时间步骤中会发生变化，您可以
    /// 如果不使用b2ContactListener，会错过一些碰撞。
	b2ContactEdge* GetContactList();
	const b2ContactEdge* GetContactList() const;

	/// Get the next body in the world's body list.
    /// 找到世界物体名单上的下一个物体。
	b2Body* GetNext();
	const b2Body* GetNext() const;

	/// Get the user data pointer that was provided in the body definition.
    /// 获取主体定义中提供的用户数据指针。
	b2BodyUserData& GetUserData();
	const b2BodyUserData& GetUserData() const;

	/// Get the parent world of this body.
    /// 得到这个主体的父世界。
	b2World* GetWorld();
	const b2World* GetWorld() const;

	/// Dump this body to a file
    /// 将该主体转储到文件中
	void Dump();

private:

	friend class b2World;
	friend class b2Island;
	friend class b2ContactManager;
	friend class b2ContactSolver;
	friend class b2Contact;

	friend class b2DistanceJoint;
	friend class b2FrictionJoint;
	friend class b2GearJoint;
	friend class b2MotorJoint;
	friend class b2MouseJoint;
	friend class b2PrismaticJoint;
	friend class b2PulleyJoint;
	friend class b2RevoluteJoint;
	friend class b2WeldJoint;
	friend class b2WheelJoint;

	// m_flags
	enum
	{
		e_islandFlag		= 0x0001,
		e_awakeFlag			= 0x0002,
		e_autoSleepFlag		= 0x0004,
		e_bulletFlag		= 0x0008,
		e_fixedRotationFlag	= 0x0010,
		e_enabledFlag		= 0x0020,
		e_toiFlag			= 0x0040
	};

	b2Body(const b2BodyDef* bd, b2World* world);
	~b2Body();

	void SynchronizeFixtures();
	void SynchronizeTransform();

	// This is used to prevent connected bodies from colliding.
	// It may lie, depending on the collideConnected flag.
	bool ShouldCollide(const b2Body* other) const;

	void Advance(float t);

	b2BodyType m_type;

	uint16 m_flags;

	int32 m_islandIndex;

	b2Transform m_xf;		// the body origin transform
	b2Sweep m_sweep;		// the swept motion for CCD

	b2Vec2 m_linearVelocity;
	float m_angularVelocity;

	b2Vec2 m_force;
	float m_torque;

	b2World* m_world;
	b2Body* m_prev;
	b2Body* m_next;

	b2Fixture* m_fixtureList;
	int32 m_fixtureCount;

	b2JointEdge* m_jointList;
	b2ContactEdge* m_contactList;

	float m_mass, m_invMass;

	// Rotational inertia about the center of mass.
	float m_I, m_invI;

	float m_linearDamping;
	float m_angularDamping;
	float m_gravityScale;

	float m_sleepTime;

	b2BodyUserData m_userData;
};

inline b2BodyType b2Body::GetType() const
{
	return m_type;
}

inline const b2Transform& b2Body::GetTransform() const
{
	return m_xf;
}

inline const b2Vec2& b2Body::GetPosition() const
{
	return m_xf.p;
}

inline float b2Body::GetAngle() const
{
	return m_sweep.a;
}

inline const b2Vec2& b2Body::GetWorldCenter() const
{
	return m_sweep.c;
}

inline const b2Vec2& b2Body::GetLocalCenter() const
{
	return m_sweep.localCenter;
}

inline void b2Body::SetLinearVelocity(const b2Vec2& v)
{
	if (m_type == b2_staticBody)
	{
		return;
	}

	if (b2Dot(v,v) > 0.0f)
	{
		SetAwake(true);
	}

	m_linearVelocity = v;
}

inline const b2Vec2& b2Body::GetLinearVelocity() const
{
	return m_linearVelocity;
}

inline void b2Body::SetAngularVelocity(float w)
{
	if (m_type == b2_staticBody)
	{
		return;
	}

	if (w * w > 0.0f)
	{
		SetAwake(true);
	}

	m_angularVelocity = w;
}

inline float b2Body::GetAngularVelocity() const
{
	return m_angularVelocity;
}

inline float b2Body::GetMass() const
{
	return m_mass;
}

inline float b2Body::GetInertia() const
{
	return m_I + m_mass * b2Dot(m_sweep.localCenter, m_sweep.localCenter);
}

inline b2MassData b2Body::GetMassData() const
{
	b2MassData data;
	data.mass = m_mass;
	data.I = m_I + m_mass * b2Dot(m_sweep.localCenter, m_sweep.localCenter);
	data.center = m_sweep.localCenter;
	return data;
}

inline b2Vec2 b2Body::GetWorldPoint(const b2Vec2& localPoint) const
{
	return b2Mul(m_xf, localPoint);
}

inline b2Vec2 b2Body::GetWorldVector(const b2Vec2& localVector) const
{
	return b2Mul(m_xf.q, localVector);
}

inline b2Vec2 b2Body::GetLocalPoint(const b2Vec2& worldPoint) const
{
	return b2MulT(m_xf, worldPoint);
}

inline b2Vec2 b2Body::GetLocalVector(const b2Vec2& worldVector) const
{
	return b2MulT(m_xf.q, worldVector);
}

inline b2Vec2 b2Body::GetLinearVelocityFromWorldPoint(const b2Vec2& worldPoint) const
{
	return m_linearVelocity + b2Cross(m_angularVelocity, worldPoint - m_sweep.c);
}

inline b2Vec2 b2Body::GetLinearVelocityFromLocalPoint(const b2Vec2& localPoint) const
{
	return GetLinearVelocityFromWorldPoint(GetWorldPoint(localPoint));
}

inline float b2Body::GetLinearDamping() const
{
	return m_linearDamping;
}

inline void b2Body::SetLinearDamping(float linearDamping)
{
	m_linearDamping = linearDamping;
}

inline float b2Body::GetAngularDamping() const
{
	return m_angularDamping;
}

inline void b2Body::SetAngularDamping(float angularDamping)
{
	m_angularDamping = angularDamping;
}

inline float b2Body::GetGravityScale() const
{
	return m_gravityScale;
}

inline void b2Body::SetGravityScale(float scale)
{
	m_gravityScale = scale;
}

inline void b2Body::SetBullet(bool flag)
{
	if (flag)
	{
		m_flags |= e_bulletFlag;
	}
	else
	{
		m_flags &= ~e_bulletFlag;
	}
}

inline bool b2Body::IsBullet() const
{
	return (m_flags & e_bulletFlag) == e_bulletFlag;
}

inline void b2Body::SetAwake(bool flag)
{
	if (m_type == b2_staticBody)
	{
		return;
	}

	if (flag)
	{
		m_flags |= e_awakeFlag;
		m_sleepTime = 0.0f;
	}
	else
	{
		m_flags &= ~e_awakeFlag;
		m_sleepTime = 0.0f;
		m_linearVelocity.SetZero();
		m_angularVelocity = 0.0f;
		m_force.SetZero();
		m_torque = 0.0f;
	}
}

inline bool b2Body::IsAwake() const
{
	return (m_flags & e_awakeFlag) == e_awakeFlag;
}

inline bool b2Body::IsEnabled() const
{
	return (m_flags & e_enabledFlag) == e_enabledFlag;
}

inline bool b2Body::IsFixedRotation() const
{
	return (m_flags & e_fixedRotationFlag) == e_fixedRotationFlag;
}

inline void b2Body::SetSleepingAllowed(bool flag)
{
	if (flag)
	{
		m_flags |= e_autoSleepFlag;
	}
	else
	{
		m_flags &= ~e_autoSleepFlag;
		SetAwake(true);
	}
}

inline bool b2Body::IsSleepingAllowed() const
{
	return (m_flags & e_autoSleepFlag) == e_autoSleepFlag;
}

inline b2Fixture* b2Body::GetFixtureList()
{
	return m_fixtureList;
}

inline const b2Fixture* b2Body::GetFixtureList() const
{
	return m_fixtureList;
}

inline b2JointEdge* b2Body::GetJointList()
{
	return m_jointList;
}

inline const b2JointEdge* b2Body::GetJointList() const
{
	return m_jointList;
}

inline b2ContactEdge* b2Body::GetContactList()
{
	return m_contactList;
}

inline const b2ContactEdge* b2Body::GetContactList() const
{
	return m_contactList;
}

inline b2Body* b2Body::GetNext()
{
	return m_next;
}

inline const b2Body* b2Body::GetNext() const
{
	return m_next;
}

inline b2BodyUserData& b2Body::GetUserData()
{
	return m_userData;
}

inline const b2BodyUserData& b2Body::GetUserData() const
{
	return m_userData;
}

inline void b2Body::ApplyForce(const b2Vec2& force, const b2Vec2& point, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate a force if the body is sleeping.
	if (m_flags & e_awakeFlag)
	{
		m_force += force;
		m_torque += b2Cross(point - m_sweep.c, force);
	}
}

inline void b2Body::ApplyForceToCenter(const b2Vec2& force, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate a force if the body is sleeping
	if (m_flags & e_awakeFlag)
	{
		m_force += force;
	}
}

inline void b2Body::ApplyTorque(float torque, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate a force if the body is sleeping
	if (m_flags & e_awakeFlag)
	{
		m_torque += torque;
	}
}

inline void b2Body::ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate velocity if the body is sleeping
	if (m_flags & e_awakeFlag)
	{
		m_linearVelocity += m_invMass * impulse;
		m_angularVelocity += m_invI * b2Cross(point - m_sweep.c, impulse);
	}
}

inline void b2Body::ApplyLinearImpulseToCenter(const b2Vec2& impulse, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate velocity if the body is sleeping
	if (m_flags & e_awakeFlag)
	{
		m_linearVelocity += m_invMass * impulse;
	}
}

inline void b2Body::ApplyAngularImpulse(float impulse, bool wake)
{
	if (m_type != b2_dynamicBody)
	{
		return;
	}

	if (wake && (m_flags & e_awakeFlag) == 0)
	{
		SetAwake(true);
	}

	// Don't accumulate velocity if the body is sleeping
	if (m_flags & e_awakeFlag)
	{
		m_angularVelocity += m_invI * impulse;
	}
}

inline void b2Body::SynchronizeTransform()
{
	m_xf.q.Set(m_sweep.a);
	m_xf.p = m_sweep.c - b2Mul(m_xf.q, m_sweep.localCenter);
}

inline void b2Body::Advance(float alpha)
{
	// Advance to the new safe time. This doesn't sync the broad-phase.
	m_sweep.Advance(alpha);
	m_sweep.c = m_sweep.c0;
	m_sweep.a = m_sweep.a0;
	m_xf.q.Set(m_sweep.a);
	m_xf.p = m_sweep.c - b2Mul(m_xf.q, m_sweep.localCenter);
}

inline b2World* b2Body::GetWorld()
{
	return m_world;
}

inline const b2World* b2Body::GetWorld() const
{
	return m_world;
}

#endif
