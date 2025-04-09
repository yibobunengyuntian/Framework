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
#ifndef B2_POLYGON_SHAPE_H
#define B2_POLYGON_SHAPE_H

#include "b2_api.h"
#include "b2_shape.h"

struct b2Hull;

/// A solid convex polygon. It is assumed that the interior of the polygon is to
/// the left of each edge.
/// Polygons have a maximum number of vertices equal to b2_maxPolygonVertices.
/// In most cases you should not need many vertices for a convex polygon.
/// 凸多边形：实心凸多边形假设多边形的内部为
/// 每条边的左边。
/// 多边形的最大顶点数等于b2_maxPolygonVertices。
/// 在大多数情况下，凸多边形不需要很多顶点。
class B2_API b2PolygonShape : public b2Shape
{
public:
	b2PolygonShape();

	/// Implement b2Shape.
    /// 实现b2Shape。
	b2Shape* Clone(b2BlockAllocator* allocator) const override;

	/// @see b2Shape::GetChildCount
	int32 GetChildCount() const override;

	/// Create a convex hull from the given array of local points.
	/// The count must be in the range [3, b2_maxPolygonVertices].
	/// @warning the points may be re-ordered, even if they form a convex polygon
	/// @warning if this fails then the polygon is invalid
    /// @returns true if valid.
    /// 从给定的局部点数组创建一个凸包。
    /// 计数必须在[3，b2_maxPolygonVertices]的范围内。
    /// @warning这些点可以重新排序，即使它们形成一个凸多边形
    /// @warning如果失败，则多边形无效
    /// 如果有效，@返回true
	bool Set(const b2Vec2* points, int32 count);

	/// Create a polygon from a given convex hull (see b2ComputeHull).
    /// @warning the hull must be valid or this will crash or have unexpected behavior.
    /// 从给定的凸包中创建一个多边形（参见b2computeull）。
    /// 警告船体必须是有效的，否则这将崩溃或有意想不到的行为
	void Set(const b2Hull& hull);

	/// Build vertices to represent an axis-aligned box centered on the local origin.
	/// @param hx the half-width.
	/// @param hy the half-height.
    /// 构建顶点以表示以本地原点为中心的轴对齐框。
    /// @param 半宽度。
    /// @param 半高。
	void SetAsBox(float hx, float hy);

	/// Build vertices to represent an oriented box.
	/// @param hx the half-width.
	/// @param hy the half-height.
	/// @param center the center of the box in local coordinates.
	/// @param angle the rotation of the box in local coordinates.
    /// 构建顶点来表示一个有方向的框。
    /// @param 半宽度。
    /// @param 半高。
    /// @param 将框的中心置于本地坐标中。
    /// 以局部坐标旋转盒子的角度。
	void SetAsBox(float hx, float hy, const b2Vec2& center, float angle);

	/// @see b2Shape::TestPoint
	bool TestPoint(const b2Transform& transform, const b2Vec2& p) const override;

	/// Implement b2Shape.
	/// @note because the polygon is solid, rays that start inside do not hit because the normal is
	/// not defined.
    /// 实现b2Shape。
    /// @注意，因为多边形是实心的，光线从里面开始不会击中，因为法线是实心的
    /// 没有定义。
	bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
					const b2Transform& transform, int32 childIndex) const override;

	/// @see b2Shape::ComputeAABB
	void ComputeAABB(b2AABB* aabb, const b2Transform& transform, int32 childIndex) const override;

	/// @see b2Shape::ComputeMass
	void ComputeMass(b2MassData* massData, float density) const override;

	/// Validate convexity. This is a very time consuming operation.
	/// @returns true if valid
    /// 验证凸性。这是一个非常耗时的操作。
    /// 如果有效，@返回true
	bool Validate() const;

	b2Vec2 m_centroid;
	b2Vec2 m_vertices[b2_maxPolygonVertices];
	b2Vec2 m_normals[b2_maxPolygonVertices];
	int32 m_count;
};

#endif
