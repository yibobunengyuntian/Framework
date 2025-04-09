#include "itemedge.h"
#include "scene.h"
#include "maths.h"

ItemEdge::ItemEdge(b2World *world, b2BodyDef bd, const QPointF &p1, const QPointF &p2)
    :ItemBase(world, bd)
{
    const QPointF pos = (p1 + p2)/2.0;
    m_p1 = p1 - pos;
    m_p2 = p2 - pos;
    m_shape.moveTo(m_p1);
    m_shape.lineTo(m_p2);
    b2EdgeShape *pEdge = new b2EdgeShape;
    pEdge->SetTwoSided(pointToVec2(m_p1/Scene::m_pix_meter), pointToVec2(m_p2/Scene::m_pix_meter));
    m_pB2Shape = pEdge;
    this->setPos(pos);
    setMaterial();
}

void ItemEdge::paintItem(QPainter *painter)
{
    painter->setPen(m_pen);
    // painter->setBrush(m_brush);
    painter->drawLine(m_p1, m_p2);
}

void ItemEdge::setPen(QPen pen)
{
    m_pen = pen;
}
