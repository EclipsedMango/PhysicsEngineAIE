#pragma once 
#include "Colour.h"
#include "PhysicsObject.h"
#include "Maths.h"
#include "Shape.h"

class Plane : public Shape {
public:
	Plane(Vec2 normal, float distance, Colour colour);
	~Plane() override = default;

	void debug_draw(LineRenderer* lines) const override;

	[[nodiscard]] Colour get_colour() const override { return m_colour; }
	[[nodiscard]] ShapeType get_type() const override { return PLANE; }

	[[nodiscard]] Vec2 get_normal() const { return m_normal; }
	[[nodiscard]] float get_distance() const { return m_distance; }

protected:
	Vec2 m_normal;
	float m_distance;
	Colour m_colour;
};

