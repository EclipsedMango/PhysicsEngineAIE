#ifndef LINERENDERER_CIRCLE_H
#define LINERENDERER_CIRCLE_H

#pragma once
#include "Colour.h"
#include "Shape.h"

class Circle : public Shape {
public:
	Circle(Vec2 pos, float radius, Colour colour);

	void debug_draw(LineRenderer* lines) const override;

	[[nodiscard]] float get_radius() const { return m_radius; }
	[[nodiscard]] ShapeType get_type() const override { return CIRCLE; }
	
protected:
	float m_radius;
};

#endif //LINERENDERER_CIRCLE_H
