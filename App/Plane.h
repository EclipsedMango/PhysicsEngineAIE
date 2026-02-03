#pragma once 
#include "PhysicsObject.h"
#include "Maths.h"

class Plane : public PhysicsObject {
public:
	Plane();
	Plane(Vec2 normal, float distance);

	void fixed_update(Vec2 gravity, float timeStep) override;

	void draw(LineRenderer* lines) override;

	void set_position(const Vec2 position) override {}
	[[nodiscard]] Vec2 get_position() override { return {0,0}; }

	void set_velocity(const Vec2 velocity) override {}
	[[nodiscard]] Vec2 get_velocity() override {return {0.0f, 0.0f};}

    [[nodiscard]] float get_inverse_mass() override { return 0.0f; }
	[[nodiscard]] Vec2 get_normal() const { return m_normal; }
	[[nodiscard]] float get_distance() const { return m_distanceToOrigin; }

	void reset_position() override;
protected:
	Vec2 m_normal;
	float m_distanceToOrigin;
	
};

