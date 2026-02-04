#include "Circle.h"

#include "LineRenderer.h"

Circle::Circle(const Vec2 pos, const float radius, const Colour colour) {
	set_position(pos);
	set_colour(colour);
	m_radius = radius;
}

void Circle::debug_draw(LineRenderer *lines) const {
	lines->DrawCircle(get_position(), m_radius, m_colour);
}
