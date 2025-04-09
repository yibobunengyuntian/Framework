#ifndef ITEMRECT_H
#define ITEMRECT_H

#include <QPointF>
#include "itembase.h"

class ItemRect: public ItemBase
{
public:

protected:
    friend class Scene;

    ItemRect(b2World *world, b2BodyDef bd, const QPointF &center, const qreal w, const qreal &h);
    virtual void paintItem(QPainter *painter) override;

private:
    qreal m_w;
    qreal m_h;
};

#endif // ITEMRECT_H
