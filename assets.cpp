#include "assets.h"

Asset::Asset(Vec2 pos_) 
    : pos(pos_)
    {

    }

Rectangle::Rectangle(Vec2 dims_)
    : dims(dims_)
    {

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

    }

MovableCircle::MovableCircle(double mass_, double radius_)
    : MovableAsset(mass_)
    , radius(radius_)
    {

    }

MovableCircle::MovableCircle(Vec2 pos_, Vec2 v_, double mass_, double radius_)
    : MovableAsset(pos_, v_, mass_)
    , radius(radius_)
    {

    }
