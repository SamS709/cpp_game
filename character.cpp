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
    move_frames.clear();
    jump_frames.clear();
    
    // Load run animation frames (run_0.png to run_7.png)
    for (int i = 0; i <= 7; ++i) {
        QString framePath = basePath + QString("/run/run_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            move_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }

    // Load jump animation frames (jump_0.png to jump_11.png)
    for (int i = 0; i <= 11; ++i) {
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
        frame_per_sprite_jump = total_jump_time * 1000 / (jump_frames.size() * time_between_frames);
    }
    
    // Use first move frame as idle frame
    if (!move_frames.isEmpty()) {
        idleFrame = move_frames[0];
        qDebug() << "Loaded" << move_frames.size() << "sprite frames";
    }
}

void Character::update()
{
    // Move character based on input
    if (jumping) {
        jump_time += time_between_frames/1000;
        if(jump_time > total_jump_time) {
            jumping = false;
            frameCounter = 0;
            current_move_frame = 0;
        }
        else if (right) {
            x += 1;
            facingRight = true;
            // Advance animation frame
            if (!jump_frames.isEmpty()) {
                frameCounter++;
                if (frameCounter >= frame_per_sprite_jump) {
                    frameCounter = 0;
                    current_move_frame = (current_move_frame + 1) % jump_frames.size();
                }
            }
        } else if (left) {
            x -= 1;
            facingRight = false;
            // Advance animation frame
            if (!jump_frames.isEmpty()) {
                frameCounter++;
                if (frameCounter >= frame_per_sprite_jump) {
                    frameCounter = 0;
                    current_move_frame = (current_move_frame + 1) % jump_frames.size();
                }
            }
        }
    }
    if(moving){
        if (right) {
            x += 3;
            facingRight = true;
            
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
            facingRight = false;
            
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
    
}

void Character::draw(QPainter &painter)
{
    const QPixmap *currentSprite = nullptr;
    qDebug() << "Moving = " << moving;
    qDebug() << "Jumping = " << jumping;
    
    // Select appropriate sprite based on movement
    if ((moving) && !move_frames.isEmpty()) {
            currentSprite = &move_frames[current_move_frame];
    } else if (jumping && !jump_frames.isEmpty()) {
            currentSprite = &jump_frames[current_move_frame];
    } else if (!idleFrame.isNull()) {
        currentSprite = &idleFrame;
    }
    
    // Draw the sprite
    if (currentSprite && !currentSprite->isNull()) {
        painter.save();

        // Add the y variations wile jumping
        double delta_y = 0.0;
        if(jumping) {
            delta_y = get_y_jump(jump_time);
        }
        
        // Calculate position (center the sprite)
        double drawX = x - currentSprite->width() / 2.0;
        double drawY = y + delta_y - currentSprite->height() / 2.0;
        
        // Flip horizontally if facing left
        if (!facingRight) {
            painter.translate(x, y);
            painter.scale(-1, 1);
            painter.translate(-x, -y);
        }
        
        painter.drawPixmap(drawX, drawY, *currentSprite);
        painter.restore();
    }
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
}

void Character::set_left(bool l)
{
    left = l;
}

void Character::set_jumping(bool jumping_)
{
    jumping = jumping_;
    jump_time = 0.0;
}

bool Character::get_jumping()
{
    return jumping;
}


void Character::checkBounds(int windowWidth)
{
    if (x > windowWidth + 50) {
        x = -50;
    } else if (x < -50) {
        x = windowWidth + 50;
    }
}
