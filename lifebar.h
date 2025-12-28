#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "QPainter"
#include "QPixmap"
#include "assets.h"

class Lifebar: public Rectangle {

public:

    void set_percentage(double p_) {p = p_;}
    void draw(QPainter &painter);
    void reload_image();
    Lifebar();
    Lifebar(Vec2 dim_);
    Lifebar(Vec2 pos_, Vec2 dims_);

private:
    QPixmap sprite;
    double im_scale = 1.5;
    double p = 1.0;

};



#endif