#include "PhysicsScene.h"

#include <algorithm>

#include "Vec2.h"
#include "imgui.h"
#include "PhysicsObject.h"

#include "CollisionInfo.h"
#include "PhysicsShape.h"

PhysicsScene::PhysicsScene() : m_gravity(Vec2(0, 0)), m_time_step(0.01f) {
	appInfo.appName = "Example Program";
    appInfo.grid.extent = 100;
    appInfo.grid.show = false;
}

PhysicsScene::~PhysicsScene() {

}

// happens after opengl init
void PhysicsScene::Initialise() {
    set_gravity(Vec2(0, -9.81f));

    const Vec2 planePos = Vec2(0.0f, 1.0f) * -5.0f;
    PhysicsShape* floor = new PhysicsShape(new Plane(Vec2(0.0f, 1.0f), -5.0f, Colour::GREEN), new RigidBody(planePos, Vec2(0,0), 0, 0.0f));
    m_actors.push_back(floor);

    const Vec2 planePos2 = Vec2(30.0f, 90.0f) * -5.0f;
    PhysicsShape* floor2 = new PhysicsShape(new Plane(Vec2(30.0f, 90.0f), -5.0f, Colour::GREEN), new RigidBody(planePos2, Vec2(0, 0), 0, 0.0f));
    m_actors.push_back(floor2);

    const Vec2 planePos3 = Vec2(-30.0f, 90.0f) * -5.0f;
    PhysicsShape* floor3 = new PhysicsShape(new Plane(Vec2(-30.0f, 90.0f), -5.0f, Colour::GREEN), new RigidBody(planePos3, Vec2(0, 0), 0, 0.0f));
    m_actors.push_back(floor3);

    PhysicsShape* shape = new PhysicsShape(new Circle(cursorPos, 0.8f, Colour::WHITE), new RigidBody(cursorPos, Vec2(0.25f, 8.0f), 1.0f, 1.0f));
    m_actors.push_back(shape);
}

void PhysicsScene::Update(const float delta) {
	ImGui::Begin("Window");
	ImGui::End();

	static float accumulated_time = 0.0f;
    accumulated_time += delta;

    while (accumulated_time >= m_time_step) {
        for (const auto actor : m_actors) {
            actor->get_rigid_body()->fixed_update(m_gravity, m_time_step);
            actor->get_shape()->set_position(actor->get_rigid_body()->get_position());
        }

        accumulated_time -= m_time_step;

        // collision checks
        for (int outer = 0; outer < m_actors.size() - 1; outer++) {
            for (int inner = outer + 1; inner < m_actors.size(); inner++) {
                CollisionInfo info = CollisionInfo::check_shape_against_shape(m_actors[outer]->get_shape(), m_actors[inner]->get_shape());

                if (info.is_collision()) {
                    resolve_collision(info);
                }
            }
        }
    }

    for (const auto actor : m_actors) {
        actor->get_shape()->debug_draw(lines);
    }
}

void PhysicsScene::add_actor(PhysicsShape* actor) {
	m_actors.push_back(actor);
}

void PhysicsScene::remove_actor(PhysicsShape* actor) {
    const auto iterator = std::remove_if(m_actors.begin(), m_actors.end(), [actor](const PhysicsShape* a){
            if (a == actor) {
                delete a;
                return true;
            }

            return false;
    });

    m_actors.erase(iterator, m_actors.end());
}

void PhysicsScene::OnLeftClick() {
    add_actor(new PhysicsShape(new Circle(cursorPos, 0.8f, Colour::WHITE), new RigidBody(cursorPos, Vec2(0.25f, 8.0f), 1.0f, 1.0f)));
}

void PhysicsScene::OnRightClick() {
    add_actor(new PhysicsShape(new AABB(cursorPos, 0.75f, 0.5f, Colour::WHITE), new RigidBody(cursorPos, Vec2(0.25f, 8.0f), 1.0f, 1.0f)));
}

void PhysicsScene::resolve_collision(const CollisionInfo& info) const {
    if (!info.is_collision()) {
        return;
    }

    const PhysicsShape* actor_a = find_actor_from_shape(info.m_shape_a);
    const PhysicsShape* actor_b = find_actor_from_shape(info.m_shape_b);

    RigidBody* body_a = actor_a ? actor_a->get_rigid_body() : nullptr;
    RigidBody* body_b = actor_b ? actor_b->get_rigid_body() : nullptr;

    resolve_penetration(body_a, body_b, info.m_normal, info.m_depth);
    resolve_impulse(body_a, body_b, info.m_normal);

    if (actor_a) actor_a->get_shape()->set_position(body_a->get_position());
    if (actor_b) actor_b->get_shape()->set_position(body_b->get_position());
}

void PhysicsScene::resolve_penetration(RigidBody* body_a, RigidBody* body_b, const Vec2& normal, const float depth) {
    if (!body_a && !body_b) return;

    const float inv_mass_a = body_a ? body_a->get_inverse_mass() : 0.0f;
    const float inv_mass_b = body_b ? body_b->get_inverse_mass() : 0.0f;
    const float total_inv_mass = inv_mass_a + inv_mass_b;

    if (total_inv_mass <= 0) {
        return;
    }

    if (body_a) {
        const float move_per = inv_mass_a / total_inv_mass;
        body_a->set_position(body_a->get_position() - normal * depth * move_per);
    }

    if (body_b) {
        const float move_per = inv_mass_b / total_inv_mass;
        body_b->set_position(body_b->get_position() + normal * depth * move_per);
    }
}

void PhysicsScene::resolve_impulse(RigidBody* body_a, RigidBody* body_b, const Vec2& normal) {
    if (!body_a && !body_b) {
        return;
    }

    const Vec2 vel_a = body_a ? body_a->get_velocity() : Vec2{0, 0};
    const Vec2 vel_b = body_b ? body_b->get_velocity() : Vec2{0, 0};

    const Vec2 relative_velocity = vel_b - vel_a;
    const float vel_along_normal = Dot(relative_velocity, normal);

    if (vel_along_normal > 0.0f) {
        return;
    }

    constexpr float restitution = 0.2f;

    const float inv_mass_a = body_a ? body_a->get_inverse_mass() : 0.0f;
    const float inv_mass_b = body_b ? body_b->get_inverse_mass() : 0.0f;

    float j = -(1.0f + restitution) * vel_along_normal;
    j /= inv_mass_a + inv_mass_b;

    const Vec2 impulse = normal * j;

    if (body_a) {
        body_a->apply_impulse(-impulse);
    }

    if (body_b) {
        body_b->apply_impulse( impulse);
    }
}

// O(n) lookup ideally replace with hashmap or something fast
PhysicsShape* PhysicsScene::find_actor_from_shape(const Shape* shape) const {
    for (PhysicsShape* actor : m_actors) {
        if (actor->get_shape() == shape) {
            return actor;
        }
    }

    return nullptr;
}