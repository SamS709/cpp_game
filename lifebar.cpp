#include "lifebar.h"

Lifebar::Lifebar(){
    sprite = QPixmap("resources/images/assets/hp.png");
    sprite = sprite.scaledToHeight(get_h() * im_scale, Qt::SmoothTransformation);
}

Lifebar::Lifebar(Vec2 dim_)
    :Rectangle(dim_)
    {
        sprite = QPixmap("resources/images/assets/hp.png");
        sprite = sprite.scaledToHeight(get_h() * im_scale, Qt::SmoothTransformation);
    }
Lifebar::Lifebar(Vec2 pos_, Vec2 dims_)
    :Rectangle(pos_, dims_)
    {
        sprite = QPixmap("resources/images/assets/hp.png");
        sprite = sprite.scaledToHeight(get_h() * im_scale, Qt::SmoothTransformation);
        
    }

void Lifebar::reload_image(){
    sprite = QPixmap("resources/images/assets/hp.png");
    sprite = sprite.scaledToHeight(get_h() * im_scale, Qt::SmoothTransformation);
}

void Lifebar::draw(QPainter &painter) {
    float offset {0.0};
    if(get_x()==0.0) {
        offset = sprite.width() / 3.0; 
    }
    else {
        offset = -sprite.width();
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(get_color());
    painter.drawRect(get_x() + 2 * offset + sprite.width() * 1.5, get_y(), get_w() * p, get_h());
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(get_x() + 2 * offset + sprite.width() * 1.5, get_y(), get_w(), get_h());
    painter.drawPixmap(get_x() + 2 * offset, get_y() + (get_h() - sprite.height()) / 2.0, sprite);
    
    
    
}