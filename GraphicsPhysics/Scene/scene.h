#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QVector2D>
#include "box2d/box2d.h"
#include "contactlistener.h"
#include "itemcircle.h"
#include "itemrect.h"
#include "itempolygon.h"
#include "itemedge.h"
#include "itemchain.h"

class Scene: public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Scene     场景构造函数
     * @param gravity   重力加速度
     * @param pix_meter 多少像素拟为1米
     */
    Scene(const QVector2D &gravity = QVector2D(0.0f, -9.8f), const int &pix_meter = 30);

    /**
     * @brief CreateCircle  创建一个圆形图元
     * @param center        圆心位置
     * @param r             半径
     * @param bd            刚体数据
     * @return              圆形图元
     */
    ItemCircle *CreateCircle(const QPointF &center, const qreal &r, b2BodyDef bd = b2BodyDef());

    /**
     * @brief CreateRect    创建一个矩形图元
     * @param center        中心位置
     * @param w             宽
     * @param h             高
     * @param bd            刚体数据
     * @return              形图元
     */
    ItemRect *CreateRect(const QPointF &center, const qreal &w, const qreal &h, b2BodyDef bd = b2BodyDef());

    /**
     * @brief CreatePolygon 创建一个多边形图元(注意!必须为凸多边形)
     * @param points        三个及以上顶点
     * @param bd            刚体数据
     * @return              多边形图元
     */
    ItemPolygon *CreatePolygon(const QList<QPointF> &points, b2BodyDef bd = b2BodyDef());

    /**
     * @brief CreateEdge    创建一条边缘线(主要用于创建静态环境中的边界或平台等线性碰撞体)
     * @param p1            起点
     * @param p2            终点
     * @param bd            刚体数据
     * @return              一条边缘线
     */
    ItemEdge *CreateEdge(const QPointF &p1, const QPointF &p2, b2BodyDef bd = b2BodyDef());

    /**
     * @brief CreateChan    创建一个多段线
     * @param points        多段线的端点
     * @param bd            刚体数据
     * @return              一个多段线
     */
    ItemChain *CreateChan(const QList<QPointF> &points, b2BodyDef bd = b2BodyDef());

    // 删除图元
    void DestroyItem(ItemBase* item);

    // 创建关节(目前还没完善)
    b2Joint* CreateJoint(const b2JointDef &def);
    // 删除关节
    void DestroyJoint(b2Joint* joint);

    static int m_pix_meter; //多少像素为一米

protected:
    void timerEvent(QTimerEvent *event) override;

    template<typename T, typename... Args>
    T* CreateItem(Args&&... args) {
        auto item = new T(m_pWorld, std::forward<Args>(args)...);
        addItem(item);
        m_items.append(item);
        return item;
    }

public Q_SLOTS:
    void clear();
    void start();
    void stop();

    void setShowAllShape(const bool &is = true);
    void setShowAllBoundingRect(const bool &is = true);

Q_SIGNALS:
    void signalTimerEvent();
    void signalBeginContact(ItemBase *A, ItemBase *B, QPointF pos);

protected Q_SLOTS:
    void onBeginContact(b2Contact* contact);

private:
    b2World *m_pWorld = nullptr;
    ContactListener *m_pContactListener = nullptr;
    QList<ItemBase *> m_items;
    bool m_isStop = true;
    float m_fps = 60.0;
    int m_velocityIterations = 8; //这是速度迭代次数，用于解算速度约束（例如摩擦力和弹性）。增加迭代次数可以提高模拟的稳定性和准确性，但同样会增加计算量。
    int m_positionIterations = 3; //这是位置迭代次数，用于解算位置约束（例如刚体之间的接触）。增加迭代次数可以减少物体之间的穿透现象，但也会提高计算量。

    QList<QGraphicsItem *> m_destroyItems;
};

#endif // SCENE_H
