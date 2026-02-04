#include "PhysicsScene.h"
#include "Vec2.h"
#include "imgui.h"
#include <algorithm>
#include "PhysicsObject.h"
#include "Circle.h"
#include <iterator>

#include "AABB.h"
#include "CollisionInfo.h"
#include "LineRenderer.h"

PhysicsScene::PhysicsScene() : m_gravity(Vec2(0, 0)), m_time_step(0.01f) {
	appInfo.appName = "Example Program";
    appInfo.grid.extent = 100;
}

PhysicsScene::~PhysicsScene() {

}

// happens after opengl init
void PhysicsScene::Initialise() {
    // set_gravity(Vec2(0, -9.81f));
}

void PhysicsScene::Update(const float delta) {
	ImGui::Begin("Window");
	ImGui::End();

	// static float accumulated_time = 0.0f;
 //    accumulated_time += delta;
 //
 //    while (accumulated_time >= m_time_step) {
 //        for (const auto actor : m_actors) {
 //            actor->fixed_update(m_gravity, m_time_step);
 //        }
 //
 //        accumulated_time -= m_time_step;
 //
 //        // collision checks
 //        for (int outer = 0; outer < m_actors.size() - 1; outer++) {
 //            for (int inner = outer + 1; inner < m_actors.size(); inner++) {
 //
 //            }
 //        }
 //    }
 //
 //    for (const auto actor : m_actors) {
 //        actor->draw(lines);
 //    }

    AABB test_box(cursorPos, 1.5f, 0.9f, Colour::WHITE);
    test_box.debug_draw(lines);

    AABB other_box(Vec2(2.1f, 3.0f), 1.0f, 0.5f, Colour::WHITE);
    other_box.debug_draw(lines);

    // Circle test_circle(Vec2(cursorPos), 0.8f, Colour::WHITE);
    // Circle other_circle(Vec2(-1.9f, 0.4f), 0.65f, Colour::WHITE);
    //
    // test_circle.debug_draw(lines);
    // other_circle.debug_draw(lines);

    // CollisionInfo overlap = CollisionInfo::check_shape_against_shape(&test_circle, &other_circle);
    // overlap.debug_draw(lines);

    CollisionInfo overlap2 = CollisionInfo::check_shape_against_shape(&other_box, &test_box);
    if (overlap2.is_collision()) {
        AABB disp_box = test_box;
        disp_box.set_position(disp_box.get_position() + overlap2.normal * overlap2.depth);
        disp_box.set_colour(Colour::GREY);
        disp_box.debug_draw(lines);
    }
}

void PhysicsScene::add_actor(PhysicsObject* actor) {
	// m_actors.push_back(actor);
}

void PhysicsScene::remove_actor(PhysicsObject* actor) {
    // const auto iterator = std::remove_if(m_actors.begin(), m_actors.end(), [actor](const PhysicsObject* a){
    //         if (a == actor) {
    //             delete a;
    //             return true;
    //         }
    //
    //         return false;
    // });
    //
    // m_actors.erase(iterator, m_actors.end());
}

void PhysicsScene::OnLeftClick() {
    // add_actor(new Circle(cursorPos, {0.0f, 0.0f}, 20.0f, 1.0f, Colour::BLUE));
}

