#include "assets.h"

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
        plane_x_0 = new Plane(dims_, {-1.0, 0.0});
        plane_x_1 = new Plane(dims_, {1.0, 0.0});
        plane_y_0 = new Plane(dims_, {0.0, 1.0});
        plane_y_1 = new Plane(dims_, {0.0, -1.0});

    }

Rectangle::Rectangle(Vec2 pos_, Vec2 dims_)
    : Asset(pos_, dims_)
    {
        plane_x_0 = new Plane(Vec2(pos.x, pos.y + dims.y), Vec2(dims.y, 0.0), Vec2(-1.0, 0.0));
        plane_x_1 = new Plane(Vec2(pos.x + dims.x, pos.y), Vec2(dims.y, 0.0), Vec2(1.0, 0.0));
        plane_y_0 = new Plane(pos, dims, Vec2(0.0, -1));
        plane_y_1 = new Plane(Vec2(pos.x + dims.x, pos.y + dims.y), dims, Vec2(0.0, 1.0));
    }

Rectangle::~Rectangle()
    {
        delete plane_x_0;
        delete plane_x_1;
        delete plane_y_0;
        delete plane_y_1;
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

MovableRectangle::MovableRectangle(double mass_)
    : MovableAsset(mass_)
    {
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
