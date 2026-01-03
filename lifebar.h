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
    BonusBox();
    BonusBox(Vec2 dim_);
    BonusBox(Vec2 pos_, Vec2 dims_);

private:
    QVector<QPixmap> sprites;
    float scale;
    int b = 0;

};



#endif