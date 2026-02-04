#ifndef LINERENDERER_AABB_H
#define LINERENDERER_AABB_H
#include "Shape.h"

class AABB : public Shape {
public:
    AABB(Vec2 pos, float half_width, float half_height, Colour colour);

    void debug_draw(LineRenderer *lines) const override;
    [[nodiscard]] ShapeType get_type() const override { return BOX; }

    [[nodiscard]] float get_half_width() const { return m_half_width; }
    [[nodiscard]] float get_half_height() const { return m_half_height; }

protected:
    float m_half_width;
    float m_half_height;
};


#endif //LINERENDERER_AABB_H