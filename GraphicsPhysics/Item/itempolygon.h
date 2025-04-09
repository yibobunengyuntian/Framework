#ifndef ITEMPOLYGON_H
#define ITEMPOLYGON_H

#include <QPointF>
#include "itembase.h"

class ItemPolygon: public ItemBase
{
public:

protected:
    friend class Scene;

    ItemPolygon(b2World *world, b2BodyDef bd, const QList<QPointF> &points);
    virtual void paintItem(QPainter *painter) override;

private:
    QList<QPointF> m_points;
};

#endif // ITEMPOLYGON_H
