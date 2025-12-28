#include "assets.h"

// ...existing code...
std::optional<StaticConstraint> PlaneCollider::checkContact(const MovableCircle& particle, int index) const {
    const Vec2 center = particle.get_pos_expected();
    const Vec2 toCenter = center - init_point;

    // Distance along normal (penetration must be positive)
    float distance = toCenter.dot(normal);
    float penetration = particle.get_radius() - distance;
    if (penetration <= 0) return std::nullopt;

    // Finite segment test along plane tangent
    Vec2 tangent = Vec2{-normal.y, normal.x}; // perpendicular to normal
    float s = toCenter.dot(tangent);          // local coord along the plane

    // Require the circle center to be within [radius, width - radius]
    if (s < 0.0 || s > (width)) {
        return std::nullopt;
    }

    StaticConstraint constraint;
    constraint.particleIndex = index;
    constraint.normal = normal;
    constraint.penetration = penetration;
    constraint.contactPoint = center - normal * particle.get_radius();
    return constraint;
}
// ...existing code...


Asset::Asset(Vec2 pos_) 
    : pos(pos_)
    , dims({0.5, 0.5})
    {

    }

Asset::Asset(Vec2 pos_, Vec2 dims_) 
    : pos(pos_)
    , dims(dims_)
    {

    }

Rectangle::Rectangle(Vec2 dims_)
    : Asset({0.0, 0.0}, dims_)
    {

    }

Rectangle::Rectangle(Vec2 pos_, Vec2 dims_)
    : Asset(pos_, dims_)
    {

    }

void Rectangle::draw(QPainter &painter){
    painter.setBrush(get_color());
    painter.drawRect(get_x(), get_y(), get_w(), get_h());
}
    



MovableAsset::MovableAsset(double mass_)
    : Asset()
    , mass(mass_)
    {
        
    }

MovableAsset::MovableAsset(Vec2 pos_, Vec2 v_, double mass_)
    : Asset(pos_)
    , v(v_)
    , mass(mass_)
    {
        
    }


MovableCircle::MovableCircle(double mass_)
    : MovableAsset(mass_)
    {
        dims = {radius * 2.0, radius * 2.0};
    }

MovableCircle::MovableCircle(double mass_, double radius_)
    : MovableAsset(mass_)
    , radius(radius_)
    {
        dims = {radius * 2.0, radius * 2.0};
    }

MovableCircle::MovableCircle(Vec2 pos_, Vec2 v_, double mass_, double radius_)
    : MovableAsset(pos_, v_, mass_)
    , radius(radius_)
    {
        dims = {2.0 * radius, 2.0 * radius};
    }

void MovableCircle::draw(QPainter &painter){
    const double r = get_radius();
    painter.drawEllipse(QPointF(get_x(), get_y()), r, r);
}

MovableRectangle::MovableRectangle(Vec2 dims_, double mass_)
    : MovableAsset(mass_)
    {
        dims = dims_;
    }

MovableRectangle::MovableRectangle(Vec2 pos_, Vec2 dims_, Vec2 v_, double mass_)
    : MovableAsset(pos_, v_, mass_)
    {
        dims = dims_;
    }
