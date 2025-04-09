#include "view.h"


View::View(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setMouseTracking(true);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

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
