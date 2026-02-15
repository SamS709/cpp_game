#include "env.h"
#include <algorithm>
#include <cmath>

std::mt19937 Env::rng;



Env::Env(Character *c1_, Character *c2_, float dt_, float width_, float height_)
    : c1(c1_)
    , c2(c2_)
    , dt(dt_ / 1000.0)
    , width(width_)
    , height(height_) {
        ground_height = 100.0f;
        float margin_w = 70.0f;
        float margin_h = margin_w * height / width;
        bonuses_spawn_ranges[0] = margin_w;
        bonuses_spawn_ranges[1] = margin_h;
        bonuses_spawn_ranges[2] = width - 2.0 * margin_w;
        bonuses_spawn_ranges[3] = height - 2.0 * margin_h - ground_height;
        panel1 = new CommandPanel(Vec2(10, 100), Vec2(150, 100));
        panel1->set_player(1);

        panel2 = new CommandPanel(Vec2(width - 160, 100), Vec2(150, 100));
        panel2->set_player(2);
        
        c1->set_speed_move(speed_move);
        c1->set_speed_jump(speed_jump);
        c1->set_speed_run(speed_run);
        c1->set_hp(max_hp);
        c1->set_projectile_damages(projectile_damages);
        c2->set_speed_move(speed_move);
        c2->set_speed_jump(speed_jump);
        c2->set_speed_run(speed_run);
        c2->set_hp(max_hp);
        c2->set_projectile_damages(projectile_damages);



        characters.push_back(c1);
        characters.push_back(c2);

        load_env_assets();
        rng.seed(std::random_device{}());

    }



Env::~Env(){
    delete collider;

}

void Env::load_env_assets(){
    collider = new Collider();
    visual_container = new VisualContainer(100.0, false);
    c2->set_lifebar_dims(width - life_bar_width , 30.0, life_bar_width, 20.0 );
    c1->set_lifebar_dims(0.0,30.0,life_bar_width,20);
    Vec2 pos = Vec2(width / 2 - 200, height/2);
    Vec2 dims = Vec2(400.0, 100.0);
    // Ground
    std::unique_ptr<Rectangle> ground = std::make_unique<Rectangle>(Vec2(-100.0, height - ground_height), Vec2(width + 200.0, ground_height));
    ground->set_color(150,150,150,255);
    assets.push_back(std::move(ground));
    // Platforms
    float platfor_width = 100.0f;
    std::unique_ptr<Rectangle> p1 = std::make_unique<Rectangle>(Vec2(150.0, height-height/3.0), Vec2(platfor_width,25.0));
    p1->set_color(50,50,50,255);
    assets.push_back(std::move(p1));
    std::unique_ptr<Rectangle> p2 = std::make_unique<Rectangle>(Vec2(width - (platfor_width + 150.0), height-height/3.0), Vec2(platfor_width,25.0));
    p2->set_color(50,50,50,255);
    assets.push_back(std::move(p2));

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
    c1->update(width);
    c2->update(width);
    update_bonuses();
    
}

void Env::update_bonuses(){
    bonus_time += dt;
    if (bonus_time > bonus_sample_time && bonuses.size()< 6) {
        bonus_time = 0.0f;
        bool value = std::rand() & 1;
        if(value) {
            std::uniform_real_distribution<float> dist_x(bonuses_spawn_ranges[0], bonuses_spawn_ranges[2]);
            float x = dist_x(rng);    
            std::uniform_real_distribution<float> dist_y(bonuses_spawn_ranges[1], bonuses_spawn_ranges[3]);
            float y = dist_y(rng);
            std::uniform_int_distribution<int> dist_b(0, n_bonuses - 1);
            int b = dist_b(rng);
            bonuses.push_back(std::make_unique<BonusBox>(b, visual_container, Vec2(x, y), Vec2(100.0,25.0), dt));
        }
    }
    for (auto it = bonuses.begin(); it != bonuses.end();){
        it->get()->update(particles);
        if(it->get()->get_finished()){
            it = bonuses.erase(it);
        } else {
            ++it;
        }
    }
}

void Env::apply_damping() {
    for(MovableCircle *particle: particles){
        particle->set_v(particle->get_v() * particle->get_damp(), dt);
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
            particle->set_v(particle->get_v() - frictionDir * frictionMag, dt);
        }
        
    }
    
}

void Env::draw_assets(QPainter &painter){
    for (MovableCircle *particle: particles) {
        particle->draw(painter);
        
    }
    // Draw obstacles
    painter.setBrush(QColor(200, 100, 100));
    for (const auto& asset : assets) {
            asset->draw(painter);
    }
    for (const auto& bonus: bonuses){
        bonus->draw(painter);
    }
    panel1->draw(painter);
    panel2->draw(painter);

    painter.setBrush(QColor(200,200,200));

    // Uncomment to debut the area of random spawn of bonus box
    // painter.drawRect(static_cast<int>(bonuses_spawn_ranges[0]), static_cast<int>(bonuses_spawn_ranges[1]), static_cast<int>(bonuses_spawn_ranges[2]), static_cast<int>(bonuses_spawn_ranges[3]));

    c1->draw((painter));
    c2->draw((painter));

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





void Env::paint(QPainter *painter){
    draw_assets(*painter);
    
    

}

void Env::mousePressEvent(QMouseEvent* event) {
        particles.push_back(new Bomb(
        visual_container->bomb_sprites,
        Vec2(event->pos().x(), event->pos().y()),
        {0.0, 0.0},
        5.0,
        3.0,
        10.0,
        5.0
    ));
    }

void Env::keyPressEvent(QKeyEvent *event){
    // Ignore auto-repeated events
    if (event->isAutoRepeat()) return;

    // Character 1 controls 
    if (event->key() == Qt::Key_Up) {
        if (!c1->get_double_jumping()) {
            c1->set_moving(false);
            c1->set_sword_attacking(false);
            c1->set_jumping(true);            
        }
    }
    if (event->key() == Qt::Key_Down) {
        if (!c1->get_jumping() && !c1->get_sliding() && !c1->get_sword_attacking()) {
            if (c1->get_moving()){
                c1->set_sliding(true);
            } else {
                c1->set_lowering(true);
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
        if(c1->get_lowering()){
            c1->set_sword_attacking_low(true);
        } else {
            c1->set_sword_attacking(true);
        }
        c1->set_lowering(false, true);
        c1->set_sliding(false);
        c1->set_jumping(false);
        
        
    } if (event->key() == Qt::Key_1) {
        c1->set_projectile_attacking(true);
        
        
    }
    
    // Character 2 controls 
    if (event->key() == Qt::Key_Z) {
        if (!c2->get_double_jumping()) {
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
        if(c2->get_lowering()){
            c2->set_sword_attacking_low(true);
        } else {
            c2->set_sword_attacking(true);
        }
        c2->set_lowering(false, true);
        c2->set_sliding(false);
        c2->set_jumping(false);
        
        
    } if (event->key() == Qt::Key_E) {
        c2->set_projectile_attacking(true);
        
    } if (event->key() == Qt::Key_R) {
        c2->set_defending_attack(true);
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
        c1->set_sword_attacking_low(false);
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
    } else if (event->key() == Qt::Key_R) {
        c2->set_defending_attack(false);
    }
}