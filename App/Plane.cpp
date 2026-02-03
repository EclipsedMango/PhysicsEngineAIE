#include "Plane.h"

#include "LineRenderer.h"

Plane::Plane() : PhysicsObject(ShapeType::PLANE) {
	m_distanceToOrigin = 0;
	m_normal = { 0, 1 };
}

Plane::Plane(const Vec2 normal, const float distance) : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance) {
	m_normal.Normalise();
}

void Plane::fixed_update(Vec2 gravity, float timeStep) {

}

void Plane::draw(LineRenderer *lines) {
	const Vec2 PlaneCenter = m_distanceToOrigin * m_normal;

	const Vec2 dir1 = m_normal.GetRotatedBy90();
	const Vec2 dir2 = m_normal.GetRotatedBy270();

	lines->DrawLineSegment(PlaneCenter, PlaneCenter + 25 * dir1);
	lines->DrawLineSegment(PlaneCenter, PlaneCenter + 25 * dir2);
	lines->DrawLineWithArrow(PlaneCenter, PlaneCenter + m_normal * 2);
}

void Plane::reset_position() {
	m_distanceToOrigin = 0;
}
