#ifndef LINERENDERER_SHAPE_H
#define LINERENDERER_SHAPE_H

#include "Colour.h"
#include "Vec2.h"

class LineRenderer;

enum ShapeType {
    CIRCLE = 0,
    BOX = 1,
    PLANE = 2,
};

class Shape {
public:
    virtual ~Shape() = default;

    virtual void debug_draw(LineRenderer* lines) const = 0;
    [[nodiscard]] virtual ShapeType get_type() const = 0;

    void set_position(Vec2 const  &pos) { m_pos = pos; };
    [[nodiscard]] Vec2 get_position() const { return m_pos; }

    void set_colour(Colour const &colour) { m_colour = colour; }
    [[nodiscard]] virtual Colour get_colour() const { return m_colour; }
private:
    Vec2 m_pos;
    Colour m_colour;
};

#endif //LINERENDERER_SHAPE_H