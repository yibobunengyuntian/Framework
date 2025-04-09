#include "itemrect.h"
#include "scene.h"


ItemRect::ItemRect(b2World *world, b2BodyDef bd, const QPointF &center, const qreal w, const qreal &h)
    :ItemBase(world, bd), m_w(w), m_h(h)
{
    ItemBase::setPos(center);
    m_shape.addRect(-m_w/2.0, -m_h/2.0, m_w, m_h);

    b2PolygonShape *pPolygonShape = new b2PolygonShape;
    pPolygonShape->SetAsBox (w/2.0/Scene::m_pix_meter, h/2.0/Scene::m_pix_meter);
    m_pB2Shape = pPolygonShape;
    setMaterial();
}

void ItemRect::paintItem(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_brush);
    painter->drawRect(- m_w/2.0, - m_h/2.0, m_w, m_h);
}
