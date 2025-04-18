#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsItem>
#include <QPainter>

#include "box2d/box2d.h"

class ItemBase: public QGraphicsItem
{

public:
    /**
     * @brief setPos    设置位置
     * @param pos       位置
     */
    void setPos(const QPointF &pos);
    /**
     * @brief setRotation   设置旋转量
     * @param angle         角度
     */
    void setRotation(const qreal &angle);
    /**
     * @brief setBrush  设置填充
     * @param brush     填充物
     */
    void setBrush(const QBrush &brush);
    /**
     * @brief setImage  设置图像
     * @param image     图像路径
     */
    void setImage(const QString &image);
    /**
     * @brief setMaterial           设置材质
     * @param friction              摩擦系数
     * @param restitution           弹性系数(反弹)
     * @param density               密度
     * @param restitutionThreshold  反弹阈值
     * @param isSensor              是否为传感器
     */
    void setMaterial(const float &friction = 0.2f,
                     const float &restitution = 0.0f,
                     const float &density = 0.0f,
                     const float &restitutionThreshold = 1.0f * b2_lengthUnitsPerMeter,
                     const float &isSensor = false);

    /**
     * @brief setShowBoundingRect   设置是否显示包围盒
     * @param is                    是否
     */
    void setShowBoundingRect(const bool &is = true);
    /**
     * @brief setShowShape          设置是否显示形状轮廓线
     * @param is                    是否
     */
    void setShowShape(const bool &is = true);

    /**
     * @brief setLinearVelocity 设置线速度
     * @param v                 线速度
     */
    void setLinearVelocity(const QPointF& v);
    // 返回线速度
    QPointF linearVelocity();

    // 返回包围盒(本地坐标)
    virtual QRectF boundingRect() const override;
    // 返回形状轮廓(本地坐标)
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
};

#endif // ITEMBASE_H
