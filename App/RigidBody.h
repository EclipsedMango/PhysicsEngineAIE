// #pragma once
//
// #include "PhysicsObject.h"
// #include "Maths.h"
//
// class RigidBody : public PhysicsObject {
// public:
// 	RigidBody(ShapeType shape_id, Vec2 pos, Vec2 vel, float orientation, float mass);
// 	~RigidBody() override = default;
//
// 	void fixed_update(Vec2 gravity, float time_step) override;
// 	void apply_force(Vec2 force);
// 	void apply_impulse(Vec2 impulse);
// 	void apply_force_to_actor(RigidBody* other, Vec2 force);
//
// 	[[nodiscard]] Vec2 get_position() override { return m_position; }
// 	void set_position(const Vec2 position) override { m_position = position; }
//
// 	[[nodiscard]] float get_orientation() const { return m_orientation; }
// 	void set_orientation(const float orientation) { m_orientation = orientation; }
//
// 	[[nodiscard]] Vec2 get_velocity() override { return m_velocity; }
// 	void set_velocity(const Vec2 velocity) override { m_velocity = velocity; }
//
// 	[[nodiscard]] float get_mass() const { return m_mass; }
// 	float get_inverse_mass() override { return m_inv_mass; }
//
// 	[[nodiscard]] ShapeType get_type() {  }
//
// 	void reset_position() override;
// protected:
// 	Vec2 m_position;
// 	Vec2 m_velocity;
// 	Vec2 m_force_accumulated;
//
// 	float m_mass;
// 	float m_inv_mass;
// 	float m_orientation;
// };
