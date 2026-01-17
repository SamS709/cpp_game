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
    void draw(QPainter &painter) override;
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
    Bomb(const QVector<QPixmap>& bomb_sprites_) {inv_mass = 1 / mass; sprites = bomb_sprites_;};
    Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float explosion_time_);
    Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float mass_, float explosion_time_);
    Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float mass_, float radius_, float explosion_time_);
    Bomb(const QVector<QPixmap>& bomb_sprites_, Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_, float explosion_time_);

    void explode();
    void set_explosion_started(bool exploded_){explosion_started = exploded_;}
    bool get_explosion_started() { return explosion_started; }
    void set_explosion_finished(bool exploded_){explosion_finished = exploded_;}
    bool get_explosion_finished() { return explosion_finished; }
    float get_damage(){ return damage; }
    void set_creator(int creator_) {creator = creator_;}
    int get_creator() {return creator; }
    void draw(QPainter& painter) override;
    void rescale_sprites(const QVector<QPixmap>& bomb_sprites_);
    void update(float dt);


private:
    QVector<QPixmap> sprites;
    QPixmap *current_sprite = nullptr;
    float damage {20.0};
    bool explosion_started = false;
    bool explosion_finished = false;
    int creator;
    float total_explosion_time;
    float explosion_time = 0.0;
};


class BonusBox: public Rectangle {

public:

    void set_bonus(int b) {b = b;}
    int get_bonus() const { return b; }
    void draw(QPainter &painter) override;
    void update(std::vector<MovableCircle*>& particles);
    void activate(Vec2 v_char, std::vector<MovableCircle*>& particles, int i);
    bool get_activate() { return activated; }
    bool get_finished() { return finished; }
    BonusBox(const QVector<QPixmap>& bomb_sprites_, float dt_);
    BonusBox(const QVector<QPixmap>& bomb_sprites_, Vec2 dim_,float dt_);
    BonusBox(const QVector<QPixmap>& bomb_sprites_, Vec2 pos_, Vec2 dims_, float dt_);

private:
    vector<Bomb*> bombs;
    int n_bombs = 3;
    QVector<QPixmap> sprites;
    QVector<QPixmap> bomb_sprites;
    float scale;
    float t = 0.0f;
    float dt;
    float bomb_time = 0.0f;
    float total_bomb_time = 5.0f;
    float bomb_explosion_time = 0.2f;
    int b = 0;
    bool visual = true;
    bool first_activated = true;
    bool activated = false;
    bool finished = false;
    bool explosion_started = false;

};


class Projectile: public MovableCircle {
public:
    Projectile(const QVector<QPixmap>& projectile_sprites_) {inv_mass = 1 / mass; sprites = projectile_sprites_;};
    Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float disparition_time_);
    Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float mass_, float disparition_time_);
    Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float mass_, float radius_, float disparition_time_);
    Projectile(const QVector<QPixmap>& projectile_sprites_, Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_, float disparition_time_);

    void explode();
    void set_hit_started(bool hitted_){hit_started = hitted_;}
    bool get_hit_started() { return hit_started; }
    void set_hit_finished(bool hitted_){hit_finished = hitted_;}
    bool get_hit_finished() { return hit_finished; }
    // void set_first_hit(bool hitted_){first_hit = hitted_;}
    // bool get_first_hit() { return first_hit; }
    float get_damage(){ return damage; }
    void set_creator(int creator_) {creator = creator_;}
    int get_creator() {return creator; }
    
    void draw(QPainter& painter) override;
    void rescale_sprites(const QVector<QPixmap>& projectile_sprites_);
    void update(float dt, float &projectil_time, float projectile_min_time);
    void set_disparition_time(float t){ disparition_time = t; }
    float get_min_lifetime() { return min_lifetime; }
    void set_min_lifetime(float t){ min_lifetime = t; }


private:
    QVector<QPixmap> sprites;
    QPixmap *current_sprite = nullptr;
    float damage {20.0};
    bool hit_started = false;
    bool hit_finished = false;
    int creator;
    float total_disparition_time = 0.00;
    float disparition_time = 0.0;
    float min_lifetime = 0.5;
};

#endif