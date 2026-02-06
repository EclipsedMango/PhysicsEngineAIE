
#include <algorithm>

#include "CollisionInfo.h"
#include "LineRenderer.h"

void CollisionInfo::debug_draw(LineRenderer *lines) const {
    lines->DrawLineWithArrow(m_shape_b->get_position(), m_shape_b->get_position() + m_normal, Colour::GREEN);
}

CollisionFn collision_table[SHAPE_COUNT][SHAPE_COUNT] = {
    { CollisionInfo::circle_vs_circle, CollisionInfo::circle_vs_aabb, CollisionInfo::circle_vs_plane },
    { CollisionInfo::aabb_vs_circle, CollisionInfo::aabb_vs_aabb, CollisionInfo::aabb_vs_plane },
    { CollisionInfo::plane_vs_circle, CollisionInfo::plane_vs_aabb, nullptr }
};

CollisionInfo CollisionInfo::check_shape_against_shape(Shape *shape_a, Shape *shape_b) {
    const CollisionFn fn = collision_table[shape_a->get_type()][shape_b->get_type()];
    if (!fn) return {};
    return fn(shape_a, shape_b);
}

CollisionInfo CollisionInfo::circle_vs_circle(Shape *shape_a, Shape *shape_b) {
    return check_circle_against_circle(dynamic_cast<Circle*>(shape_a), dynamic_cast<Circle*>(shape_b));
}

CollisionInfo CollisionInfo::aabb_vs_aabb(Shape *shape_a, Shape *shape_b) {
    return check_aabb_against_aabb(dynamic_cast<AABB*>(shape_b), dynamic_cast<AABB*>(shape_a));
}

CollisionInfo CollisionInfo::plane_vs_circle(Shape *shape_a, Shape *shape_b) {
    return check_plane_against_circle(dynamic_cast<Plane*>(shape_a), dynamic_cast<Circle*>(shape_b));
}

CollisionInfo CollisionInfo::plane_vs_aabb(Shape *shape_a, Shape *shape_b) {
    return check_plane_against_aabb(dynamic_cast<Plane*>(shape_a), dynamic_cast<AABB*>(shape_b));
}

CollisionInfo CollisionInfo::circle_vs_plane(Shape *shape_a, Shape *shape_b) {
    CollisionInfo info = check_plane_against_circle(dynamic_cast<Plane*>(shape_b), dynamic_cast<Circle*>(shape_a));
    info.m_normal = -info.m_normal;
    std::swap(info.m_shape_a, info.m_shape_b);
    return info;
}

CollisionInfo CollisionInfo::aabb_vs_plane(Shape *shape_a, Shape *shape_b) {
    CollisionInfo info = check_plane_against_aabb(dynamic_cast<Plane*>(shape_b), dynamic_cast<AABB*>(shape_a));
    info.m_normal = -info.m_normal;
    std::swap(info.m_shape_a, info.m_shape_b);
    return info;
}

CollisionInfo CollisionInfo::aabb_vs_circle(Shape *shape_a, Shape *shape_b) {
    return check_aabb_against_circle(dynamic_cast<AABB*>(shape_a), dynamic_cast<Circle*>(shape_b));
}

CollisionInfo CollisionInfo::circle_vs_aabb(Shape *shape_a, Shape *shape_b) {
    auto info = check_aabb_against_circle(dynamic_cast<AABB*>(shape_b), dynamic_cast<Circle*>(shape_a));
    info.m_normal = -info.m_normal;
    std::swap(info.m_shape_a, info.m_shape_b);
    return info;
}

CollisionInfo CollisionInfo::check_circle_against_circle(Circle *circle_a, Circle *circle_b) {
    CollisionInfo result;
    result.m_shape_a = circle_a;
    result.m_shape_b = circle_b;

    const Vec2 dist = circle_b->get_position() - circle_a->get_position();

    const float distance = dist.GetMagnitude();
    result.m_normal = dist.GetNormalised();

    result.m_depth = -(distance - circle_a->get_radius() - circle_b->get_radius());
    return result;
}

CollisionInfo CollisionInfo::check_aabb_against_aabb(AABB *box_a, AABB *box_b) {
    CollisionInfo result;
    result.m_shape_a = box_a;
    result.m_shape_b = box_b;

    const float dx = box_a->get_position().x - box_b->get_position().x;
    const float overlap_x = box_a->get_half_width() + box_b->get_half_width() - std::abs(dx);

    // AABB collision only happens if both x and y overlap, so exit early id no collision
    if (overlap_x <= 0.0f) {
        return result;
    }

    const float dy = box_a->get_position().y - box_b->get_position().y;
    const float overlap_y = box_a->get_half_height() + box_b->get_half_height() - std::abs(dy);
    if (overlap_y <= 0.0f) {
        return result;
    }

    if (overlap_x < overlap_y) {
        result.m_depth = overlap_x;
        result.m_normal = Vec2(dx < 0.0f ? 1.0f : -1.0f, 0.0f);
    } else {
        result.m_depth = overlap_y;
        result.m_normal = Vec2(0.0f, dy < 0.0f ? 1.0f : -1.0f);
    }

    return result;
}

CollisionInfo CollisionInfo::check_aabb_against_circle(AABB *box_a, Circle *circle_b) {
    CollisionInfo result;
    result.m_shape_a = box_a;
    result.m_shape_b = circle_b;

    const Vec2 box_pos = box_a->get_position();
    const Vec2 circle_pos = circle_b->get_position();

    const Vec2 distance = circle_pos - box_pos;

    const float clamped_x = std::clamp(distance.x, -box_a->get_half_width(), box_a->get_half_width());
    const float clamped_y = std::clamp(distance.y, -box_a->get_half_height(), box_a->get_half_height());

    // on AABB
    const Vec2 closest_point = box_pos + Vec2(clamped_x, clamped_y);

    const Vec2 closest_to_circle = circle_pos - closest_point;
    const float distance_squared = closest_to_circle.GetMagnitudeSquared();
    const float radius = circle_b->get_radius();

    // outside, exit early
    if (distance_squared > pow(radius, 2)) {
        return result;
    }

    const float distance_magnitude = closest_to_circle.GetMagnitude();
    if (distance_magnitude < 0.0001f) {
        const float overlap_x = radius + box_a->get_half_width() - std::abs(distance.x);
        const float overlap_y = radius + box_a->get_half_height() - std::abs(distance.y);

        if (overlap_x < overlap_y) {
            result.m_depth = overlap_x;
            result.m_normal = Vec2(distance.x < 0.0f ? -1.0f : 1.0f, 0.0f);
        } else {
            result.m_depth = overlap_y;
            result.m_normal = Vec2(0.0f, distance.y < 0.0f ? -1.0f : 1.0f);
        }
    } else {
        result.m_depth = radius - distance_magnitude;
        result.m_normal = closest_to_circle / distance_magnitude;
    }

    return result;

}

CollisionInfo CollisionInfo::check_plane_against_circle(Plane *plane_a, Circle *circle_b) {
    CollisionInfo result;
    result.m_shape_a = plane_a;
    result.m_shape_b = circle_b;

    const float signed_distance = Dot(plane_a->get_normal(), circle_b->get_position()) - plane_a->get_distance();
    if (signed_distance > circle_b->get_radius()) {
        return result;
    }

    result.m_depth  = circle_b->get_radius() - signed_distance;
    result.m_normal = plane_a->get_normal();

    return result;
}

CollisionInfo CollisionInfo::check_plane_against_aabb(Plane *plane_a, AABB *box_b) {
    CollisionInfo result;
    result.m_shape_a = plane_a;
    result.m_shape_b = box_b;

    const Vec2 planeNormal = plane_a->get_normal();
    const float planeDistance = plane_a->get_distance();

    const Vec2 boxCenter = box_b->get_position();
    const float halfWidth = box_b->get_half_width();
    const float halfHeight = box_b->get_half_height();

    const float dist = Dot(boxCenter, planeNormal) - planeDistance;
    const float r = halfWidth  * std::abs(planeNormal.x) + halfHeight * std::abs(planeNormal.y);

    if (dist > r) {
        return result;
    }

    result.m_depth = r - dist;
    result.m_normal = planeNormal;

    return result;
}
