
#include "PhysicsShape.h"

PhysicsShape::PhysicsShape(Shape* shape, RigidBody* body) : m_shape(shape), m_body(body) {}

PhysicsShape::~PhysicsShape() {
    delete m_shape;
    delete m_body;
}