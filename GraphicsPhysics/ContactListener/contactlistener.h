#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <QObject>
#include "box2d/box2d.h"

class ContactListener: public QObject, public b2ContactListener
{
    Q_OBJECT
public:
    ContactListener();

Q_SIGNALS:
    void BeginContactSignal(b2Contact* contact);
    void EndContactSignal(b2Contact* contact);
    void PreSolveSignal(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolveSignal(b2Contact* contact, const b2ContactImpulse* impulse);

protected:
    // 在这里处理接触开始的逻辑
    void BeginContact(b2Contact* contact) override;

    // 在这里处理接触结束的逻辑
    void EndContact(b2Contact* contact) override;

    // 在这里处理接触预解决的逻辑
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

    // 在这里处理接触后解决的逻辑
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

};

#endif // CONTACTLISTENER_H
