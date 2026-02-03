#include "Circle.h"

#include "LineRenderer.h"

Circle::Circle(const Vec2 pos, const Vec2 vel, const float mass, const float radius, const Colour colour) : RigidBody(ShapeType::CIRCLE, pos, vel, 0, mass) {
	m_radius = radius;
	m_colour = colour;
}

void Circle::draw(LineRenderer *lines) {
	lines->DrawCircle(m_position, m_radius, m_colour);
}

bool Circle::check_collision_circle(Circle *other) {
	const double x = pow(other->get_position().x - get_position().x, 2);
	const double y = pow(other->get_position().y - get_position().y, 2);

	if (x + y <= pow(other->get_radius() + get_radius(), 2)) {
		m_colliding = true;
		return true;
	}

	m_colliding = false;
	return false;
}
