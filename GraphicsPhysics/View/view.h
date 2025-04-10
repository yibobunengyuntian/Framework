#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "scene.h"

class View: public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);
    View(Scene *scene, QWidget *parent = nullptr);
    void setScene(Scene *scene);
    Scene *scene() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseDoubleClick(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
};

#endif // VIEW_H
