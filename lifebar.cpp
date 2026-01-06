#include "lifebar.h"
#include "utils.h"
#include <algorithm>
#include <QDebug>

Lifebar::Lifebar(){
    SpriteLoader loader(get_h() * im_scale, false);
    vector<float> asset_dim, character_dim;
    loader.loadSprite("resources/images/assets/hp.png", sprite, asset_dim, character_dim);
}

Lifebar::Lifebar(Vec2 dim_)
    :Rectangle(dim_)
    {
        SpriteLoader loader(get_h() * im_scale, false);
        vector<float> asset_dim, character_dim;
        loader.loadSprite("resources/images/assets/hp.png", sprite, asset_dim, character_dim);
    }
Lifebar::Lifebar(Vec2 pos_, Vec2 dims_)
    :Rectangle(pos_, dims_)
    {
        SpriteLoader loader(get_h() * im_scale, false);
        vector<float> asset_dim, character_dim;
        loader.loadSprite("resources/images/assets/hp.png", sprite, asset_dim, character_dim);
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


BonusBox::BonusBox(QVector<QPixmap> bomb_sprites_, float dt_)
    :dt(dt_)
    , bomb_sprites(bomb_sprites_)
    {
        sprites.append(bomb_sprites_[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
    }

BonusBox::BonusBox(QVector<QPixmap> bomb_sprites_, Vec2 dims_, float dt_)
    :Rectangle(Vec2(dims_.y, dims_.y))
    ,dt(dt_)
    , bomb_sprites(bomb_sprites_)
    {
        sprites.append(bomb_sprites_[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
    }
BonusBox::BonusBox(QVector<QPixmap> bomb_sprites_, Vec2 pos_, Vec2 dims_, float dt_)
    :Rectangle(pos_, Vec2(dims_.y, dims_.y))
    ,dt(dt_)
    , bomb_sprites(bomb_sprites_)
    {
        sprites.append(bomb_sprites_[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
    }

void BonusBox::activate(Vec2 v_char, std::vector<MovableCircle*>& particles, int j){
    activated = true;
    visual = false;
    if(b == 0 && first_activated){
        for (int i = 0; i < n_bombs; i++) {
            Bomb* bomb = new Bomb(
                                bomb_sprites,
                                get_pos() + i * 10.0,
                                v_char * 3.0,
                                5.0,
                                5.0,
                                5.0,
                                bomb_explosion_time
                                );
            bomb->set_creator(j);
            bomb->update_expected_pos(dt);
            bombs.push_back(bomb);
            particles.push_back(bomb);
        }
    }
    first_activated = false;

}




void BonusBox::update(std::vector<MovableCircle*>& particles){
    t+=dt;
    if(activated && b==0){
        for (Bomb* bomb : bombs) {
            bomb->update(dt); 
            if(bomb->get_explosion_finished()){
                auto it = std::find(particles.begin(), particles.end(), bomb);
                if (it != particles.end()) {
                    particles.erase(it);
                    delete bomb;
                }
            }
                
        }
        if (bomb_time>total_bomb_time-bomb_explosion_time && !explosion_started){
            for (Bomb* bomb : bombs) {
                bomb->set_explosion_started(true);
            }
            explosion_started = true;
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
       
}


Bomb::Bomb(QVector<QPixmap> bomb_sprites_, float damage_, float explosion_time_)
    : MovableCircle()
    , sprites(bomb_sprites_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(QVector<QPixmap> bomb_sprites_, float damage_, float mass_, float explosion_time_)
    : MovableCircle(mass_)
    , sprites(bomb_sprites_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(QVector<QPixmap> bomb_sprites_, float damage_, float mass_, float radius_, float explosion_time_)
    : MovableCircle(mass_, radius_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(QVector<QPixmap> bomb_sprites_, Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_, float explosion_time_)
    : MovableCircle(pos_, v_, mass_, radius_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }

void Bomb::rescale_sprites(QVector<QPixmap> bomb_sprites_){
    for (const QPixmap& sprite : bomb_sprites_) {
        sprites.append(sprite.scaledToHeight(radius * 8.0, Qt::SmoothTransformation));
    }
    if (!sprites.isEmpty()) {
        current_sprite = &sprites[0];
    }
}

void Bomb::draw(QPainter& painter) {
    if (current_sprite && !current_sprite->isNull()) {
        painter.save();
        
        
        painter.translate(get_x(), get_y());
        if(!explosion_started){
            painter.rotate(w); 
        }
        
        painter.drawPixmap(-current_sprite->width() / 2.0, 
                          -current_sprite->height() / 2.0, 
                          *current_sprite);
        
        painter.restore();
    }
}

void Bomb::update(float dt){
    if (explosion_started){
        explosion_time += dt;
        if (!sprites.isEmpty()) {
            float p = explosion_time / total_explosion_time;
            int target_frame = (int)(p * sprites.size());
            current_sprite = &sprites[std::min(target_frame, (int)sprites.size() - 1)];
            if(p==1.0f){
                explosion_finished = true;
            }
        }
    }
    

}


void Bomb::explode(){
    explosion_started = true;
}