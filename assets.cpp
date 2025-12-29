#include "assets.h"

std::optional<StaticConstraint> PlaneCollider::checkContact(const MovableCircle& particle, int index) const {
    const Vec2 center = particle.get_pos_expected();
    const float radius = particle.get_radius();
    const Vec2 toCenter = center - get_pos();

    // Distance along normal (penetration must be positive)
    float distance = toCenter.dot(norm);
    float penetration = radius - distance;
    if (penetration <= 0) return std::nullopt;

    // Finite segment test along plane tangent
    Vec2 tangent = Vec2{-norm.y, norm.x}; // perpendicular to normal
    float s = toCenter.dot(tangent);          // local coord along the plane
    
    StaticConstraint constraint;
    constraint.particleIndex = index;

    // Check collision type based on position along plane
    if (s < 0.0f) {
        // Left edge: circular cap around init_point
        float dist = (center - get_pos()).length();
        if (dist > radius) return std::nullopt;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - get_pos()).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else if (s > get_w()) {
        // Right edge: circular cap around end_point
        Vec2 end_point = get_pos() + tangent * get_w();
        float dist = (center - end_point).length();
        if (dist > radius) return std::nullopt;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - end_point).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else {
        // Flat face collision: use original plane normal
        constraint.normal = norm;
        constraint.penetration = penetration;
        constraint.contactPoint = center - norm * radius;
    }
    
    return constraint;
}


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


Plane::Plane(Vec2 dims_, Vec2 norm_)
    : Asset({0.0, 0.0}, {dims_.x, 0.0})
    , norm(norm_.normalized())
    {

    }

Plane::Plane(Vec2 pos_, Vec2 dims_, Vec2 norm_)
    : Asset(pos_, {dims_.x, 0.0})
    , norm(norm_.normalized())
    {

    }

void Plane::draw(QPainter &painter) {
    Vec2 tangent(-norm.y, norm.x);
    Vec2 p1 = get_pos();
    Vec2 p2 = p1 + tangent * get_w();
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawLine(QPointF(p1.x, p1.y), QPointF(p2.x, p2.y));
    painter.setBrush(get_color());
    painter.drawRect(get_x(), get_y(), get_w(), get_h());
}

std::optional<StaticConstraint> AssetCollider::checkContact(const MovableCircle& particle, int index) const {
    const Vec2 center = particle.get_pos_expected();
    const float radius = particle.get_radius();
    const Vec2 toCenter = center - get_pos();

    // Distance along normal (penetration must be positive)
    float distance = toCenter.dot(norm);
    float penetration = radius - distance;
    if (penetration <= 0) return std::nullopt;

    // Finite segment test along plane tangent
    Vec2 tangent = Vec2{-norm.y, norm.x}; // perpendicular to normal
    float s = toCenter.dot(tangent);          // local coord along the plane
    
    StaticConstraint constraint;
    constraint.particleIndex = index;

    // Check collision type based on position along plane
    if (s < 0.0f) {
        // Left edge: circular cap around init_point
        float dist = (center - get_pos()).length();
        if (dist > radius) return std::nullopt;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - get_pos()).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else if (s > get_w()) {
        // Right edge: circular cap around end_point
        Vec2 end_point = get_pos() + tangent * get_w();
        float dist = (center - end_point).length();
        if (dist > radius) return std::nullopt;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - end_point).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else {
        // Flat face collision: use original plane normal
        constraint.normal = norm;
        constraint.penetration = penetration;
        constraint.contactPoint = center - norm * radius;
    }
    
    return constraint;
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
