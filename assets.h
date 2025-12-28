#ifndef ASSET_H
#define ASSET_H
#include <QPainter>
#include <cmath>

struct Vec2 {
    float x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    
    Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(float s) const { return Vec2(x / s, y / s); }
    
    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(float s) { x *= s; y *= s; return *this; }
    
    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSquared() const { return x * x + y * y; }
    
    Vec2 normalized() const {
        float len = length();
        return len > 0.0001f ? Vec2(x / len, y / len) : Vec2(0, 0);
    }
    
    float dot(const Vec2& v) const { return x * v.x + y * v.y; }
};

class Asset {

public:

    Asset(Vec2 pos_);
    Asset(Vec2 pos_, Vec2 dims_);
    Asset() = default;
    double get_x() { return pos.x; }
    double get_y() { return pos.y; }
    QColor get_color() { return color; }
    void set_color(int r, int g, int b, int a) { color = QColor(r, g, b, a); }
    void set_x(double x_) { pos.x = x_; }
    void set_y(double y_) { pos.y = y_; }
    double get_h() const { return dims.y; }
    double get_w() const { return dims.x; }
    void set_h(double h_) { dims.y = h_; }
    void set_w(double w_) { dims.x = w_; }

protected:

    Vec2 pos = {0.0, 0.0};
    Vec2 dims = {0.5, 0.5};
    QColor color = QColor(200,100,100);

};

class Rectangle: public Asset {

public:

    Rectangle() = default;
    Rectangle(Vec2 dim_);
    Rectangle(Vec2 pos_, Vec2 dims_);
    void draw(QPainter &painter);



protected:

};

class MovableAsset : public Asset{

public:

    MovableAsset(Vec2 pos_, Vec2 v_, double mass_);
    MovableAsset(double mass_);
    MovableAsset() = default;

    double get_v_x() const { return v.x; }
    double get_v_y() const { return v.y; }
    void set_v_x(double v_x_) { v.x = v_x_; }
    void set_v_y(double v_y_) { v.y = v_y_; }

    double get_x_expected() const { return pos_exp.x; }
    double get_y_expected() const { return pos_exp.y; }
    void set_x_expected(double x_expected_) { pos_exp.x = x_expected_; }
    void set_y_expected(double y_expected_) { pos_exp.y = y_expected_; }

    double get_mass() { return mass; }
    void set_mass(double mass_) { mass = mass_; }

    void update_vel(double dt){ v.x = (pos_exp.x - pos.x) / dt; v.y = (pos_exp.y - pos.y) / dt; }
    void update_expected_pos(double dt){ pos_exp.x = pos.x + v.x * dt; pos_exp.y = pos.y + v.y * dt; }
    void update_pos(){ pos.x = pos_exp.x; pos.y = pos_exp.y; }
    void update_expected_pos_collision(double delta_x, double delta_y){ pos_exp.x += delta_x; pos_exp.y += delta_y; }
    virtual double get_rest() const { return 0.0; } // Default restitution coefficient

protected:

    Vec2 pos_exp {0.0, 0.0};
    Vec2 v {0.0, 0.0};
    double mass {0.0};
};

class MovableCircle : public MovableAsset{

public:
    MovableCircle() = default;
    MovableCircle(double mass_);
    MovableCircle(double mass_, double radius_);
    MovableCircle(Vec2 pos_, Vec2 v_, double mass_, double radius_);

    double get_radius() const { return radius; }
    void set_radius(double radius_) { radius = radius_; }

    double get_rest() const override { return 0.8; } // Higher bounce for particles

protected:
    double radius {0.5};

};

class MovableRectangle : public MovableAsset{

public:
    MovableRectangle() = default;
    MovableRectangle(Vec2 dims_, double mass_);
    MovableRectangle(Vec2 pos_, Vec2 dims_, Vec2 v_, double mass_);

protected:

};

#endif
