#include "itempolygon.h"
#include "maths.h"
#include "scene.h"

ItemPolygon::ItemPolygon(b2World *world, b2BodyDef bd, const QList<QPointF> &points)
    :ItemBase(world, bd)
{
    //将多边形质心与局部坐标原点重合
    uint count = points.count();
    b2Vec2 *vertices = new b2Vec2[count];
    for(uint i = 0; i < count; ++i)
    {
        vertices[i] = pointToVec2(points[i]/Scene::m_pix_meter);
    }
    b2PolygonShape *pPolygon = new b2PolygonShape;
    pPolygon->Set(vertices, count);
    QPointF pos = vec2ToPoint(pPolygon->m_centroid) * Scene::m_pix_meter;
    for(uint i = 0; i < count; ++i)
    {
        m_points.append((points[i] - pos).toPoint());
        vertices[i] = pointToVec2(m_points[i]/Scene::m_pix_meter);
    }
    m_points.append((points[0] - pos).toPoint());
    delete pPolygon;
    pPolygon = new b2PolygonShape;
    pPolygon->Set(vertices, count);
    m_pB2Shape = pPolygon;
    m_shape.addPolygon(QPolygonF(m_points));
    this->setPos(pos);
    setMaterial();
}

void ItemPolygon::paintItem(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_brush);
    painter->drawPolygon(QPolygonF(m_points));
}
