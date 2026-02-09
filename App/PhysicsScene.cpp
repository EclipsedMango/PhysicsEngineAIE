#include "PhysicsScene.h"

#include <algorithm>
#include <iostream>

#include "Vec2.h"
#include "imgui.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"
#include "PhysicsShape.h"

static ShapeType current_shape = CIRCLE;
const char* shape_names[] = { "Circle", "Box" };

static float circle_rad = 1.0f;
static float box_half_width = 2.0f;
static float box_half_height = 2.0f;
static float shape_mass = 1.0f;
static float shape_velocity[2];

PhysicsScene::PhysicsScene() : m_player(nullptr), m_gravity(Vec2(0, 0)), m_time_step(0.01f) {
    appInfo.appName = "Example Program";
    appInfo.grid.show = false;
}

PhysicsScene::~PhysicsScene() {
    for (const PhysicsShape* actor : m_actors) {
        delete actor;
    }

    m_actors.clear();
}

// happens after opengl init
void PhysicsScene::Initialise() {
    set_gravity(Vec2(0, -9.81f));

    const Vec2 planePos = Vec2(0.0f, 1.0f) * -5.0f;
    PhysicsShape* floor = new PhysicsShape(new Plane(Vec2(0.0f, 1.0f), -5.0f, Colour::WHITE), new RigidBody(planePos, Vec2(0,0), 0, 0.0f, STATIC));
    m_actors.push_back(floor);

    const Vec2 planePos2 = Vec2(30.0f, 90.0f) * -5.0f;
    PhysicsShape* floor2 = new PhysicsShape(new Plane(Vec2(30.0f, 90.0f), -5.0f, Colour::WHITE), new RigidBody(planePos2, Vec2(0, 0), 0, 0.0f, STATIC));
    m_actors.push_back(floor2);

    m_player = new PhysicsShape(new Circle(Vec2(0, 10), 0.5f, Colour::CYAN.Darken()), new RigidBody(Vec2(0, 10), Vec2(0, 100), 0, 100.0f, KINEMATIC));
    m_actors.push_back(m_player);

    // const Vec2 planePos3 = Vec2(-30.0f, 90.0f) * -5.0f;
    // PhysicsShape* floor3 = new PhysicsShape(new Plane(Vec2(-30.0f, 90.0f), -5.0f, Colour::WHITE), new RigidBody(planePos3, Vec2(0, 0), 0, 0.0f));
    // m_actors.push_back(floor3);
}

void PhysicsScene::Update(const float delta) {
	ImGui::Begin("Tools");
    ImGui::SetWindowSize(ImVec2(250, 300));

    if (ImGui::CollapsingHeader("Drawing", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Debug Normals", &m_show_debug_normals);
        ImGui::Checkbox("Debug Contacts", &m_show_debug_contacts);
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Object Creation", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Combo("Types", reinterpret_cast<int*>(&current_shape), shape_names, IM_ARRAYSIZE(shape_names));

        switch (current_shape) {
            case CIRCLE: {
                ImGui::SliderFloat("Radius", &circle_rad, 0.1f, 10.0f);
                break;
            }
            case BOX: {
                ImGui::SliderFloat("Half Width", &box_half_width, 0.1f, 10.0f);
                ImGui::SliderFloat("Half Height", &box_half_height, 0.1f, 10.0f);
                break;
            }
            default: {}
        }

        ImGui::InputFloat("Mass", &shape_mass);
        ImGui::InputFloat2("Velocity", shape_velocity);
    }

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
                Shape* shape_a = m_actors[outer]->get_shape();
                Shape* shape_b = m_actors[inner]->get_shape();

                if (shape_a->get_type() == PLANE && shape_b->get_type() == PLANE) {
                    continue;
                }

                CollisionInfo info = CollisionInfo::check_shape_against_shape(shape_a, shape_b);

                if (info.is_collision()) {
                    resolve_collision(info);
                    if (m_show_debug_normals) {
                        info.debug_draw(lines);
                    }

                    if (m_show_debug_contacts) {
                        info.debug_draw_contact(lines);
                    }
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

void PhysicsScene::remove_actor(const PhysicsShape* actor) {
    const auto it = std::find(m_actors.begin(), m_actors.end(), actor);

    if (it != m_actors.end()) {
        delete *it;
        m_actors.erase(it);
    }
}

void PhysicsScene::OnLeftClick() {
    Shape* shape;
    switch (current_shape) {
        case CIRCLE: {
            shape = new Circle(cursorPos, circle_rad, Colour::WHITE);
            break;
        }
        case BOX: {
            shape = new AABB(cursorPos, box_half_width, box_half_height, Colour::WHITE);
            break;
        }
        default: {
            shape = new Circle(cursorPos, 1.0f, Colour::RED);
        }
    }

    RigidBody* body = new RigidBody(cursorPos, Vec2(shape_velocity[0], shape_velocity[1]), 1.0f, shape_mass, DYNAMIC);
    add_actor(new PhysicsShape(shape, body));
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

    constexpr float restitution = 0.15f;

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

    Vec2 tangent = relative_velocity - Dot(relative_velocity, normal) * normal;
    if (tangent.GetMagnitude() < 0.0001) {
        return;
    }

    tangent.Normalise();

    float jt = -Dot(relative_velocity, tangent);
    jt /= inv_mass_a + inv_mass_b;

    constexpr float mu = 0.2f;
    const float max_friction = j * mu;

    // Coulomb made this apparently idk
    jt = std::clamp(jt, -max_friction, max_friction);
    const Vec2 friction_impulse = tangent * jt;

    if (body_a) body_a->apply_impulse(-friction_impulse);
    if (body_b) body_b->apply_impulse( friction_impulse);
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