#include "itemcircle.h"

ItemCircle::ItemCircle(b2World *world, b2BodyDef bd, const QPointF &center, const qreal r)
    :ItemBase(world, bd), m_r(r)
{
    this->setPos(center);
    m_shape.addEllipse(QPoint(0, 0), m_r, m_r);

    b2CircleShape *pCircleShape = new b2CircleShape;
    pCircleShape->m_p.Set(0,0);
    pCircleShape->m_radius = r/30.0;
    m_pB2Shape = pCircleShape;
    setMaterial();
}

void ItemCircle::paintItem(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_brush);
    painter->drawEllipse(QPointF(0, 0), m_r, m_r);
    // painter->drawRect(-m_r, -m_r, 2*m_r, 2*m_r);
}
