#ifndef LINERENDERER_PLANE_H
#define LINERENDERER_PLANE_H

#include "Colour.h"
#include "Shape.h"

class Plane : public Shape {
public:
	Plane(Vec2 normal, float distance, Colour colour);

	void debug_draw(LineRenderer* lines) const override;

	[[nodiscard]] ShapeType get_type() const override { return PLANE; }
	[[nodiscard]] Vec2 get_normal() const { return m_normal; }
	[[nodiscard]] float get_distance() const { return m_distance; }

protected:
	Vec2 m_normal;
	float m_distance;
};

#endif //LINERENDERER_PLANE_H