#include "contactlistener.h"

ContactListener::ContactListener() {}

void ContactListener::BeginContact(b2Contact *contact)
{
    emit BeginContactSignal(contact);
}

void ContactListener::EndContact(b2Contact *contact)
{
    emit EndContactSignal(contact);
}

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    emit PreSolveSignal(contact, oldManifold);
}

void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    emit PostSolveSignal(contact, impulse);
}
