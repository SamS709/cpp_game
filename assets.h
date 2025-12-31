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
    Vec2 get_pos() const {return pos;}
    float get_x() const { return pos.x; }
    float get_y() const { return pos.y; }
    QColor get_color() const { return color; }
    void set_color(int r, int g, int b, int a) { color = QColor(r, g, b, a); }
    void set_x(float x_) { pos.x = x_; }
    void set_y(float y_) { pos.y = y_; }
    float get_h() const { return dims.y; }
    float get_w() const { return dims.x; }
    void set_h(float h_) { dims.y = h_; }
    void set_w(float w_) { dims.x = w_; }
    virtual void draw(QPainter &painter) = 0;
    bool get_has_contact() const { return has_contact; }
    void set_has_contact(bool c) {has_contact = c;}

protected:

    Vec2 pos = {0.0, 0.0};
    Vec2 dims = {0.5, 0.5};
    QColor color = QColor(200,100,100);
    bool has_contact = false;

};

class Plane: public Asset {

public:

    Plane() = default;
    Plane(Vec2 dim_, Vec2 norm_);
    Plane(Vec2 pos_, Vec2 dims_, Vec2 norm_);
    void draw(QPainter &painter) ;
    Vec2 get_norm() const {return norm;}

protected:
    Vec2 norm {1.0, 0.0};

};

class Rectangle: public Asset {

public:

    Rectangle() = default;
    ~Rectangle();
    Rectangle(Vec2 dim_);
    Rectangle(Vec2 pos_, Vec2 dims_);
    void draw(QPainter &painter) ;

    Plane *plane_x_0;
    Plane *plane_x_1;
    Plane *plane_y_0;
    Plane *plane_y_1;

};


class MovableAsset : public Asset{

public:

    MovableAsset(Vec2 pos_, Vec2 v_, float mass_);
    MovableAsset(float mass_);
    MovableAsset() = default;

    Vec2 get_v() const { return v;}
    float get_v_x() const { return v.x; }
    float get_v_y() const { return v.y; }
    void set_v(Vec2 v_) { v = v_; }
    void set_v_x(float v_x_) { v.x = v_x_; }
    void set_v_y(float v_y_) { v.y = v_y_; }

    Vec2 get_pos_expected() const {return pos_exp;}
    
    float get_x_expected() const { return pos_exp.x; }
    float get_y_expected() const { return pos_exp.y; }
    void set_x_expected(float x_expected_) { pos_exp.x = x_expected_; }
    void set_y_expected(float y_expected_) { pos_exp.y = y_expected_; }

    float get_mass() { return mass; }
    void set_mass(float mass_) { mass = mass_; }

    float get_damp() {return damp; }
    void set_damp(float damp_) {damp = damp_; }

    void update_vel(float dt){ v.x = (pos_exp.x - pos.x) / dt; v.y = (pos_exp.y - pos.y) / dt; }
    void update_expected_pos(float dt){ pos_exp.x = pos.x + v.x * dt; pos_exp.y = pos.y + v.y * dt; }
    void update_pos(){ pos.x = pos_exp.x; pos.y = pos_exp.y; }
    void update_expected_pos_collision(float delta_x, float delta_y){ pos_exp.x += delta_x; pos_exp.y += delta_y; }
    void update_expected_pos_collision(Vec2 pos_exp_){ pos_exp += pos_exp_ ;}

    
    virtual float get_rest() const { return 0.0; } // Default restitution coefficient

protected:

    Vec2 pos_exp {0.0, 0.0};
    Vec2 v {0.0, 0.0};
    float mass {0.0};
    float damp = 0.98;
};

class MovableCircle : public MovableAsset{

public:
    MovableCircle() = default;
    MovableCircle(float mass_);
    MovableCircle(float mass_, float radius_);
    MovableCircle(Vec2 pos_, Vec2 v_, float mass_, float radius_);


    float get_radius() const { return radius; }
    void set_radius(float radius_) { radius = radius_; }

    float get_rest() const override { return 0.8; } 

    void draw(QPainter &painter) ;

protected:
    float radius {0.5};

};

class MovableRectangle : public MovableAsset{

public:
    MovableRectangle() = default;
    MovableRectangle(float mass_);
    MovableRectangle(Vec2 dims_, float mass_);
    MovableRectangle(Vec2 pos_, Vec2 dims_, Vec2 v_, float mass_);

protected:

};

struct StaticConstraint {
    int index;     // Index of the particle
    Vec2 normal;           // Contact normal
    float penetration;     // Penetration depth
    Vec2 contactPoint;     // Contact point on the collider
};

struct DynamicConstraint {
    int particleIndex1;
    int particleIndex2;
    Vec2 normal;
    float penetration;
};




// class SphereCollider : public Collider {
// public:
//     Vec2 center;
//     float radius;
    
//     SphereCollider(Vec2 center, float radius) 
//         : center(center), radius(radius) {}
    
//     std::optional<StaticConstraint> checkContact(const MovableCircle& particle, int index) const override {
//         Vec2 diff = particle.get_pos_expected() - center;
//         float distance = diff.length();
//         float minDist = particle.get_radius() + radius;
//         float penetration = minDist - distance;
        
//         if (penetration > 0 && distance > 0.0001f) {
//             StaticConstraint constraint;
//             constraint.particleIndex = index;
//             constraint.normal = diff.normalized();
//             constraint.penetration = penetration;
//             constraint.contactPoint = center + constraint.normal * radius;
//             return constraint;
//         }
//         return std::nullopt;
//     }
    

// };

#endif
