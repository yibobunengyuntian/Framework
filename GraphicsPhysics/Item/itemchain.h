#ifndef ITEMCHAIN_H
#define ITEMCHAIN_H

#include <QPointF>
#include "itembase.h"

class ItemChain: public ItemBase
{
public:

protected:
    friend class Scene;

    ItemChain(b2World *world, b2BodyDef bd, const QList<QPointF> &points);
    virtual void paintItem(QPainter *painter) override;

private:
    QList<QPointF> m_points;
};

#endif // ITEMCHAIN_H
