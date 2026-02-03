#pragma once 
#include "RigidBody.h"
#include "Colour.h"

class Circle : public RigidBody {
public:
	Circle(Vec2 pos, Vec2 vel, float mass, float radius, Colour colour);
	~Circle() override = default;

	void draw(LineRenderer* lines) override;

	[[nodiscard]] float get_radius() const { return m_radius; }
	[[nodiscard]] Colour get_colour() const { return m_colour; }

	bool check_collision_circle(Circle* other);
	
protected:
	float m_radius;
	Colour m_colour;

	bool m_colliding {};
};
