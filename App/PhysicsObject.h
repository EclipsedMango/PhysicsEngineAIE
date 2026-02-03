#pragma once
#include "math.h"
#include "Vec2.h"

class LineRenderer;

enum class ShapeType : int {
	PLANE = 0,
	CIRCLE = 1,
	BOX = 2
};

class PhysicsObject {
protected:
	explicit PhysicsObject(const ShapeType shapeType) : m_shape_id(shapeType) {}

public:
    virtual ~PhysicsObject() = default;

	virtual void fixed_update(Vec2 gravity, float timeStep) = 0;
	virtual void draw(LineRenderer* lines) = 0;
	virtual void reset_position() = 0;

    virtual float get_inverse_mass() = 0;

    virtual Vec2 get_velocity() = 0;
    virtual void set_velocity(Vec2 velocity) = 0;

	virtual Vec2 get_position() = 0;
    virtual void set_position(Vec2 position) = 0;

private:
	ShapeType m_shape_id;
};
