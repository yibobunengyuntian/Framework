#include "view.h"


View::View(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setMouseTracking(true);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // 启用渲染抗锯齿
    this->setRenderHint(QPainter::Antialiasing);
    // 文本抗锯齿
    this->setRenderHint(QPainter::TextAntialiasing);

    QTransform transform;
    transform.reset();
    transform.scale(1, -1);
    this->setTransform(transform);
}

View::View(Scene *scene, QWidget *parent)
    :QGraphicsView(scene, parent)
{
    this->setMouseTracking(true);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QTransform transform;
    transform.reset();
    transform.scale(1, -1);
    this->setTransform(transform);
}

void View::setScene(Scene *scene)
{
    QGraphicsView::setScene(scene);
}

Scene *View::scene() const
{
    return static_cast<Scene*>(QGraphicsView::scene());
}

void View::mousePressEvent(QMouseEvent *event)
{
    emit mousePress(event);
    QGraphicsView::mousePressEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease(event);
    QGraphicsView::mouseReleaseEvent(event);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit mouseDoubleClick(event);
    QGraphicsView::mouseDoubleClickEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event);
    QGraphicsView::mouseMoveEvent(event);
}
