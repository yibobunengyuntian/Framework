#ifndef ITEMCIRCLE_H
#define ITEMCIRCLE_H

#include <QPointF>
#include "itembase.h"

class ItemCircle: public ItemBase
{
public:

protected:
    friend class Scene;

    ItemCircle(b2World *world, b2BodyDef bd, const QPointF &center, const qreal r);
    virtual void paintItem(QPainter *painter) override;

private:
    qreal m_r;
};

#endif // ITEMCIRCLE_H
