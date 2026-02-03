#include "PhysicsScene.h"
#include "RigidBody.h"
#include "Vec2.h"
#include "imgui.h"
#include <algorithm>
#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include "CollisionInfo.h"
#include <iostream>
#include <iterator>

PhysicsScene::PhysicsScene() : m_gravity(Vec2(0, 0)), m_time_step(0.01f) {
	appInfo.appName = "Example Program";
    appInfo.grid.extent = 100;
}

PhysicsScene::~PhysicsScene() {
	for (const PhysicsObject* actor : m_actors) {
		delete actor;
	}
}

// happens after opengl init
void PhysicsScene::Initialise() {
	
}

void PhysicsScene::Update(const float delta) {
	ImGui::Begin("Window");
	ImGui::End();

	static float accumulated_time = 0.0f;
    accumulated_time += delta;

    while (accumulated_time >= m_time_step) {
        for (const auto actor : m_actors) {
            actor->fixed_update(m_gravity, m_time_step);
        }

        accumulated_time -= m_time_step;

        // collision checks
        for (int outer = 0; outer < m_actors.size() - 1; outer++) {
            for (int inner = outer + 1; inner < m_actors.size(); inner++) {
                PhysicsObject* obj1 = m_actors[outer];
                PhysicsObject* obj2 = m_actors[inner];

                const Circle* sphere1 = dynamic_cast<Circle*>(obj1);
                const Circle* sphere2 = dynamic_cast<Circle*>(obj2);

                if (sphere1 != nullptr && sphere2 != nullptr) {
                    // TODO do the necessary maths in here
                    // TODO if the spheres touch, set their velocities to zero for now
                }
            }
        }

    }

    for (const auto actor : m_actors) {
        actor->draw(lines);
    }
}

void PhysicsScene::add_actor(PhysicsObject* actor) {
	m_actors.push_back(actor);
}

void PhysicsScene::remove_actor(PhysicsObject* actor) {
    const auto iterator = std::remove_if(m_actors.begin(), m_actors.end(), [actor](const PhysicsObject* a){
            if (a == actor) {
                delete a;
                return true;
            }

            return false;
    });

    m_actors.erase(iterator, m_actors.end());
}

void PhysicsScene::OnLeftClick() {
    // add_actor(new Circle(cursorPos, {0.0f, 0.0f}, 20.0f, 1.0f, Colour::BLUE));
}


// WE ARE DOING B->A FOR NORMALS

CollisionInfo PhysicsScene::Sphere2Plane(PhysicsObject *A, PhysicsObject *B) {

    // NOTE: For circle-plane collisions, the collision normal will be either -1 * planeNormal or the planeNormal. 
        CollisionInfo info;
        Circle* CircleA = static_cast<Circle*>(A);
        Plane* PlaneB = static_cast<Plane*>(B);

        if (abs(Dot(CircleA->get_position(), PlaneB->get_normal()) - PlaneB->get_distance()) <= CircleA->get_radius())  {
            info.isColliding = true;

            // NOTE: We flip the direction of the ternary expression to comply with B->A
            float distanceToPlane = Dot(CircleA->get_position(), PlaneB->get_normal()) - PlaneB->get_distance();
            (distanceToPlane > 0) ? info.collisionNormal = PlaneB->get_normal() : info.collisionNormal = -1.0f * PlaneB->get_normal();
            info.penetrationDepth = CircleA->get_radius() - abs(distanceToPlane);
        }
    return info;

   }

CollisionInfo PhysicsScene::Plane2Sphere(PhysicsObject *A, PhysicsObject *B) {

    // NOTE: For circle-plane collisions, the collision normal will be either -1 * planeNormal or the planeNormal. 
            CollisionInfo info;
            Plane* PlaneA = static_cast<Plane*>(A);
            Circle* CircleB = static_cast<Circle*>(B);
            
            if (abs(Dot(CircleB->get_position(), PlaneA->get_normal()) - PlaneA->get_distance()) <= CircleB->get_radius())  {
                info.isColliding = true;

                // Get normal direction
                float distanceToPlane = PlaneA->get_distance() - Dot(CircleB->get_position(), PlaneA->get_normal());
                (distanceToPlane > 0) ? info.collisionNormal = PlaneA->get_normal() : info.collisionNormal = -1.0f * PlaneA->get_normal();
                info.penetrationDepth = CircleB->get_radius() - abs(distanceToPlane);
            }
        
        return info;
}


CollisionInfo PhysicsScene::Sphere2Sphere(PhysicsObject *A, PhysicsObject *B) {

    CollisionInfo info;

    Circle* CircleA = static_cast<Circle*>(A);
    Circle* CircleB = static_cast<Circle*>(B);

    // TODO: Clean this up
    if ((CircleA->get_position() - CircleB->get_position()).GetMagnitudeSquared() < (CircleA->get_radius() + CircleB->get_radius()) * (CircleA->get_radius() + CircleB->get_radius())) {
        info.isColliding = true;
        info.collisionNormal = (CircleA->get_position() - CircleB->get_position()).Normalise();
        info.penetrationDepth = (CircleA->get_radius() + CircleB->get_radius()) - (CircleA->get_position() - CircleB->get_position()).GetMagnitude();
    }

    return info;
}

CollisionInfo PhysicsScene::Plane2Plane(PhysicsObject *A, PhysicsObject *B) {
    // don't do anything;
    return CollisionInfo();
}

//NOTE: Only handles linear cases right now
void PhysicsScene::ResolveCollisions(PhysicsObject* A, PhysicsObject* B, CollisionInfo& info) {
    
    float totalInverseMass = A->get_inverse_mass() + B->get_inverse_mass();
    if (totalInverseMass > 0.0f) { Vec2 correction = (info.penetrationDepth / totalInverseMass) * info.collisionNormal;
        A->set_position( A->get_position() + A->get_inverse_mass() * correction );
        B->set_position( B->get_position() - B->get_inverse_mass() * correction );
    }
    
    Vec2 relativeVelocity = A->get_velocity() - B->get_velocity();

    if (Dot(relativeVelocity, info.collisionNormal) < 0) {
        float impulseMagnitude = -1.7 * (Dot(relativeVelocity, info.collisionNormal)) / (A->get_inverse_mass() + B->get_inverse_mass());
        Vec2 newVelocityA = A->get_velocity() + A->get_inverse_mass() * (impulseMagnitude * info.collisionNormal);
        Vec2 newVelocityB = B->get_velocity() - B->get_inverse_mass() *  (impulseMagnitude * info.collisionNormal);
        A->set_velocity(newVelocityA);
        B->set_velocity(newVelocityB);
    }
  
}

