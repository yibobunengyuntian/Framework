#include "scene.h"
#include "maths.h"

int Scene::m_pix_meter = 30;

Scene::Scene(const QVector2D &gravity, const int &pix_meter)
{
    m_pix_meter = pix_meter;
    m_pWorld = new b2World(vector2DToVec2(gravity));
    this->startTimer ( 1000 / m_fps);
    m_pContactListener = new ContactListener;
    m_pWorld->SetContactListener(m_pContactListener);
    connect(m_pContactListener, SIGNAL(BeginContactSignal(b2Contact*)), this, SLOT(onBeginContact(b2Contact*)));
}

ItemCircle *Scene::CreateCircle(const QPointF &center, const qreal &r, b2BodyDef bd)
{
    return CreateItem<ItemCircle>(bd, center, r);
}

ItemRect *Scene::CreateRect(const QPointF &center, const qreal &w, const qreal &h, b2BodyDef bd)
{
    return CreateItem<ItemRect>(bd, center, w, h);
}

ItemPolygon *Scene::CreatePolygon(const QList<QPointF> &points, b2BodyDef bd)
{
    return CreateItem<ItemPolygon>(bd, points);
}

ItemEdge *Scene::CreateEdge(const QPointF &p1, const QPointF &p2, b2BodyDef bd)
{
    return CreateItem<ItemEdge>(bd, p1, p2);
}

ItemChain *Scene::CreateChan(const QList<QPointF> &points, b2BodyDef bd)
{
    return CreateItem<ItemChain>(bd, points);
}

void Scene::DestroyItem(ItemBase *item)
{
    m_destroyItems.append(item);
}

b2Joint *Scene::CreateJoint(const b2JointDef &def)
{
    return m_pWorld->CreateJoint(&def);
}

void Scene::DestroyJoint(b2Joint *joint)
{
    m_pWorld->DestroyJoint(joint);
}

void Scene::start()
{
    m_isStop = false;
}

void Scene::stop()
{
    m_isStop = true;
}

void Scene::setShowAllShape(const bool &is)
{
    for(auto item:m_items)
    {
        item->setShowShape(is);
    }
}

void Scene::setShowAllBoundingRect(const bool &is)
{
    for(auto item:m_items)
    {
        item->setShowBoundingRect(is);
    }
}

void Scene::onBeginContact(b2Contact* contact)
{
    auto A = reinterpret_cast<ItemBase*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    auto B = reinterpret_cast<ItemBase*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    // 创建一个b2WorldManifold对象来存储世界坐标的碰撞点
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    b2Vec2 worldPoint = worldManifold.points[0];
    QPointF pos = vec2ToPoint(worldPoint)*m_pix_meter;
    emit signalBeginContact(A, B, pos);
}

void Scene::timerEvent(QTimerEvent *event)
{
    auto destroyItems = m_destroyItems;
    for(auto item: destroyItems)
    {
        m_destroyItems.removeOne(item);
        m_items.removeOne(item);
        if(this->items().contains(item))
        {
            QGraphicsScene::removeItem(item);
            delete item;
        }
    }

    if(m_isStop)
    {
        return;
    }
    m_pWorld->Step(1.0f / m_fps, m_velocityIterations, m_positionIterations);
    for(ItemBase *item:m_items)
    {
        item->updateTransform();
    }
    QGraphicsScene::timerEvent(event);
    emit signalTimerEvent();
}

void Scene::clear()
{
    m_destroyItems.append(this->items());
}
