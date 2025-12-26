#include "character.h"

Character::Character(Vec2 pos_, double time_betwwen_frames_, double mass_)
    : MovableAsset(mass)
    , time_between_frames(time_betwwen_frames_)
    , right(false)
    , left(false)
    , moving(false)
    , jumping(false)
    , facingRight(true)
    , current_move_frame(0)
    , current_jump_frame(0)
    , frameCounter(0)
    , framesPerSprite(100)  // Change sprite every 5 updates (~12 FPS at 60 FPS)
    , jump_height(500)
{
    loadSpriteFrames("resources/images/characters/redhat");
    current_dims = move_frames_dims[0];
    set_x(pos_.x);
    set_y(pos_.y);
    
    
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
    move_frames_dims.clear();
    for (int i = 1; i <=39; ++i) {
        QString framePath = basePath + QString("/walk/walk_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            vector<double> frameDims;
            set_hitbox(sprite, frameDims);
            move_frames.append(sprite);
            move_frames_dims.push_back(frameDims);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }
    // Use first move frame as idle frame
    if (!move_frames.isEmpty()) {
        idleFrame = move_frames[5];
        idle_dims = move_frames_dims[5];
        qDebug() << "Loaded" << move_frames.size() << "sprite frames";
    }
}

void Character::load_jump_frames(const QString &basePath){

    jump_frames.clear();
    for (int i = 0; i <= 63; ++i) {
        QString framePath = basePath + QString("/jump/jump_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            vector<double> frameDims;
            set_hitbox(sprite, frameDims);
            jump_frames.append(sprite);
            jump_frames_dims.push_back(frameDims);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }

}

void Character::load_slide_frames(const QString &basePath){

    slide_frames.clear();
    for (int i = 0; i <= 4; ++i) {
        QString framePath = basePath + QString("/slide/slide_%1.png").arg(i);
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            
            slide_frames.append(sprite);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    
    }

}

void Character::load_lower_frames(const QString &basePath){
    lower_frames.clear();
    for (int i = 0; i <= 10; ++i) {
        QString framePath = basePath + QString("/lower/lower_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            vector<double> frameDims;
            set_hitbox(sprite, frameDims);
            lower_frames.append(sprite);
            lower_frames_dims.push_back(frameDims);
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }

}

void Character::load_sword_attack_frames(const QString &basePath){
    sword_attack_frames.clear();
    for (int i = 1; i <= 121; ++i) {
        QString framePath = basePath + QString("/sword_attack/sword_attack_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            sword_attack_frames.append(sprite);
            // qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }
}

void Character::set_hitbox(QPixmap& sprite, vector<double>& dims_){
    // Convert QPixmap to QImage to access pixels
    QImage image = sprite.toImage();
    
    if (image.isNull()) {
        return;
    }
    
    int width = image.width();
    int height = image.height();
    
    // Find left bound (first non-transparent pixel from left)
    int leftBound = width;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QRgb pixel = image.pixel(x, y);
            // Check if pixel is visible (not transparent)
            if (qAlpha(pixel) > 10) {
                leftBound = x;
                goto foundLeft;
            }
        }
    }
    foundLeft:
    
    // Find right bound (first non-transparent pixel from right)
    int rightBound = 0;
    for (int x = width - 1; x >= 0; x--) {
        for (int y = 0; y < height; y++) {
            QRgb pixel = image.pixel(x, y);
            if (qAlpha(pixel) > 10) {
                rightBound = x;
                goto foundRight;
            }
        }
    }
    foundRight:
    
    // Find top bound (first non-transparent pixel from top)
    int topBound = height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            if (qAlpha(pixel) > 10) {
                topBound = y;
                goto foundTop;
            }
        }
    }
    foundTop:
    
    // Find bottom bound (first non-transparent pixel from bottom)
    int bottomBound = 0;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            if (qAlpha(pixel) > 10) {
                bottomBound = y;
                goto foundBottom;
            }
        }
    }
    foundBottom:
    
    // Calculate character dimensions
    double charWidth = rightBound - leftBound;
    double charHeight = bottomBound - topBound;
    
    // Fill dims_ with computed dimensions [leftBound, topBound, width/2, height/2]
    dims_.clear();
    dims_.push_back(leftBound);
    dims_.push_back(topBound);
    dims_.push_back(charWidth / 2.0);
    dims_.push_back(charHeight / 2.0);
    
    // Draw bounding box on sprite if draw_baxes is true
    if (draw_baxes) {
        QPainter painter(&sprite);
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(leftBound, topBound, charWidth, charHeight);
    }
    
    qDebug() << "Hitbox bounds - Left:" << leftBound << "Right:" << rightBound 
             << "Top:" << topBound << "Bottom:" << bottomBound 
             << "Width:" << charWidth << "Height:" << charHeight;
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
    sword_attack_time += time_between_frames/1000; // sword_attack_time also counts when not attacking
    if (sword_attacking){
        update_sword_attack();
    }
    if (lowering){
        update_lower();
    }
    walk_step_time += time_between_frames/1000; // walk_step_time always counts
    if(moving){
        update_move();
    } 
    
}

void Character::update_jump(){
    jump_time += time_between_frames/1000;
    
    // Update animation frame based on velocity - smooth interpolation
    if (!jump_frames.isEmpty()) {
        double v_y = get_v_y();
        int num_frames = jump_frames.size();
        
        // Map velocity to frame index
        // v_y ranges from -jump_velocity (rising) to +jump_velocity (falling)
        // Map to frame 0 (start of jump) -> num_frames/2 (peak) -> num_frames-1 (landing)
        
        double velocity_range = speed_jump * 2.0;  // Total velocity range
        double normalized = (v_y + speed_jump) / velocity_range;  // 0.0 to 1.0
        normalized = std::max(0.0, std::min(1.0, normalized));  // Clamp
        
        // Map to frame index
        current_move_frame = (int)(normalized * (num_frames - 1));
        current_move_frame = std::max(0, std::min(current_move_frame, num_frames - 1));
    }
}


void Character::update_lower(){
    if (!lower_frames.isEmpty()) {
        current_move_frame = 9;
        }
}


void Character::update_slide(){
    if(slide_time > total_slide_time) {
        sliding = false;
        slide_time = 0.0;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;
        }
    } else {
        // Calculate which frame to show based on elapsed time
        if (!slide_frames.isEmpty()) {
            int target_frame = (int)((slide_time / total_slide_time) * slide_frames.size());
            current_move_frame = std::min(target_frame, (int)slide_frames.size() - 1);
        }
        
        // No horizontal movement during slide, just maintain direction
    }
}

void Character::update_sword_attack(){
    if(sword_attack_time > total_sword_attack_time) {
        sword_attacking = false;
        sword_attack_time = 0.0;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;
        }
    } else {
        // Calculate which frame to show based on elapsed time
        if (!sword_attack_frames.isEmpty()) {
            int target_frame = (int)((sword_attack_time / total_sword_attack_time) * sword_attack_frames.size());
            current_move_frame = std::min(target_frame, (int)sword_attack_frames.size() - 1);
            qDebug()<<current_move_frame;
        }
        
        // No horizontal movement during slide, just maintain direction
    }
}

void Character::update_move(){
    if (right) {
        // x += 0.5;
        
        if (!move_frames.isEmpty()) {
            double cycle_time = fmod(walk_step_time, total_walk_step_time);
            int target_frame = (int)((cycle_time / total_walk_step_time) * move_frames.size());
            current_move_frame = std::min(target_frame, (int)move_frames.size() - 1);
        }
    } else if (left) {
        // x -= 0.5;
        
        if (!move_frames.isEmpty()) {
            double cycle_time = fmod(walk_step_time, total_walk_step_time);
            int target_frame = (int)((cycle_time / total_walk_step_time) * move_frames.size());
            current_move_frame = std::min(target_frame, (int)move_frames.size() - 1);
        }
    } else {
        current_move_frame = 0;
        walk_step_time = 0.0;
    }
}

void Character::draw(QPainter &painter)
{
    
    double delta_x = 0.0;

    if (moving && !move_frames.isEmpty()) {
        currentSprite = &move_frames[current_move_frame];
        current_dims = move_frames_dims[current_move_frame];
    } else if (jumping && !jump_frames.isEmpty()) {
        currentSprite = &jump_frames[current_move_frame];
        current_dims = jump_frames_dims[current_move_frame];
    } else if (sliding && !slide_frames.isEmpty()) {
        delta_x = get_x_sliding(slide_time);
        currentSprite = &slide_frames[current_move_frame];
        if (slide_dir == "right") {
            x += delta_x;
        } else {
            x-= delta_x;
        }
    } else if (lowering && !lower_frames.isEmpty()) {
        currentSprite = &lower_frames[current_move_frame];
        current_dims = lower_frames_dims[current_move_frame];
    } else if (sword_attacking && !sword_attack_frames.isEmpty()) {
        delta_x = get_x_sword_attacking(sword_attack_time);
        currentSprite = &sword_attack_frames[current_move_frame];
        if (sword_attack_dir == "right") {
            x += delta_x;
        } else {
            x-= delta_x;
        }
    } else if (!(idleFrame.isNull() | idle_dims.empty())) {
        current_dims = idle_dims;
        currentSprite = &idleFrame;
    }
    
    // Update dims based on current sprite
    if (currentSprite && !currentSprite->isNull()) {
        set_w(2*current_dims[2]);
        set_h(2*current_dims[3]);
    }
    
    // Draw the sprite
    if (currentSprite && !currentSprite->isNull()) {
        painter.save();

        // Calculate position for drawing (offset from the character dims)
        double drawX = get_x() - current_dims[0]-current_dims[2];
        double drawY = get_y() - current_dims[1]-current_dims[3];
        
        // Flip horizontally if facing left
        handle_rotate(painter);
        painter.drawPixmap(drawX, drawY, *currentSprite);
        painter.restore();
    }   
}

void Character::handle_rotate(QPainter &painter){
    if(sliding || sword_attacking){
        if (slide_dir=="left" || sword_attack_dir == "left"){
            painter.translate(get_x(), get_y());
            painter.scale(-1, 1);
            painter.translate(-get_x(), -get_y());
        }
    } else {
        if (!facingRight) {
            painter.translate(get_x(), get_y());
            painter.scale(-1, 1);
            painter.translate(-get_x(), -get_y());
        }
    }
}

double Character::get_x_sliding(double t) {
    return slide_dist * (total_slide_time - t) / (total_slide_time );
}

double Character::get_x_sword_attacking(double t) {
    return sword_attack_dist * (total_sword_attack_time - t) / (total_sword_attack_time );
}

void Character::set_moving(bool m)
{
    moving = m;
    if (m) {
        if (right) {
            set_v_x(speed_move);
        }
        else {
            set_v_x(-speed_move);
        }
    }
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
    current_move_frame = 0;
    sliding = false;
    if (jumping_) {
        set_v_y(-speed_jump);
    }
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
        current_move_frame = 0;
        if (right) {
            slide_dir = "right";
        } else if (left) {
            slide_dir = "left";
        } else {
            slide_dir = facingRight ? "right" : "left";
        }
    }
}

void Character::set_lowering(bool s)
{
    lowering = s;
}

void Character::set_sword_attacking(bool a)
{
    if (sword_attack_time > time_between_sword_attacks) {
        moving = false;
        sword_attacking = a;
        sword_attack_time = 0.0;
        current_move_frame = 0;
        if (right) {
            sword_attack_dir = "right";
        } else if (left) {
            sword_attack_dir = "left";
        } else {
            sword_attack_dir = facingRight ? "right" : "left";
        }
    }
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

bool Character::get_sword_attacking()
{
    return sword_attacking;
}


void Character::checkBounds(int windowWidth)
{
    if (get_x() > windowWidth + 50) {
        set_x(-50);
    } else if (get_x() < -50) {
        set_x(50);
    }
}




