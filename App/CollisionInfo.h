#pragma once
#include "Vec2.h"
#include "Shape.h"
#include "Circle.h"
#include "AABB.h"
#include "Plane.h"

constexpr int SHAPE_COUNT = 3;

class LineRenderer;

struct CollisionInfo {
    Vec2 normal;
    float depth = -1;
    Shape* shape_a {};
    Shape* shape_b {};

    [[nodiscard]] bool is_collision() const { return depth > 0; }
    void resolve_collision(Shape* shape_a, Shape* shape_b);
    void debug_draw(LineRenderer* lines) const;

    static CollisionInfo check_shape_against_shape(Shape* shape_a, Shape* shape_b);

    static CollisionInfo circle_vs_circle(Shape* shape_a, Shape* shape_b);
    static CollisionInfo aabb_vs_circle(Shape* shape_a, Shape* shape_b);

    static CollisionInfo circle_vs_aabb(Shape* shape_a, Shape* shape_b);
    static CollisionInfo aabb_vs_aabb(Shape* shape_a, Shape* shape_b);

    static CollisionInfo plane_vs_circle(Shape* shape_a, Shape* shape_b);
    static CollisionInfo plane_vs_aabb(Shape* shape_a, Shape* shape_b);

    static CollisionInfo circle_vs_plane(Shape* shape_a, Shape* shape_b);
    static CollisionInfo aabb_vs_plane(Shape* shape_a, Shape* shape_b);

    static CollisionInfo check_circle_against_circle(Circle* circle_a, Circle* circle_b);
    static CollisionInfo check_aabb_against_aabb(AABB* box_a, AABB* box_b);
    static CollisionInfo check_aabb_against_circle(AABB* box_a, Circle* circle_b);
    static CollisionInfo check_plane_against_circle(Plane* plane_a, Circle* circle_b);
    static CollisionInfo check_plane_against_aabb(Plane* plane_a, AABB* box_n);
};

using CollisionFn = CollisionInfo(*)(Shape*, Shape*);
