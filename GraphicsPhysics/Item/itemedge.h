#ifndef ITEMEDGE_H
#define ITEMEDGE_H

#include "itembase.h"

class ItemEdge: public ItemBase
{
public:

protected:
    friend class Scene;

    ItemEdge(b2World *world, b2BodyDef bd, const QPointF &p1, const QPointF &p2);
    virtual void paintItem(QPainter *painter) override;
    void setPen(QPen pen);

private:
    QPointF m_p1;
    QPointF m_p2;

    QPen m_pen;
};

#endif // ITEMEDGE_H
