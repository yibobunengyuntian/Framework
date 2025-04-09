#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "scene.h"

class View: public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);
    View(Scene *scene, QWidget *parent = nullptr);
    void setScene(Scene *scene);
    Scene *scene() const;
};

#endif // VIEW_H
