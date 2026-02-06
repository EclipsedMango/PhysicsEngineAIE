#include "Plane.h"

#include "LineRenderer.h"

Plane::Plane(const Vec2 normal, const float distance, const Colour colour) {
    m_normal = normal.GetNormalised();
    m_distance = distance;
    m_colour = colour;
    set_position(m_normal * m_distance);
}

void Plane::debug_draw(LineRenderer *lines) const {
    const Vec2 center = m_normal * m_distance;
    const Vec2 tangent = m_normal.GetRotatedBy90();

    lines->DrawLineSegment(center - tangent * 50.0f, center + tangent * 50.0f, m_colour);
    lines->DrawLineWithArrow(center, center + m_normal * 2.5f, m_colour);
}
