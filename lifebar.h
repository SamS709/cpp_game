#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "QPainter"
#include "QPixmap"
#include "assets.h"
#include <vector>

using namespace std;

class Lifebar: public Rectangle {

public:

    void set_percentage(float p_) {p = p_;}
    void draw(QPainter &painter) ;
    void reload_image();
    Lifebar();
    Lifebar(Vec2 dim_);
    Lifebar(Vec2 pos_, Vec2 dims_);

private:
    QPixmap sprite;
    float im_scale = 1.5;
    float p = 1.0;

};


class Bomb: public MovableCircle {
public:
    Bomb() {inv_mass = 1 / mass; };
    Bomb(float damage_);
    Bomb(float damage_, float mass_);
    Bomb(float damage_, float mass_, float radius_);
    Bomb(Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_);

    void explode();
    void set_exploded(bool exploded_){exploded = exploded_;}
    bool get_exploded() { return exploded; }
    float get_damage(){ return damage; }
    void set_creator(int creator_) {creator = creator_;}
    int get_creator() {return creator; }

private:
    float damage {20.0};
    bool exploded = false;
    int creator;
};


class BonusBox: public Rectangle {

public:

    void set_bonus(int b) {b = b;}
    int get_bonus() const { return b; }
    void draw(QPainter &painter) ;
    void load_images();
    void reload_image();
    void update(std::vector<MovableCircle*>& particles);
    void activate(Vec2 v_char, std::vector<MovableCircle*>& particles, int i);
    bool get_activate() { return activated; }
    bool get_finished() { return finished; }
    BonusBox(float dt_);
    BonusBox(Vec2 dim_,float dt_);
    BonusBox(Vec2 pos_, Vec2 dims_, float dt_);

private:
    vector<Bomb*> bombs;
    int n_bombs = 3;
    QVector<QPixmap> sprites;
    float scale;
    float t = 0.0f;
    float dt;
    float bomb_time = 0.0f;
    float total_bomb_time = 5.0f;
    int b = 0;
    bool visual = true;
    bool first_activated = true;
    bool activated = false;
    bool finished = false;

};

#endif