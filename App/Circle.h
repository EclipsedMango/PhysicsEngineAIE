#pragma once
#include "Colour.h"
#include "Shape.h"

class Circle : public Shape {
public:
	Circle(Vec2 pos, float radius, Colour colour);
	~Circle() override = default;

	void debug_draw(LineRenderer* lines) const override;

	[[nodiscard]] float get_radius() const { return m_radius; }
	[[nodiscard]] Colour get_colour() const { return m_colour; }
	[[nodiscard]] ShapeType get_type() const override { return CIRCLE; }
	
protected:
	float m_radius;
	Colour m_colour;
};
