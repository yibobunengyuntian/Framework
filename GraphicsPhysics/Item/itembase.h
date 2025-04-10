#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsItem>
#include <QPainter>

#include "box2d/box2d.h"

class ItemBase: public QGraphicsItem
{

public:
    void setPos(const QPointF &pos);
    void setRotation(const qreal &angle);
    void setBrush(const QBrush &brush);
    void setImage(const QString &image);
    void setMaterial(const float &friction = 0.2f,
                     const float &restitution = 0.0f,
                     const float &density = 0.0f,
                     const float &restitutionThreshold = 1.0f * b2_lengthUnitsPerMeter,
                     const float &isSensor = false);

    void setShowBoundingRect(const bool &is = true);
    void setShowShape(const bool &is = true);

    void setLinearVelocity(const QPointF& v);

    void setUserData(const QVariant &data);
    QVariant userData();

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

protected:
    friend class Scene;

    ItemBase(b2World *world, b2BodyDef bd);
    ~ItemBase();

    void updateTransform();

    inline void setTransformOriginPoint(const QPointF &origin)
    {
        QGraphicsItem::setTransformOriginPoint(origin);
    }
    inline void setTransformOriginPoint(qreal ax, qreal ay)
    {
        QGraphicsItem::setTransformOriginPoint(ax, ay);
    }
    inline void setTransform(const QTransform &matrix, bool combine = false)
    {
        QGraphicsItem::setTransform(matrix, combine);
    }
    inline void resetTransform()
    {
        QGraphicsItem::resetTransform();
    }
    inline void setTransformations(const QList<QGraphicsTransform *> &transformations)
    {
        QGraphicsItem::setTransformations(transformations);
    }

    virtual void paintItem(QPainter *painter) = 0;
    virtual void paintShape(QPainter *painter);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QPainterPath m_shape;
    b2Body *m_pBody = nullptr;
    b2Shape *m_pB2Shape = nullptr;

    bool m_isShowShape = false;
    bool m_isShowBoundingRect = false;
    QBrush m_brush = QColor(20, 80, 100);
    QImage m_image;
    QVariant m_userData;
};

#endif // ITEMBASE_H
