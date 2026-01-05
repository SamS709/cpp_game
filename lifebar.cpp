#include "lifebar.h"
#include <algorithm>
#include <QDebug>

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


BonusBox::BonusBox(float dt_)
    :dt(dt_)
    {
        load_images();
    }

BonusBox::BonusBox(Vec2 dims_, float dt_)
    :Rectangle(Vec2(dims_.y, dims_.y))
    ,dt(dt_)
    {
        load_images();
    }
BonusBox::BonusBox(Vec2 pos_, Vec2 dims_, float dt_)
    :Rectangle(pos_, Vec2(dims_.y, dims_.y))
    ,dt(dt_)
    {
        load_images();
        
    }

void BonusBox::activate(Vec2 v_char, std::vector<MovableCircle*>& particles, int i){
    activated = true;
    visual = false;
    if(b == 0 && first_activated){
        for (int i = 0; i < n_bombs; i++) {
            Bomb* bomb = new Bomb(
                                get_pos() + i * 10.0,
                                v_char * 3.0,
                                5.0,
                                5.0,
                                5.0
                                );
            bomb->set_creator(i);
            bomb->update_expected_pos(dt);
            bombs.push_back(bomb);
            particles.push_back(bomb);
        }
    }
    first_activated = false;

}

void BonusBox::reload_image(){
    load_images();
}

void BonusBox::load_images(){
    set_color(120,120,120,120);
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

void BonusBox::update(std::vector<MovableCircle*>& particles){
    t+=dt;
    if(activated && b==0){
        for (Bomb* bomb : bombs) {
            if(bomb->get_exploded()){
                auto it = std::find(particles.begin(), particles.end(), bomb);
                if (it != particles.end()) {
                    particles.erase(it);
                    delete bomb;
                }
            }
                
        }   
        if(bomb_time>total_bomb_time){
            // Remove bombs from particles vector
            for (Bomb* bomb : bombs) {
                auto it = std::find(particles.begin(), particles.end(), bomb);
                if (it != particles.end()) {
                    particles.erase(it);
                    delete bomb;
                }
            }
            bombs.clear();
            finished = true;
        }
        bomb_time += dt;
    }
}

void BonusBox::draw(QPainter &painter) {
    if(visual){
        painter.setPen(Qt::NoPen);
        painter.setBrush(get_color());
        painter.drawRect(get_x() , get_y(), get_w(), get_h());
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(get_x(), get_y(), get_w(), get_h());
        painter.drawPixmap(get_x() + (-sprites[b].width() + get_w()) / 2.0 , get_y() + (-sprites[b].height() + get_h()) / 2.0 , sprites[b]);
    }
    // if (activated) {
    //     // Draw bombs
    //     if (b == 0) {
    //         for (MovableCircle* bomb:bombs){
    //             bomb->draw(painter);
    //         }
    //     }
    // }
    
    
       
}


Bomb::Bomb(float damage_)
    : MovableCircle()
    , damage(damage_)
    {

    }
Bomb::Bomb(float damage_, float mass_)
    : MovableCircle(mass_)
    , damage(damage_)
    {

    }
Bomb::Bomb(float damage_, float mass_, float radius_)
    : MovableCircle(mass_, radius_)
    , damage(damage_)
    {

    }
Bomb::Bomb(Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_)
    : MovableCircle(pos_, v_, mass_, radius_)
    , damage(damage_)
    {

    }

void Bomb::explode(){
    exploded = true;
}