#include "character.h"
#include <QDebug>

Character::Character(double startX, double startY, double time_betwwen_frames_)
    : x(startX)
    , y(startY)
    , time_between_frames(time_betwwen_frames_)
    , right(false)
    , left(false)
    , moving(false)
    , jumping(false)
    , facingRight(true)
    , current_move_frame(0)
    , current_jump_frame(0)
    , frameCounter(0)
    , framesPerSprite(3)  // Change sprite every 5 updates (~12 FPS at 60 FPS)
    , jump_height(100)
{
    
}

void Character::loadSpriteFrames(const QString &basePath)
{
    load_move_frames(basePath);
    load_jump_frames(basePath); 
    load_slide_frames(basePath);
    load_lower_frames(basePath);
    load_sword_attack_frames(basePath);
    
}

void Character::load_move_frames(const QString &basePath){
    move_frames.clear();
    for (int i = 0; i <= 4; ++i) {
        QString framePath = basePath + QString("/run/run_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            move_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }
    // Use first move frame as idle frame
    if (!move_frames.isEmpty()) {
        idleFrame = move_frames[0];
        qDebug() << "Loaded" << move_frames.size() << "sprite frames";
    }
}

void Character::load_jump_frames(const QString &basePath){

    jump_frames.clear();
    for (int i = 0; i <= 4; ++i) {
        QString framePath = basePath + QString("/jump/jump_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            jump_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }
    if (!jump_frames.isEmpty()) {
        frame_per_sprite_jump = total_jump_time * 1000 / (jump_frames.size() * time_between_frames) * 2;
    }
}

void Character::load_slide_frames(const QString &basePath){

    slide_frames.clear();
    for (int i = 0; i <= 4; ++i) {
        QString framePath = basePath + QString("/slide/slide_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            slide_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    
    }
    if (!slide_frames.isEmpty()) {
        frame_per_sprite_slide = total_slide_time * 1000 / (slide_frames.size() * time_between_frames) * 2;
    }
}

void Character::load_lower_frames(const QString &basePath){
    lower_frames.clear();
    for (int i = 0; i <= 3; ++i) {
        QString framePath = basePath + QString("/lower/lower_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            lower_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }

}

void Character::load_sword_attack_frames(const QString &basePath){
    sword_attack_frames.clear();
    for (int i = 0; i <= 3; ++i) {
        QString framePath = basePath + QString("/sword_attack/sword_attack_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            lower_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }

}

void Character::update()
{
    // Move character based on input
    if (jumping) {
        update_jump();
    }
    slide_time += time_between_frames/1000; // slide_time also counts when not sliding
    if (sliding) {
        update_slide();
    }
    if (lowering){
        update_lower();
    }
    if (sword_attacking){
        update_sword_attack();
    }
    if(moving){
        update_move();
    } 
    
}

void Character::update_jump(){
    jump_time += time_between_frames/1000;
    if(jump_time > total_jump_time) {
        jumping = false;
        frameCounter = 0;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;
        }
    } else {
        // Advance jump animation frame (regardless of direction)
        if (!jump_frames.isEmpty()) {
            frameCounter++;
            if (frameCounter >= frame_per_sprite_jump) {
                frameCounter = 0;
                current_move_frame = (current_move_frame + 1) % jump_frames.size();
            }
        }
        
        // Move horizontally during jump
        if (right) {
            x += 1;
        } else if (left) {
            x -= 1;
        }
    }
}

void Character::update_lower(){
    if (!lower_frames.isEmpty()) {
        frameCounter++;
        if (frameCounter >= framesPerSprite) {
            frameCounter = 0;
            current_move_frame = 1;
        }
    }
}


void Character::update_slide(){
    if(slide_time > total_slide_time) {
        sliding = false;
        slide_time = 0.0;
        frameCounter = 0;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;
        }
    } else {
        // Advance jump animation frame (regardless of direction)
        if (!slide_frames.isEmpty()) {
            frameCounter++;
            if (frameCounter >= frame_per_sprite_slide) {
                frameCounter = 0;
                current_move_frame = (current_move_frame + 1) % jump_frames.size();
            }
        }
        
        // No horizontal movement during slide, just maintain direction
    }
}

void Character::update_sword_attack(){
    // if(slide_time > total_slide_time) {
    //     sliding = false;
    //     slide_time = 0.0;
    //     frameCounter = 0;
    //     current_move_frame = 0;
    //     // Resume moving if a direction key is still pressed
    //     if (right || left) {
    //         moving = true;
    //     }
    // } else {
    //     // Advance jump animation frame (regardless of direction)
    //     if (!slide_frames.isEmpty()) {
    //         frameCounter++;
    //         if (frameCounter >= frame_per_sprite_slide) {
    //             frameCounter = 0;
    //             current_move_frame = (current_move_frame + 1) % jump_frames.size();
    //         }
    //     }
        
    //     // No horizontal movement during slide, just maintain direction
    // }
}

void Character::update_move(){
    if (right) {
        x += 3;
        
        // Advance animation frame
        if (!move_frames.isEmpty()) {
            frameCounter++;
            if (frameCounter >= framesPerSprite) {
                frameCounter = 0;
                current_move_frame = (current_move_frame + 1) % move_frames.size();
            }
        }
    } else if (left) {
        x -= 3;
        
        // Advance animation frame
        if (!move_frames.isEmpty()) {
            frameCounter++;
            if (frameCounter >= framesPerSprite) {
                frameCounter = 0;
                current_move_frame = (current_move_frame + 1) % move_frames.size();
            }
        }
    } else {
        // Reset to first frame when not moving
        current_move_frame = 0;
        frameCounter = 0;
    }
}

void Character::draw(QPainter &painter)
{
    const QPixmap *currentSprite = nullptr;
    // qDebug() << "Moving = " << moving;
    // qDebug() << "Jumping = " << jumping;
    
    // Select appropriate sprite based on movement
    double delta_y = 0.0;
    double delta_x = 0.0;

    if (moving && !move_frames.isEmpty()) {
        currentSprite = &move_frames[current_move_frame];
    } else if (jumping && !jump_frames.isEmpty()) {
        // Add y variations when jumping
        delta_y = get_y_jump(jump_time);
        currentSprite = &jump_frames[current_move_frame];
    } else if (sliding && !slide_frames.isEmpty()) {
        // Add x variations when sliding
        delta_x = get_x_sliding(slide_time);
        currentSprite = &slide_frames[current_move_frame];
    } else if (lowering && !lower_frames.isEmpty()) {
        currentSprite = &lower_frames[current_move_frame];
    } else if (!idleFrame.isNull()) {
        currentSprite = &idleFrame;
    }
    
    // Draw the sprite
    if (currentSprite && !currentSprite->isNull()) {
        painter.save();

        if (slide_dir == "right") {
            x += delta_x;
        } else {
            x-= delta_x;
        }
        
        // Calculate position (center the sprite)
        double drawX = x - currentSprite->width() / 2.0;
        double drawY = y + delta_y - currentSprite->height() / 2.0;
        
        // Flip horizontally if facing left
        handle_rotate(painter);
        painter.drawPixmap(drawX, drawY, *currentSprite);
        painter.restore();
    }   
}

void Character::handle_rotate(QPainter &painter){
    if(sliding){
        if (slide_dir=="left"){
            painter.translate(x, y);
            painter.scale(-1, 1);
            painter.translate(-x, -y);
        }
    } else {
        if (!facingRight) {
            painter.translate(x, y);
            painter.scale(-1, 1);
            painter.translate(-x, -y);
        }
    }
}

double Character::get_x_sliding(double t) {
    return slide_dist * (total_slide_time - t) / (total_slide_time );
}

double Character::get_y_jump(double t) {
    return - 4 * jump_height * t / total_jump_time * (1 - t / total_jump_time);
}

void Character::set_moving(bool m)
{
    moving = m;
}

void Character::set_right(bool r)
{
    right = r;
    if (r) facingRight = true;
}

void Character::set_left(bool l)
{
    left = l;
    if (l) facingRight = false;
}

void Character::set_jumping(bool jumping_)
{
    jumping = jumping_;
    jump_time = 0.0;
    sliding = false;
}

void Character::set_sliding(bool s)
{
    if (slide_time > time_between_slides) {
        moving = false;
        qDebug() << "set true";
        qDebug() << "Sliding = " << sliding;
        qDebug() << "slide_time" << slide_time << ">" << time_between_slides;
        sliding = s;
        slide_time = 0.0;
        if (right) {
            slide_dir = "right";
        } else if (left) {
            slide_dir = "left";
        }
    }
}

void Character::set_lowering(bool s)
{
    lowering = s;
}

bool Character::get_moving()
{
    return moving;
}

bool Character::get_jumping()
{
    return jumping;
}

bool Character::get_sliding()
{
    return sliding;
}


void Character::checkBounds(int windowWidth)
{
    if (x > windowWidth + 50) {
        x = -50;
    } else if (x < -50) {
        x = windowWidth + 50;
    }
}
