#include "env.h"
#include <algorithm>
#include <cmath>

bool Env::check_rectangles_overlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

Env::Env(Character *c1_, Character *c2_, float dt_, float width_, float height_)
    : c1(c1_)
    , c2(c2_)
    , dt(dt_ / 1000.0)
    , width(width_)
    , height(height_) {

        // Load sprite frames
        c1->set_speed_move(speed_move);
        c1->set_speed_jump(speed_jump);
        c1->set_speed_run(speed_run);
        c1->set_hp(max_hp);
        c2->set_speed_move(speed_move);
        c2->set_speed_jump(speed_jump);
        c2->set_speed_run(speed_run);
        c2->set_hp(max_hp);

        characters.push_back(c1);
        characters.push_back(c2);

        load_env_assets();
    }



Env::~Env(){
    delete collider;

}

void Env::load_env_assets(){
    collider = new Collider();
    bonuses.push_back(std::make_unique<BonusBox>(Vec2(100.0, height-height/2.0), Vec2(100.0,25.0)));
    MovableCircle *p1 = new MovableCircle(
        {static_cast<float>(width)/2.0f+250.0f, static_cast<float>(height)/2.0f},
        {-100.0, -5.0},
        5.0,
        20.0
    );
    MovableCircle *p2 = new MovableCircle(
        {0.0f, static_cast<float>(height)/2.0f},
        {50.0, -5.0},
        5.0,
        20.0
    );
    c1->set_lifebar_dims(0.0,30.0,life_bar_width,20);
    c2->set_lifebar_dims(width - life_bar_width , 30.0, life_bar_width, 20.0 );
    particles.push_back(p1);
    particles.push_back(p2);
    Vec2 pos = Vec2(width / 2 - 200, height/2);
    Vec2 dims = Vec2(400.0, 100.0);
    // assets.push_back(std::make_unique<Rectangle>(Vec2(width/2.0 - 50.0, height-height/3.0), Vec2(100.0, 10)));
    assets.push_back(std::make_unique<Rectangle>(Vec2(-100.0, height - 100.0), Vec2(width + 200.0, 100)));
    assets.push_back(std::make_unique<Rectangle>(Vec2(width/2.0 - 50.0, height-height/3.0), Vec2(100.0,25.0)));

}





void Env::update_velocities_and_positions(){
    for (MovableCircle *particle : particles) {
        update_velocity_and_position(particle);

    }
    update_velocity_and_position(c1);
    update_velocity_and_position(c2);
}

void Env::update_velocity_and_position(MovableAsset *a){
    a->update_vel(dt);
    a->update_pos();
}


void Env::update(int width){
    apply_external_forces();
    update_expected_positions(); 
    collider->resolve_collisions(particles, characters, assets, bonuses);
    update_velocities_and_positions();
    apply_damping();
    apply_friction();  // Apply friction AFTER velocity update
    handle_attacks();
    c1->update(width);
    c2->update(width);
    for (auto it = bonuses.begin(); it != bonuses.end();){
        it->get()->update(dt, particles);
        if(it->get()->get_finished()){
            it = bonuses.erase(it);
        } else {
            ++it;
        }
    }
}

void Env::apply_damping() {
    for(MovableCircle *particle: particles){
        particle->set_v(particle->get_v() * particle->get_damp());
    }

}

void Env::apply_friction() {
    // Apply friction to particles
    const auto& particle_constraints = collider->get_static_constraints();
    for (const auto& constraint : particle_constraints) {
        MovableCircle* particle = particles[constraint.index];
        
        // Calculate tangent velocity (perpendicular to collision normal)
        Vec2 relativeVel = particle->get_v();
        float normalVel = relativeVel.dot(constraint.normal);
        Vec2 tangentVel = relativeVel - constraint.normal * normalVel;
        
        // Apply tangent friction
        float tangentSpeed = tangentVel.length();
        if (tangentSpeed > 0.0001) {
            Vec2 frictionDir = tangentVel / tangentSpeed;
            float frictionMag = std::min(friction * std::abs(normalVel), tangentSpeed);
            particle->set_v(particle->get_v() - frictionDir * frictionMag);
        }
        
    }
    
}

void Env::draw_assets(QPainter &painter){
    for (MovableCircle *particle: particles) {
        particle->draw(painter);
        qDebug()<<particle->get_pos().x;
        
    }
    // Draw obstacles
    painter.setBrush(QColor(200, 100, 100));
    for (const auto& asset : assets) {
            asset->draw(painter);
    }
    for (const auto& bonus: bonuses){
        bonus->draw(painter);
    }

}

void Env::apply_external_forces(){
    for (MovableCircle *particle : particles) {
        float new_v_y = particle->get_v_y() + g * dt;
        particle->set_v_y(new_v_y);
    }

    float new_v_y_c1 = c1->get_v_y() + g * dt;
    c1->set_v_y(new_v_y_c1);
    
    float new_v_y_c2 = c2->get_v_y() + g * dt;
    c2->set_v_y(new_v_y_c2);
}

void Env::update_expected_positions(){
    for (MovableCircle *particle : particles) {
        particle->update_expected_pos(dt);
    }
    c1->update_expected_pos(dt);
    c2->update_expected_pos(dt);

}


void Env::handle_sword_attack(Character* attacker, Character* defender){
    // Check if sword dims are available
    if (attacker->get_current_sword_dims().empty() || defender->get_current_character_dims().empty() || defender->get_current_asset_dims().empty()) {
        return;
    }
    float x_character_offset;
    float x_sword_offset;
    if(defender->get_right()){
        x_character_offset = defender->get_current_character_dims()[0] - defender->get_current_asset_dims()[0] - defender->get_current_asset_dims()[2];
    }
    else {
        x_character_offset = defender->get_current_asset_dims()[0] + defender->get_current_asset_dims()[2] - defender->get_current_character_dims()[0] - 2.0 * defender->get_current_character_dims()[2];
    }
    if(attacker->get_right()){
        x_sword_offset = attacker->get_current_sword_dims()[0] - attacker->get_current_asset_dims()[0] - attacker->get_current_asset_dims()[2];
    }
    else {
        x_sword_offset = attacker->get_current_asset_dims()[0] + attacker->get_current_asset_dims()[2] - attacker->get_current_sword_dims()[0] - 2.0 * attacker->get_current_sword_dims()[2];
    }
    
    float x_sword = attacker->get_x() + x_sword_offset;
    float y_sword = attacker->get_y() + attacker->get_current_sword_dims()[1] - attacker->get_current_asset_dims()[1] - attacker->get_current_asset_dims()[3];
    float w_sword = 2.0 * attacker->get_current_sword_dims()[2];
    float h_sword = 2.0 * attacker->get_current_sword_dims()[3];
    // Attacked character dims
    float x_character = defender->get_x() + x_character_offset;
    float y_character = defender->get_y() + defender->get_current_character_dims()[1] - defender->get_current_asset_dims()[1] - defender->get_current_asset_dims()[3];
    float w_character = 2.0 * defender->get_current_character_dims()[2];
    float h_character = 2.0 * defender->get_current_character_dims()[3];
    if (check_rectangles_overlap(x_sword, y_sword, w_sword, h_sword, x_character, y_character, w_character, h_character)) {
        qDebug()<<"Attack hit!";
        attacker->set_first_hit_sword_attack(false);
        defender->set_hp(defender->get_hp() - attacker->get_sword_attack_damages());
    }
}

void Env::handle_attacks(){
    if (c1->get_attacking()){
        if (c1->get_sword_attacking() && c1->get_first_hit_sword_attack()) {
            handle_sword_attack(c1, c2);
        }
    }
    if (c2->get_attacking()){
        if (c2->get_sword_attacking() && c2->get_first_hit_sword_attack()) {
            handle_sword_attack(c2, c1);
        }
    }
}

void Env::paint(QPainter *painter){
    draw_assets(*painter);
    c1->draw((*painter));
    c2->draw((*painter));
    

}

void Env::mousePressEvent(QMouseEvent* event) {
        particles.push_back(new MovableCircle(
        Vec2(event->pos().x(), event->pos().y()),
        {0.0, 0.0},
        3.0,
        10.0
    ));
    }

void Env::keyPressEvent(QKeyEvent *event){
    // Ignore auto-repeated events
    if (event->isAutoRepeat()) return;

    // Character 1 controls 
    if (event->key() == Qt::Key_Up) {
        if (!c1->get_jumping()) {
            c1->set_moving(false);
            c1->set_sword_attacking(false);
            c1->set_jumping(true);            
        }
    }
    if (event->key() == Qt::Key_Down) {
        if (!c1->get_jumping() && !c1->get_sliding() && !c1->get_sword_attacking()) {
            if (c1->get_moving()){
                c1->set_sliding(true);
                qDebug()<<"sliding";
            } else {
                c1->set_lowering(true);
                qDebug()<<"lowering";
            }
        }
    }
    if (event->key() == Qt::Key_Right) {
        c1->set_right(true);
        c1->set_right_pressed(true);
        c1->set_left(false);
        if(c1->get_jumping() || c1->get_sliding()){
            c1->set_moving(false);
        } else {
            c1->set_moving(true);

        }
    } if (event->key() == Qt::Key_Left) {
        c1->set_left(true);
        c1->set_left_pressed(true);
        c1->set_right(false);

        if(c1->get_jumping() || c1->get_sliding()){
            c1->set_moving(false);
        } else {
            c1->set_moving(true);

        }
    } if (event->key() == Qt::Key_0) {
        c1->set_lowering(false);
        c1->set_sliding(false);
        c1->set_jumping(false);
        c1->set_sword_attacking(true);
        
    }
    
    // Character 2 controls 
    if (event->key() == Qt::Key_Z) {
        if (!c2->get_jumping()) {
            c2->set_moving(false);
            c2->set_sword_attacking(false);
            c2->set_jumping(true);            
        }
    }
    if (event->key() == Qt::Key_S) {
        if (!c2->get_jumping() && !c2->get_sliding() && !c2->get_sword_attacking()) {
            if (c2->get_moving()){
                c2->set_sliding(true);
            } else {
                c2->set_lowering(true);
            }
        }
    }
    if (event->key() == Qt::Key_D) {
        c2->set_right(true);
        c2->set_right_pressed(true);
        c2->set_left(false);
        if(c2->get_jumping() || c2->get_sliding()){
            c2->set_moving(false);
        } else {
            c2->set_moving(true);

        }
    } if (event->key() == Qt::Key_Q) {
        c2->set_left(true);
        c2->set_left_pressed(true);
        c2->set_right(false);

        if(c2->get_jumping() || c2->get_sliding()){
            c2->set_moving(false);
        } else {
            c2->set_moving(true);

        }
    } if (event->key() == Qt::Key_A) {
        c2->set_lowering(false);
        c2->set_sliding(false);
        c2->set_jumping(false);
        c2->set_sword_attacking(true);
        
    } 

}

void Env::keyReleaseEvent(QKeyEvent *event){
    // Ignore auto-repeated events
    if (event->isAutoRepeat()) return;
    
    // Character 1 key releases
    if (event->key() == Qt::Key_Right) {
        c1->set_right(false);
        c1->set_right_pressed(false);
        if (!c1->get_left_pressed()) {
            c1->set_moving(false);
            c1->set_v_x(0.0);
        } else{
            c1->set_left(true);
            c1->set_moving(true);
        }
    } else if (event->key() == Qt::Key_Left) {
        c1->set_left(false);
        c1->set_left_pressed(false);
        if (!c1->get_right_pressed()) {
            c1->set_moving(false);
            c1->set_v_x(0.0);
        } else{
            c1->set_right(true);
            c1->set_moving(true);

        }
    } else if (event->key() == Qt::Key_Down) {
        c1->set_lowering(false);
    } else if (event->key() == Qt::Key_0) {
        c1->set_sword_attacking(false);
    }
    
    // Character 2 key releases
    if (event->key() == Qt::Key_D) {
        c2->set_right(false);
        c2->set_right_pressed(false);
        if (!c2->get_left_pressed()) {
            c2->set_moving(false);
            c2->set_v_x(0.0);
        } else{
            c2->set_left(true);
            c2->set_moving(true);
        }
    } else if (event->key() == Qt::Key_Q) {
        c2->set_left(false);
        c2->set_left_pressed(false);
        if (!c2->get_right_pressed()) {
            c2->set_moving(false);
            c2->set_v_x(0.0);
        } else{
            c2->set_right(true);
            c2->set_moving(true);

        }
    } else if (event->key() == Qt::Key_S) {
        c2->set_lowering(false);
    } else if (event->key() == Qt::Key_A) {
        c2->set_sword_attacking(false);
    }
}