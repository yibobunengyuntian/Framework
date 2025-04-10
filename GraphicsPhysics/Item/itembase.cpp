#include "itembase.h"
#include "scene.h"
#include "maths.h"


ItemBase::ItemBase(b2World *world, b2BodyDef bd)
{
    bd.userData.pointer = reinterpret_cast<uintptr_t>(this);
    m_pBody = world->CreateBody(&bd);
    setFlag(QGraphicsItem::ItemIsSelectable, true); // 允许选中
}

ItemBase::~ItemBase()
{
    m_pBody->GetWorld()->DestroyBody(m_pBody);
    m_pBody = nullptr;
    if(m_pB2Shape)
    {
        delete m_pB2Shape;
        m_pB2Shape = nullptr;
    }
}

void ItemBase::setPos(const QPointF &pos)
{
    m_pBody->SetTransform(pointToVec2(pos/Scene::m_pix_meter), m_pBody->GetAngle());
    updateTransform();
}

void ItemBase::setRotation(const qreal &angle)
{
    m_pBody->SetTransform(m_pBody->GetPosition(), qDegreesToRadians(angle));
    updateTransform();
}

void ItemBase::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

void ItemBase::setImage(const QString &image)
{
    m_image.load(image);
    if(m_image.isNull())
    {
        return;
    }
    m_image = m_image.mirrored(false, true);
    QRectF rect = m_shape.boundingRect();
    if(m_image.width()*1.0/m_image.height() >= rect.width()*1.0/rect.height())
    {
        m_image = m_image.scaledToWidth(rect.width(), Qt::SmoothTransformation);
    }
    else
    {
        m_image = m_image.scaledToHeight(rect.height(), Qt::SmoothTransformation);
    }
}

void ItemBase::setMaterial(const float &friction, const float &restitution, const float &density, const float &restitutionThreshold, const float &isSensor)
{
    if(m_pB2Shape == nullptr)
    {
        b2PolygonShape *pPolygonShape = new b2PolygonShape;
        pPolygonShape->SetAsBox (this->boundingRect().width()/2.0/Scene::m_pix_meter, this->boundingRect().height()/2.0/Scene::m_pix_meter);
        m_pB2Shape = pPolygonShape;
    }
    b2FixtureDef fixtureDef;
    fixtureDef.shape = m_pB2Shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.restitutionThreshold = restitutionThreshold;
    fixtureDef.isSensor = isSensor;

    m_pBody->DestroyFixture(m_pBody->GetFixtureList());
    m_pBody->CreateFixture(&fixtureDef);
}

void ItemBase::setShowBoundingRect(const bool &is)
{
    m_isShowBoundingRect = is;
}

void ItemBase::setShowShape(const bool &is)
{
    m_isShowShape = is;
}

void ItemBase::setLinearVelocity(const QPointF &v)
{
    m_pBody->SetLinearVelocity(vector2DToVec2(QVector2D(v / Scene::m_pix_meter)));
}

void ItemBase::setUserData(const QVariant &data)
{
    m_userData = data;
}

QVariant ItemBase::userData()
{
    return m_userData;
}

void ItemBase::updateTransform()
{
    QPointF pos = QPointF(m_pBody->GetPosition().x*Scene::m_pix_meter,m_pBody->GetPosition().y*Scene::m_pix_meter);
    qreal angle = qRadiansToDegrees(m_pBody->GetAngle());
    if(pos != QGraphicsItem::pos())
    {
        QGraphicsItem::setPos(pos);
    }
    if(angle != QGraphicsItem::rotation())
    {
        QGraphicsItem::setRotation(angle);
    }
}

QRectF ItemBase::boundingRect() const
{
    int gap = 3;
    QRectF rect = m_shape.boundingRect();
    return QRectF(rect.topLeft() - QPointF(gap, gap), rect.bottomRight() + QPointF(gap, gap));
}

QPainterPath ItemBase::shape() const
{
    return m_shape;
}

void ItemBase::paintShape(QPainter *painter)
{
    painter->drawPath(shape());
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);


    //画图形
    painter->save();
    paintItem(painter);
    painter->restore();

    if(!m_image.isNull())
    {
        // painter->save();
        QPointF offset = this->shape().boundingRect().center();
        painter->drawImage(QRectF(- m_image.width()/2.0 + offset.x(), -m_image.height()/2.0 + offset.y(), m_image.width(), -m_image.height()),m_image);
    }

    //画边框线
    if(m_isShowShape){
        painter->save();
        paintShape(painter);
        painter->restore();
    }

    //画包围盒
    if(m_isShowBoundingRect){
        painter->save();
        // painter->setPen(QPen(Qt::black,1,Qt::DashLine));
        painter->drawRect(this->boundingRect());
        painter->restore();
    }
}
