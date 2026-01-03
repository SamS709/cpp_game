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


BonusBox::BonusBox(){
    load_images();
}

BonusBox::BonusBox(Vec2 dims_)
    :Rectangle(Vec2(dims_.y, dims_.y))
    {
        load_images();
    }
BonusBox::BonusBox(Vec2 pos_, Vec2 dims_)
    :Rectangle(pos_, Vec2(dims_.y, dims_.y))
    {
        load_images();
        
    }

void BonusBox::reload_image(){
    load_images();
}

void BonusBox::load_images(){
    set_color(0,0,0,0);
    sprites.clear();
    for (int i = 1; i <=1; ++i) {
        QString framePath = "resources/images/assets/bomb.png";
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(dims.y, Qt::SmoothTransformation);
            sprites.append(sprite);
            // qDebug() << "Loaded:" << framePath;
        } else {
            // qDebug() << "Failed to load:" << framePath;
        }
    }
}

void BonusBox::draw(QPainter &painter) {
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(get_color());
    painter.drawRect(get_x() , get_y(), get_w(), get_h());
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(get_x(), get_y(), get_w(), get_h());
    painter.drawPixmap(get_x() + (-sprites[b].width() + get_w()) / 2.0 , get_y() + (-sprites[b].height() + get_h()) / 2.0 , sprites[b]);
    
       
}