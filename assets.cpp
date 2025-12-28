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
        dims = {radius * 2.0, radius * 2.0};
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
