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

#ifndef B2_SHAPE_H
#define B2_SHAPE_H

#include "b2_api.h"
#include "b2_math.h"
#include "b2_collision.h"

class b2BlockAllocator;

/// This holds the mass data computed for a shape.
/// 这保存了为形状计算的质量数据。
struct B2_API b2MassData
{
	/// The mass of the shape, usually in kilograms.
    /// 形状的质量，通常以公斤为单位。
	float mass;

	/// The position of the shape's centroid relative to the shape's origin.
    /// 形状的质心相对于形状的原点的位置。
	b2Vec2 center;

	/// The rotational inertia of the shape about the local origin.
    /// 形状围绕局部原点的转动惯量。
	float I;
};

/// A shape is used for collision detection. You can create a shape however you like.
/// Shapes used for simulation in b2World are created automatically when a b2Fixture
/// is created. Shapes may encapsulate a one or more child shapes.
/// 形状用于碰撞检测。你可以创建一个形状，但你喜欢。
/// 在b2World中用于模拟的形状是在b2Fixture中自动创建的。
/// 形状可以封装一个或多个子形状。
class B2_API b2Shape
{
public:

	enum Type
	{
		e_circle = 0,
		e_edge = 1,
		e_polygon = 2,
		e_chain = 3,
		e_typeCount = 4
	};

	virtual ~b2Shape() {}

	/// Clone the concrete shape using the provided allocator.
    /// 使用提供的分配器克隆具体形状。
	virtual b2Shape* Clone(b2BlockAllocator* allocator) const = 0;

	/// Get the type of this shape. You can use this to down cast to the concrete shape.
	/// @return the shape type.
    /// 得到这个形状的类型。你可以用它来浇筑混凝土的形状。
    /// @返回形状类型。
	Type GetType() const;

	/// Get the number of child primitives.
    /// 获取子原的数量。
	virtual int32 GetChildCount() const = 0;

	/// Test a point for containment in this shape. This only works for convex shapes.
	/// @param xf the shape world transform.
	/// @param p a point in world coordinates.
    /// 测试这个形状的一个点是否有容器。这只适用于凸形状。
    /// @param xf形状世界变换。
    /// @param p世界坐标中的一个点。
	virtual bool TestPoint(const b2Transform& xf, const b2Vec2& p) const = 0;

	/// Cast a ray against a child shape.
	/// @param output the ray-cast results.
	/// @param input the ray-cast input parameters.
	/// @param transform the transform to be applied to the shape.
    /// @param childIndex the child shape index.
    /// 向一个子形状投射一道光线。
    /// @param输出光线投射结果。
    /// @param输入光线投射输入参数。
    /// @param transform要应用到形状上的变换。
    /// @param childIndex子形状索引
	virtual bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
						const b2Transform& transform, int32 childIndex) const = 0;

	/// Given a transform, compute the associated axis aligned bounding box for a child shape.
	/// @param aabb returns the axis aligned box.
	/// @param xf the world transform of the shape.
    /// @param childIndex the child shape.
    /// 给定一个变换，计算子形状的相关轴对齐边界框。
    /// @param aabb返回与轴对齐的框。
    /// @param xf形状的世界变换。
    /// @param childIndex子形状
	virtual void ComputeAABB(b2AABB* aabb, const b2Transform& xf, int32 childIndex) const = 0;

	/// Compute the mass properties of this shape using its dimensions and density.
	/// The inertia tensor is computed about the local origin.
	/// @param massData returns the mass data for this shape.
	/// @param density the density in kilograms per meter squared.
    /// 用它的尺寸和密度计算这个形状的质量特性。
    /// 在局部原点处计算惯性张量。
    /// @param massData返回这个形状的质量数据。
    /// @param密度密度单位是千克每平方米。
	virtual void ComputeMass(b2MassData* massData, float density) const = 0;

	Type m_type;

	/// Radius of a shape. For polygonal shapes this must be b2_polygonRadius. There is no support for
	/// making rounded polygons.
    /// 形状的半径。对于多边形形状，它必须是b2_polygonRadius。没有支持
    /// 制作圆角多边形。
	float m_radius;
};

inline b2Shape::Type b2Shape::GetType() const
{
	return m_type;
}

#endif
