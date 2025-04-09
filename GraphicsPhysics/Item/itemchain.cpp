#include "itemchain.h"

#include "maths.h"
#include "scene.h"

ItemChain::ItemChain(b2World *world, b2BodyDef bd, const QList<QPointF> &points)
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
        if(i == 0)
        {
            m_shape.moveTo(m_points[i]);
        }
        else
        {
            m_shape.lineTo(m_points[i]);
        }
    }
    delete pPolygon;
    b2ChainShape *pChainShape = new b2ChainShape;
    b2Vec2 invalidVertex(0.0f, 0.0f); // 表示无效连接的顶点
    pChainShape->CreateChain(vertices, count, invalidVertex, invalidVertex);
    m_pB2Shape = pChainShape;
    this->setPos(pos);
    setMaterial();
}

void ItemChain::paintItem(QPainter *painter)
{
    painter->drawPath(m_shape);
}
