#include "maths.h"

b2Vec2 vector2DToVec2(const QVector2D &v2d)
{
    return b2Vec2(v2d.x(), v2d.y());
}

b2Vec2 pointToVec2(const QPointF &p)
{
    return b2Vec2(p.x(), p.y());
}

QPointF vec2ToPoint(const b2Vec2 &vec2)
{
    return QPointF(vec2.x, vec2.y);
}
