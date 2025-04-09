#ifndef MATHS_H
#define MATHS_H

#include <QVector2D>

#include "box2d/box2d.h"

b2Vec2 vector2DToVec2(const QVector2D &v2d);
b2Vec2 pointToVec2(const QPointF &p);
QPointF vec2ToPoint(const b2Vec2 &vec2);

#endif // MATHS_H
