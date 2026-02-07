#ifndef LINERENDERER_RIGIDBODY_H
#define LINERENDERER_RIGIDBODY_H

#pragma once

#include "Maths.h"

class RigidBody {
public:
	RigidBody(Vec2 pos, Vec2 vel, float orientation, float mass);
	~RigidBody() = default;

	void fixed_update(Vec2 gravity, float time_step);
	void apply_force(Vec2 force);
	void apply_impulse(Vec2 impulse);
	void apply_force_to_actor(RigidBody* other, Vec2 force);

	[[nodiscard]] Vec2 get_position() const { return m_position; }
	void set_position(const Vec2 position) { m_position = position; }

	[[nodiscard]] float get_orientation() const { return m_orientation; }
	void set_orientation(const float orientation) { m_orientation = orientation; }

	[[nodiscard]] Vec2 get_velocity() const { return m_velocity; }
	void set_velocity(const Vec2 velocity) { m_velocity = velocity; }

	[[nodiscard]] float get_mass() const { return m_mass; }
	void set_mass(const float mass) { m_mass = mass; }

	[[nodiscard]] float get_inverse_mass() const { return m_inv_mass; }

	void reset_position();
protected:
	Vec2 m_position;
	Vec2 m_velocity;
	Vec2 m_force_accumulated;

	float m_mass;
	float m_inv_mass;
	float m_orientation;
};

#endif //LINERENDERER_RIGIDBODY_H
