#include "character.h"
#include "utils.h"

Character::Character(Vec2 pos_, float time_betwwen_frames_, float mass_, float max_hp_, vector<int> color_)
    : MovableRectangle(mass)
    , time_between_frames(time_betwwen_frames_)
    , max_hp(max_hp_)
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
    set_color(color_[0], color_[1], color_[2], color_[3]);
    loadSpriteFrames("resources/images/characters/redhat");
    current_asset_dims = move_frames_asset_dims[0];
    lifebar = new Lifebar(Vec2(100,20));
    projectile_time = projectile_min_time + 0.1f;
    
}

void Character::set_lifebar_dims(float x, float y, float w, float h) {
    lifebar->set_x(x);
    lifebar->set_y(y);
    lifebar->set_w(w);
    lifebar->set_h(h);
    // lifebar->reload_image();
}

void Character::loadSpriteFrames(const QString &basePath)
{
    load_move_frames(basePath);
    load_run_frames(basePath);
    load_jump_frames(basePath); 
    load_slide_frames(basePath);
    load_lower_frames(basePath);
    load_sword_attack_frames(basePath);
    load_sword_attack_low_frames(basePath);
    load_defend_attack_frames(basePath);
    load_projectile_sprites(basePath);
    
}

void Character::load_move_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "walk", 1, 39, move_frames, move_frames_asset_dims, move_frames_character_dims);
    
    // Use first move frame as idle frame
    if (!move_frames.isEmpty()) {
        idleFrame = move_frames[5];
        idle_dims = move_frames_asset_dims[5];
        idle_character_dims = move_frames_character_dims[5];
        qDebug() << "Loaded" << move_frames.size() << "sprite frames";
    }
}

void Character::load_run_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "run", 1, 39, run_frames, run_frames_asset_dims, run_frames_character_dims);
    qDebug() << "Loaded" << run_frames.size() << "run frames";
}

void Character::load_jump_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "jump", 0, 63, jump_frames, jump_frames_asset_dims, jump_frames_character_dims);
}

void Character::load_slide_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "slide", 0, 29, slide_frames, slide_frames_asset_dims, slide_frames_character_dims);
}

void Character::load_lower_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "lower", 0, 10, lower_frames, lower_frames_asset_dims, lower_frames_character_dims);
}

void Character::load_sword_attack_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "sword_attack", 1, 53, sword_attack_frames, 
                       sword_attack_frames_asset_dims, sword_attack_frames_character_dims, 
                       &sword_attack_frames_sword_dims);
    
    // set the dim of the sword to zero during first and last few frames of the attack 
    int ind = 0;
    for (vector<float> &sword_dim: sword_attack_frames_sword_dims) {
        if(ind < 20 || ind > sword_attack_frames_sword_dims.size() - 20){
            sword_dim = {0.0f, 0.0f, 0.0f, 0.0f};
        }
        ind ++;
    }
}

void Character::load_sword_attack_low_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "sword_attack_low", 1, 17, sword_attack_low_frames, 
                       sword_attack_low_frames_asset_dims, sword_attack_low_frames_character_dims, 
                       &sword_attack_low_frames_sword_dims);
    
    int ind = 0;
    for (vector<float> &sword_dim: sword_attack_low_frames_sword_dims) {
        if(ind < 2 || ind > sword_attack_low_frames_sword_dims.size() - 2){
            sword_dim = {0.0f, 0.0f, 0.0f, 0.0f};
        }
        ind ++;
    }
}

void Character::load_defend_attack_frames(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    loader.loadSequence(basePath, "defend_attack", 1, 50, defend_attack_frames, 
                       defend_attack_frames_asset_dims, defend_attack_frames_character_dims, 
                       &defend_attack_frames_sword_dims);
    qDebug() << "Loaded" << defend_attack_frames.size() << "defend_attack frames";
}

void Character::load_projectile_sprites(const QString &basePath){
    SpriteLoader loader(c_scale, draw_baxes, get_color());
    vector<vector<float>> asset_dims, character_dims;
    loader.loadSequence(basePath, "projectile", 1, 2, 
                       projectile_sprites, asset_dims, character_dims);
    
    // Initialize the projectile with the loaded sprites
    projectile = new Projectile(projectile_sprites, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 10.0f, 1.0f, 15.0f, 0.05f);
}



void Character::update(int width)
{
    qDebug()<<lowering;
    if (!moving) {
        time_until_move_button_released += time_between_frames/1000.0f;
    }
    if(runing) {
        update_run();
    } else {
        time_without_runing += time_between_frames/1000.f;
    }
    slide_time += time_between_frames/1000.0f; // slide_time also counts when not sliding
    sword_attack_time += time_between_frames/1000.0f; // sword_attack_time also counts when not attacking
    sword_attack_low_time += time_between_frames/1000.0f; // sword_attack_time also counts when not attacking
    walk_step_time += time_between_frames/1000.0f; // walk_step_time always counts
    run_step_time += time_between_frames/1000.0f; // run_step_time always counts
    if (projectile_attacking && !projectile->get_hit_finished()){
        projectile_time += time_between_frames/1000.0f;
        update_projectile();
    }
    if (jumping) {
        update_jump();
    }
    else if (sliding) {
        update_slide();
    }
    else if (lowering){
        update_lower();
    }
    else if (defending_attack){
        update_defending_attack();
    }
    else if (sword_attacking || sword_attacking_low){
        update_sword_attack();
    } else if (defending_attack) {
        update_defending_attack();
    }
    
    else if(moving){
        update_move();
    } 

    else if (!(idleFrame.isNull() | idle_dims.empty())) {
        current_asset_dims = idle_dims;
        current_character_dims = idle_character_dims;
        currentSprite = &idleFrame;
    }

    checkBounds(width);
    
}

void Character::update_projectile(){
    if(projectile_time > total_projectile_time){
        set_projectile_attacking(false);
    }
    projectile->update(time_between_frames/1000.0f, projectile_time, projectile_min_time);
}

void Character::update_jump(){
    jump_time += time_between_frames/1000.0f;
    
    // Update animation frame based on velocity - smooth interpolation
    if (!jump_frames.isEmpty()) {
        float v_y = get_v_y();
        int num_frames = jump_frames.size();
        
        // Map velocity to frame index
        // v_y ranges from -jump_velocity (rising) to +jump_velocity (falling)
        // Map to frame 0 (start of jump) -> num_frames/2 (peak) -> num_frames-1 (landing)
        
        float velocity_range = speed_jump * 2.0f;  // Total velocity range
        float normalized = (v_y + speed_jump) / velocity_range;  // 0.0 to 1.0
        normalized = std::max(0.0f, std::min(1.0f, normalized));  // Clamp
        
        // Map to frame index
        current_move_frame = (int)(normalized * (num_frames - 1));
        current_move_frame = std::max(0, std::min(current_move_frame, num_frames - 1));
    }
    currentSprite = &jump_frames[current_move_frame];
    current_asset_dims = jump_frames_asset_dims[current_move_frame];
    current_character_dims = jump_frames_character_dims[current_move_frame];
}


void Character::update_lower(){
    lower_time += time_between_frames/1000;
    if (!lower_frames.isEmpty()) {
        float normalized = 1.0f;
        int num_frames = lower_frames.size();
        if (lower_time<total_lower_time){
            if(lower_toggled) {
                normalized = lower_time / total_lower_time;
            } else {
                normalized = 1.0f - lower_time / total_lower_time;
            }
        } else if (lowering_stop){
            lowering_stop = false;
            lowering = false;
            normalized = 0.0f;
            if(sword_attacking_low){
                sword_attack_low_time = 0.0f;
            }
        }
        current_move_frame = (int)(normalized * (num_frames - 1));
    }
    currentSprite = &lower_frames[current_move_frame];
    current_asset_dims = lower_frames_asset_dims[current_move_frame];
    current_character_dims = lower_frames_character_dims[current_move_frame];
}

void Character::update_defending_attack(){
    defending_attack_time += time_between_frames/1000;
    if (!defend_attack_frames.isEmpty()) {
        float normalized = 1.0f;
        int num_frames = defend_attack_frames.size();
        if (defending_attack_time<total_defending_attack_time){
            if(defending_attack_toggled) {
                normalized = defending_attack_time / total_defending_attack_time;
            } else {
                normalized = 1.0f - defending_attack_time / total_defending_attack_time;
            }
        } else if (defending_attack_stop){
            defending_attack_stop = false;
            defending_attack = false;
            normalized = 0.0f;
        }
        current_move_frame = (int)(normalized * (num_frames - 1));
    }
    currentSprite = &defend_attack_frames[current_move_frame];
    current_asset_dims = defend_attack_frames_asset_dims[current_move_frame];
    current_character_dims = defend_attack_frames_character_dims[current_move_frame];
    current_sword_dims = defend_attack_frames_sword_dims[current_move_frame];
}


void Character::update_slide(){
    if(slide_time > total_slide_time) {
        sliding = false;
        slide_time = 0.0f;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;
        }
    } else {
        // Calculate which frame to show based on elapsed time
        if (!slide_frames.isEmpty()) {
            float p = slide_time / total_slide_time;
            int target_frame = (int)(pow(p, 0.5f) * slide_frames.size());
            current_move_frame = std::min(target_frame, (int)slide_frames.size() - 1);
            float delta_x = (static_cast<float>(facingRight)-0.5)*slide_dist*(1-p)*(1-p);
            pos.x+= delta_x;
            pos_exp.x+= delta_x;
        }
        
        // No horizontal movement during slide, just maintain direction
    }
    currentSprite = &slide_frames[current_move_frame];
    current_asset_dims = slide_frames_asset_dims[current_move_frame];
    current_character_dims = slide_frames_character_dims[current_move_frame];
    currentSprite = &slide_frames[current_move_frame];
}

void Character::update_sword_attack(){
    
    float *attack_time;
    float *total_attack_time;
    float *attack_dist = &sword_attack_dist;
    bool *s_attacking;
    QVector<QPixmap> *attack_frames;
    vector<vector<float>> *attack_frames_asset_dims;
    vector<vector<float>> *attack_frames_character_dims;
    vector<vector<float>> *attack_frames_sword_dims;


    if(sword_attacking_low){
        attack_time = &sword_attack_low_time;
        total_attack_time = &total_sword_attack_low_time;
        attack_dist = &sword_attack_low_dist;
        s_attacking = &sword_attacking_low;
        attack_frames = &sword_attack_low_frames;
        attack_frames_asset_dims = &sword_attack_low_frames_asset_dims;
        attack_frames_character_dims = &sword_attack_low_frames_character_dims;
        attack_frames_sword_dims = &sword_attack_low_frames_sword_dims;
        
    } else {
        
        attack_time = &sword_attack_time;
        total_attack_time = &total_sword_attack_time;
        attack_dist = &sword_attack_dist;
        s_attacking = &sword_attacking;
        attack_frames = &sword_attack_frames;
        attack_frames_asset_dims = &sword_attack_frames_asset_dims;
        attack_frames_character_dims = &sword_attack_frames_character_dims;
        attack_frames_sword_dims = &sword_attack_frames_sword_dims;
    }
    if(*attack_time > *total_attack_time) {
        *s_attacking = false;
        attacking = false;
        *attack_time = 0.0f;
        current_move_frame = 0;
        // Resume moving if a direction key is still pressed
        if (right || left) {
            moving = true;  
        }
    } else {    
        // Calculate which frame to show based on elapsed time
        if (!(*attack_frames).isEmpty()) {
            float percentage = *attack_time / *total_attack_time;
            int target_frame = (int)(percentage * (*attack_frames).size());
            if(percentage<0.5){
                pos.x+=(static_cast<float>(facingRight)-0.5)*(*attack_dist);
                pos_exp.x+=(static_cast<float>(facingRight)-0.5)*(*attack_dist);
            } else{
                pos.x-=(static_cast<float>(facingRight)-0.5)*(*attack_dist);
                pos_exp.x-=(static_cast<float>(facingRight)-0.5)*(*attack_dist);
            }               
            current_move_frame = std::min(target_frame, (int)(*attack_frames).size() - 1);
            currentSprite = &(*attack_frames)[current_move_frame];
            current_asset_dims = (*attack_frames_asset_dims)[current_move_frame];
            current_character_dims = (*attack_frames_character_dims)[current_move_frame];
            current_sword_dims = (*attack_frames_sword_dims)[current_move_frame];
        }
        
        // No horizontal movement during slide, just maintain direction
    }
    
}

void Character::update_run(){

    run_time += time_between_frames/1000.0f;
     if (run_frames.isEmpty()) {
        // Fallback to move frames if run frames not loaded
        update_move();
        return;
    }
    moving = false;
    
    if (right) {
        
        float cycle_time = fmod(run_step_time, total_run_step_time);
        int target_frame = (int)((cycle_time / total_run_step_time) * run_frames.size());
        current_move_frame = std::min(target_frame, (int)run_frames.size() - 1);
        
    } else if (left) {
        
        float cycle_time = fmod(run_step_time, total_run_step_time);
        int target_frame = (int)((cycle_time / total_run_step_time) * run_frames.size());
        current_move_frame = std::min(target_frame, (int)run_frames.size() - 1);
        
    } else {
        current_move_frame = 0;
        run_step_time = 0.0f;
    }
    currentSprite = &run_frames[current_move_frame];
    current_asset_dims = run_frames_asset_dims[current_move_frame];
    current_character_dims = run_frames_character_dims[current_move_frame];
}

void Character::update_move(){
    if (right) {
        // x += 0.5;
        
        if (!move_frames.isEmpty()) {
            float cycle_time = fmod(walk_step_time, total_walk_step_time);
            int target_frame = (int)((cycle_time / total_walk_step_time) * move_frames.size());
            current_move_frame = std::min(target_frame, (int)move_frames.size() - 1);
        }
    } else if (left) {
        // x -= 0.5;
        
        if (!move_frames.isEmpty()) {
            float cycle_time = fmod(walk_step_time, total_walk_step_time);
            int target_frame = (int)((cycle_time / total_walk_step_time) * move_frames.size());
            current_move_frame = std::min(target_frame, (int)move_frames.size() - 1);
        }
    } else {
        current_move_frame = 0;
        walk_step_time = 0.0f;
    }
    currentSprite = &move_frames[current_move_frame];
    current_asset_dims = move_frames_asset_dims[current_move_frame];
    current_character_dims = move_frames_character_dims[current_move_frame];
}

void Character::set_hp(float hp_) {
    float real_hp = hp_;
    if (hp_ < 0.0f) {
        is_dead = true;
        real_hp = 0.0f;
    } else if (hp_ > max_hp) {
        real_hp = max_hp;
    } 
    hp = real_hp; 
    lifebar->set_percentage(real_hp/max_hp);
}

void Character::draw(QPainter &painter)
{

    // Update dims based on current sprite
    if (currentSprite && !currentSprite->isNull()) {
        set_w(2*current_asset_dims[2]);
        set_h(2*current_asset_dims[3]);
    }
    
    // Draw the sprite
    if (currentSprite && !currentSprite->isNull()) {
        painter.save();

        // Calculate position for drawing (offset from the character dims)
        float drawX = get_x() - current_asset_dims[0]-current_asset_dims[2];
        float drawY = get_y() - current_asset_dims[1]-2.0f*current_asset_dims[3];
        
        // Flip horizontally if facing left
        handle_rotate(painter);
        painter.drawPixmap(drawX, drawY, *currentSprite);
        painter.restore();
    }   
    lifebar->draw(painter);
    if(projectile_attacking) {
        projectile->draw(painter);
    }
}

void Character::handle_rotate(QPainter &painter){
    if(attacking){
        if (sword_attack_dir == "left"){
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

float Character::get_x_sliding(float t) {
    return slide_dist * (total_slide_time - t) / (total_slide_time );
}

float Character::get_x_sword_attacking(float t) {
    return sword_attack_dist * (total_sword_attack_time - t) / (total_sword_attack_time );
}

void Character::set_moving(bool m)
{
    moving = m;
    if (m) {
        if (right) {
            if(time_until_move_button_released < time_click_to_run && run_time < total_run_time && time_without_runing > time_between_run) {
                set_v_x(speed_run);
                run_time = 0.0f; 
                runing = true;
                time_without_runing = 0.0;
            } else {
                set_v_x(speed_move);
            }
            
        }
        else {
            if(time_until_move_button_released < time_click_to_run && run_time < total_run_time && time_without_runing > time_between_run) {
                set_v_x(-speed_run);    
                run_time = 0.0f;           
                runing = true;
                time_without_runing = 0.0;
            } else {
                set_v_x(-speed_move);
            }
        }
    } else {
        runing = false;
        run_time = 0.0;
        time_until_move_button_released = 0.0;

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
    if(jumping && jumping_){
        double_jumping = jumping_;
    }
    jumping = jumping_;
    current_move_frame = 0;
    sliding = false;
    if (jumping_) {
        set_v_y(-speed_jump);
    }
    else{
        double_jumping = false;;
    }
}

void Character::set_sliding(bool s)
{
    if (slide_time > time_between_slides) {
        moving = false;

        sliding = s;
        slide_time = 0.0;
        current_move_frame = 0;
    }
}

void Character::set_lowering(bool s , bool combo)
{

    if(lowering != s) {
        lower_time = 0.0f;
    }
    if(s){
        lowering = s;
        lower_toggled = true;
        lowering_stop = false;
    } else {
        if (combo) {
            lowering = false;
            lower_toggled = false;
            lowering_stop = false;
        } else {
            lowering = false;
            lower_toggled = false;
            lowering_stop = true;
            lower_time = 0.0;
        }
    }
}

void Character::set_defending_attack(bool d, bool combo)
{

    if(defending_attack != d) {
        defending_attack_time = 0.0f;
    }
    if(d){
        defending_attack = d;
        defending_attack_toggled = true;
        defending_attack_stop = false;
    } else {
        if (combo) {
            defending_attack = false;
            defending_attack_toggled = false;
            defending_attack_stop = false;
        } else {
            defending_attack = true;
            defending_attack_toggled = false;
            defending_attack_stop = true;
            defending_attack_time = 0.0;
        }
    }
}

void Character::set_sword_attacking(bool a)
{
    if (sword_attack_time > time_between_sword_attacks) {
        moving = false;
        sword_attacking = a;
        first_hit_sword_attack = true;
        attacking = a;
        sword_attack_time = 0.0;
        current_move_frame = 0;
        
        sword_attack_dir = facingRight ? "right" : "left";
        
    }
}

void Character::set_sword_attacking_low(bool a)
{
    if (sword_attack_low_time > time_between_sword_attacks_low) {
        moving = false;
        sword_attacking_low = a;
        first_hit_sword_attack = true;
        attacking = a;
        sword_attack_low_time = 0.0;
        current_move_frame = 0;
        
        sword_attack_dir = facingRight ? "right" : "left";
        
    }
}

void Character::set_projectile_attacking(bool a) {
    if(a && projectile_time>projectile_min_time){
        projectile_attacking = a;
        projectile_time = 0.0f;
        projectile->set_hit_started(false);
        projectile->set_hit_finished(false);
        projectile->set_disparition_time(0.0f);
        projectile->set_x(get_x());
        projectile->set_y(get_y() - get_h()/2.0f);
        float v_projectile = 1000.0f;
        projectile->set_v_x(v_projectile * 2.0f * (static_cast<float>(facingRight) - 0.5f));
    } else if(!a) {
        projectile_attacking = a;
        projectile_time = projectile_min_time + 0.1;
    }

}



void Character::checkBounds(int windowWidth)
{
    if (get_x() > windowWidth + 50) {
        set_x(-50);
    } else if (get_x() < -50) {
        set_x(50);
    }
}




