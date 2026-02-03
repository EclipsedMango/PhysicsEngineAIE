#include "RigidBody.h"


RigidBody::RigidBody(const ShapeType shape_id, const Vec2 pos, const Vec2 vel, const float orientation, const float mass) : PhysicsObject(shape_id), m_position(pos), m_velocity(vel), m_mass(mass), m_orientation(orientation) {
	m_inv_mass = 1.0f / mass;
}

void RigidBody::fixed_update(const Vec2 gravity, const float time_step) {
	apply_force(gravity * m_mass);

	const Vec2 acc = m_force_accumulated * m_inv_mass;

	m_velocity += acc * time_step;
	m_position += m_velocity * time_step;

	m_force_accumulated = { 0, 0 };
}

void RigidBody::apply_force(const Vec2 force) {
	m_force_accumulated += force;
}

void RigidBody::apply_impulse(const Vec2 impulse) {
	m_velocity += impulse * m_inv_mass;
}

void RigidBody::apply_force_to_actor(RigidBody* other, const Vec2 force) {
	other->apply_force(force);
	apply_force(-1.0f * force);
}

void RigidBody::reset_position() {
	m_position = { 0,0 };
}


