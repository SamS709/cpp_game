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

class BonusBox: public Rectangle {

public:

    void set_bonus(int b) {b = b;}
    int get_bonus() const { return b; }
    void draw(QPainter &painter) ;
    void load_images();
    void reload_image();
    void update(float dt, std::vector<MovableCircle*>& particles);
    void activate(float v_char, std::vector<MovableCircle*>& particles);
    bool get_activate() { return activated; }
    bool get_finished() { return finished; }
    BonusBox();
    BonusBox(Vec2 dim_);
    BonusBox(Vec2 pos_, Vec2 dims_);

private:
    vector<MovableCircle*> bombs;
    int n_bombs = 10;
    QVector<QPixmap> sprites;
    float scale;
    float t = 0.0f;
    float bomb_time = 0.0f;
    float total_bomb_time = 5.0f;
    int b = 0;
    bool visual = true;
    bool first_activated = true;
    bool activated = false;
    bool finished = false;

};



#endif