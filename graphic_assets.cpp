#include "graphic_assets.h"
#include "utils.h"
#include <algorithm>
#include <QDebug>
#include "character.h"

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
        offset = sprite.width() / 4.0; 
    }
    else {
        offset = -sprite.width();
    }
    // qDebug()<<sprite.width();
    painter.setPen(Qt::NoPen);
    painter.setBrush(get_color());
    painter.drawRect(get_x() + 2 * offset + sprite.width() * 1.5, get_y(), get_w() * p, get_h());
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(get_x() + 2 * offset + sprite.width() * 1.5, get_y(), get_w(), get_h());
    painter.drawPixmap(get_x() + 2 * offset, get_y() + (get_h() - sprite.height()) / 2.0, sprite);
        
}


CommandPanel::CommandPanel()
    : Rectangle()
{
    set_color(50, 50, 50, 200);
}

CommandPanel::CommandPanel(Vec2 pos_, Vec2 dims_)
    : Rectangle(pos_, dims_)
{
    set_color(50, 50, 50, 100);
}

void CommandPanel::draw(QPainter &painter) {

    // Key dimensions
    float keySize = 20;
    float keySpacing = 5;
    float centerX = get_x() + get_w() / 2.0;
    float startY = get_y();
    
    // Font setup for keys
    QFont keyFont = painter.font();
    keyFont.setPointSize(10);
    keyFont.setBold(true);
    
    QFont labelFont = painter.font();
    labelFont.setPointSize(5);
    
    // Helper lambda to draw a key
    auto drawKey = [&](float x, float y, const QString& key, const QString& label, bool labelBelow = true) {
        // Draw key background (lighter color)
        painter.setBrush(QColor(80, 80, 80, 50));
        painter.setPen(QPen(QColor(150, 150, 150), 2));
        painter.drawRoundedRect(x, y, keySize, keySize, 3, 3);
        
        // Draw key letter
        painter.setPen(Qt::white);
        painter.setFont(keyFont);
        painter.drawText(QRectF(x, y, keySize, keySize), Qt::AlignCenter, key);
        
        // Draw label 
        painter.setFont(labelFont);
        painter.setPen(QColor(150, 150, 150));
        if (labelBelow) {
            painter.drawText(QRectF(x - 10, y + keySize - 1.0, keySize + 20, 15), Qt::AlignCenter, label);
        } else {
            painter.drawText(QRectF(x - 10, y - 17, keySize + 20, 15), Qt::AlignCenter, label);
        }
    };
    
    if (player == 1) {
        drawKey(centerX - keySize / 2.0, startY, "Z", "Jump", false);
        drawKey(centerX - keySize * 1.5 - keySpacing, startY + keySize + keySpacing, "Q", "Left");
        drawKey(centerX - keySize / 2.0, startY + keySize + keySpacing, "S", "Down");
        drawKey(centerX + keySize / 2.0 + keySpacing, startY + keySize + keySpacing, "D", "Right");
        drawKey(centerX - keySize - keySpacing / 2.0, startY + (keySize + keySpacing) * 2 + 10, "A", "Sword");
        drawKey(centerX + keySpacing / 2.0, startY + (keySize + keySpacing) * 2 + 10, "E", "Projectile");
    } else {
        drawKey(centerX - keySize / 2.0, startY, "↑", "Jump", false);
        drawKey(centerX - keySize * 1.5 - keySpacing, startY + keySize + keySpacing, "←", "Left");
        drawKey(centerX - keySize / 2.0, startY + keySize + keySpacing, "↓", "Down");
        drawKey(centerX + keySize / 2.0 + keySpacing, startY + keySize + keySpacing, "→", "Right");
        drawKey(centerX - keySize - keySpacing / 2.0, startY + (keySize + keySpacing) * 2 + 10, "0", "Sword");
        drawKey(centerX + keySpacing / 2.0, startY + (keySize + keySpacing) * 2 + 10, "1", "Projectile");
    }
}


BonusBox::BonusBox(int b_, const VisualContainer *vc, float dt_)
    :b(b_),
    dt(dt_)
    , bomb_sprites(vc->bomb_sprites)
    {
        sprites.append(bomb_sprites[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->speed_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->hp_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        if(b == 0) {
            set_color(255, 50, 50, 255);
        } else if(b == 1) {
            set_color(50, 50, 255, 255);
        } else if(b == 2) {
            set_color(50, 255, 50, 255);
        }
    }

BonusBox::BonusBox(int b_, const VisualContainer *vc, Vec2 dims_, float dt_)
    :Rectangle(Vec2(dims_.y, dims_.y))
    ,b(b_),
    dt(dt_)
    , bomb_sprites(vc->bomb_sprites)
    {
        sprites.append(bomb_sprites[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->speed_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->hp_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        if(b == 0) {
            set_color(255, 50, 50, 255);
        } else if(b == 1) {
            set_color(50, 50, 255, 255);
        } else if(b == 2) {
            set_color(50, 255, 50, 255);
        }
    }
BonusBox::BonusBox(int b_, const VisualContainer *vc, Vec2 pos_, Vec2 dims_, float dt_)
    :Rectangle(pos_, Vec2(dims_.y, dims_.y))
    ,b(b_),
    dt(dt_)
    , bomb_sprites(vc->bomb_sprites)
    {
        sprites.append(bomb_sprites[0].scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->speed_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        sprites.append(vc->hp_sprite.scaledToHeight(dims.y, Qt::SmoothTransformation));
        if(b == 0) {
            set_color(200, 50, 50, 255);
        } else if(b == 1) {
            set_color(50, 50, 200, 255);
        } else if(b == 2) {
            set_color(100, 200, 100, 255);
        }
    }

void BonusBox::activate(Character& character, std::vector<MovableCircle*>& particles, int j){
    activated = true;
    visual = false;
    c = &character;
    if(b == 0 && first_activated){
        for (int i = 0; i < n_bombs; i++) {
            Bomb* bomb = new Bomb(
                                bomb_sprites,
                                get_pos() + (i-1) * 20.0,
                                {5.0f * character.get_v_x(), character.get_v_y()},
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
    } else if (b == 1 && first_activated) {
        character.set_speed_multiplier(2.0);
    } else if (b == 2 && first_activated) {
        character.set_hp(character.get_hp() + 10);
    }

    first_activated = false;

}




void BonusBox::update(std::vector<MovableCircle*>& particles){
    t+=dt;
    if(activated && b==0){
        // Update all bombs first
        for (Bomb* bomb : bombs) {
            bomb->update(dt); 
        }
        
        // Remove finished bombs 
        for (auto it = bombs.begin(); it != bombs.end(); ) {
            Bomb* bomb = *it;
            if(bomb->get_explosion_finished()){
                auto particle_it = std::find(particles.begin(), particles.end(), bomb);
                if (particle_it != particles.end()) {
                    particles.erase(particle_it);
                }
                delete bomb;
                it = bombs.erase(it); 
            } else {
                ++it;
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
    else if (activated && b == 1) {
        if(t > speed_time) {
            c->set_speed_multiplier(1.0);
            finished = true;
        }
    } else if (activated && b == 2) {
        finished = true;
    }
}

void BonusBox::draw(QPainter &painter) {
    if(visual){
        painter.setPen(Qt::NoPen);
        painter.setBrush(get_color());
        painter.drawRoundedRect(get_x() , get_y(), get_w(), get_h(), 3, 3);
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(get_x() , get_y(), get_w(), get_h(), 3, 3);
        painter.drawPixmap(get_x() + (-sprites[b].width() + get_w()) / 2.0 , get_y() + (-sprites[b].height() + get_h()) / 2.0 , sprites[b]);
    }
       
}


Bomb::Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float explosion_time_)
    : MovableCircle()
    , sprites(bomb_sprites_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float mass_, float explosion_time_)
    : MovableCircle(mass_)
    , sprites(bomb_sprites_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(const QVector<QPixmap>& bomb_sprites_, float damage_, float mass_, float radius_, float explosion_time_)
    : MovableCircle(mass_, radius_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }
Bomb::Bomb(const QVector<QPixmap>& bomb_sprites_, Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_, float explosion_time_)
    : MovableCircle(pos_, v_, mass_, radius_)
    , damage(damage_)
    , total_explosion_time(explosion_time_)
    {
        rescale_sprites(bomb_sprites_);
    }

void Bomb::rescale_sprites(const QVector<QPixmap>& bomb_sprites_){
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
    if (explosion_started && !explosion_finished){
        set_v(Vec2(0.0F, 0.0f), dt);
        explosion_time += dt;
        if (!sprites.isEmpty()) {
            float p = explosion_time / total_explosion_time;
            int target_frame = (int)(p * sprites.size());
            current_sprite = &sprites[std::min(target_frame, (int)sprites.size() - 1)];
            if(explosion_time>=total_explosion_time){
                explosion_finished = true;
                set_y(0.0);
            }
        }
    }
    

}


void Bomb::explode(){
    explosion_started = true;
}




Projectile::Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float disparition_time_)
    : MovableCircle()
    , sprites(projectile_sprites_)
    , damage(damage_)
    , total_disparition_time(disparition_time_)
    {
        rescale_sprites(projectile_sprites_);
    }
Projectile::Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float mass_, float disparition_time_)
    : MovableCircle(mass_)
    , sprites(projectile_sprites_)
    , damage(damage_)
    , total_disparition_time(disparition_time_)
    {
        rescale_sprites(projectile_sprites_);
    }
Projectile::Projectile(const QVector<QPixmap>& projectile_sprites_, float damage_, float mass_, float radius_, float disparition_time_)
    : MovableCircle(mass_, radius_)
    , damage(damage_)
    , total_disparition_time(disparition_time_)
    {
        rescale_sprites(projectile_sprites_);
    }
Projectile::Projectile(const QVector<QPixmap>& projectile_sprites_, Vec2 pos_, Vec2 v_, float damage_, float mass_, float radius_, float disparition_time_)
    : MovableCircle(pos_, v_, mass_, radius_)
    , damage(damage_)
    , total_disparition_time(disparition_time_)
    {
        rescale_sprites(projectile_sprites_);
    }

void Projectile::rescale_sprites(const QVector<QPixmap>& projectile_sprites_){
    for (const QPixmap& sprite : projectile_sprites_) {
        sprites.append(sprite.scaledToHeight(radius, Qt::SmoothTransformation));
    }
    if (!sprites.isEmpty()) {
        current_sprite = &sprites[0];
    }
}

void Projectile::draw(QPainter& painter) {
    if (current_sprite && !current_sprite->isNull() && !hit_finished) {
        painter.save();
        
        
        painter.translate(get_x(), get_y());
        if(!hit_started){
            painter.rotate(w); 
        }
        
        painter.drawPixmap(-current_sprite->width() / 2.0, 
                          -current_sprite->height() / 2.0, 
                          *current_sprite);
        
        painter.restore();
    }
}

void Projectile::update(float dt, float& projectile_time, float projectile_min_time){
    
    if (hit_started){
        disparition_time += dt;
        if (!sprites.isEmpty()) {
            qDebug()<<disparition_time;
            float p = disparition_time / total_disparition_time;
            int target_frame = (int)(p * sprites.size());
            current_sprite = &sprites[std::min(target_frame, (int)sprites.size() - 1)];
            if(disparition_time>=total_disparition_time){
                disparition_time = 0.0f;
                projectile_time = projectile_min_time + 0.1;
                hit_finished = true;
                hit_started = false;
                current_sprite = &sprites[0];
                set_pos({10.0, 10.0});
            }
        }
    } else{
        update_expected_pos(dt);
        update_vel(dt);
        update_pos();
    }
    

}


void Projectile::explode(){
}