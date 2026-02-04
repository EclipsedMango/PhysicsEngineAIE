
#include "AABB.h"

#include "LineRenderer.h"

AABB::AABB(const Vec2 pos, const float half_width, const float half_height, const Colour colour) {
    set_position(pos);
    set_colour(colour);
    m_half_width = half_width;
    m_half_height = half_height;
}

void AABB::debug_draw(LineRenderer *lines) const {
    const Vec2 top_left { get_position().x - m_half_width, get_position().y + m_half_height };
    const Vec2 top_right { get_position().x + m_half_width, get_position().y + m_half_height };
    const Vec2 bottom_right { get_position().x + m_half_width, get_position().y - m_half_height };
    const Vec2 bottom_left { get_position().x - m_half_width, get_position().y - m_half_height };

    lines->AddPointToLine(top_left, get_colour());
    lines->AddPointToLine(top_right, get_colour());
    lines->AddPointToLine(bottom_right, get_colour());
    lines->AddPointToLine(bottom_left, get_colour());

    lines->FinishLineLoop();
}
