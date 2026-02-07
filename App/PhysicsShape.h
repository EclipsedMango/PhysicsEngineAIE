#ifndef LINERENDERER_PHYSICSSHAPE_H
#define LINERENDERER_PHYSICSSHAPE_H

#include "RigidBody.h"
#include "Shape.h"
#include "Vec2.h"

class Shape;
class RigidBody;

class PhysicsShape {
public:
    PhysicsShape(Shape* shape, RigidBody* body);
    ~PhysicsShape();

    [[nodiscard]] Vec2 get_position() const { return m_body->get_position(); }
    [[nodiscard]] Shape* get_shape() const { return m_shape; }
    [[nodiscard]] RigidBody* get_rigid_body() const { return m_body; }

private:
    Shape* m_shape;
    RigidBody* m_body;
};


#endif //LINERENDERER_PHYSICSSHAPE_H