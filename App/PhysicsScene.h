#ifndef LINERENDERER_PHYSICSSCENE_H
#define LINERENDERER_PHYSICSSCENE_H

#include "Application.h"
#include "TextStream.h"
#include <vector>

class PhysicsShape;
class Shape;
class RigidBody;
class CollisionInfo;

class PhysicsScene : public Application {
public:
	PhysicsScene();
	~PhysicsScene() override;
	PhysicsScene(const PhysicsScene& other) = delete;
	PhysicsScene& operator=(const PhysicsScene& other) = delete;

	void Initialise() override;
	void Update(float delta) override;

	void add_actor(PhysicsShape* actor);
	void remove_actor(const PhysicsShape* actor);

	void set_gravity(const Vec2 gravity) { m_gravity = gravity; };

	void OnLeftClick() override;
	void OnRightClick() override;

	void resolve_collision(const CollisionInfo& info) const;
	static void resolve_penetration(RigidBody* body_a, RigidBody* body_b, const Vec2& normal, float depth);
	static void resolve_impulse(RigidBody* body_a, RigidBody* body_b, const Vec2& normal);

	PhysicsShape* find_actor_from_shape(const Shape* shape) const;

private:
	Vec2 m_gravity;
	float m_time_step;
	std::vector<PhysicsShape*> m_actors;
};

#endif //LINERENDERER_PHYSICSSCENE_H
