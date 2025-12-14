#include "env.h"


Env::Env(Character *c1_, Character *c2_, double dt_)
    : c1(c1_)
    , c2(c2_)
    , dt(dt_ / 1000.0) {

        // Load sprite frames
        c1->loadSpriteFrames("resources/images/characters/redhat");
        c2->loadSpriteFrames("resources/images/characters/redhat");
        load_env_assets();
    }

void Env::load_env_assets(){
    Particle p1 = Particle{
        .pos {.x = 50.0, .y = 50.0},
        .expected_pos {.x = 50.0, .y = 50.0},
        .vel {.x = 0.0, .y = 0.0},
        .radius = 50.0,
        .mass = 1.0
    };

    particles.push_back(p1);

}

void Env::apply_static_constraints(){
    for (Particle& particle: particles) {
        double collision_dist = (particle.expected_pos.y - ground.height) * ground.norm - particle.radius;
        
        if (collision_dist <= 0) {
            double delta_y = -collision_dist * ground.norm;
            particle.expected_pos.y += delta_y;
        }
    }
}


void Env::update_velocities_and_positions(){
    for (Particle& particle : particles) {
        particle.vel.x = (particle.expected_pos.x - particle.pos.x) / dt;
        particle.vel.y = (particle.expected_pos.y - particle.pos.y) / dt;
        
        particle.pos.x = particle.expected_pos.x;
        particle.pos.y = particle.expected_pos.y;
    }
}

void Env::update(int width){
    apply_external_forces();           
    apply_static_constraints();        
    update_velocities_and_positions(); 
    
    c1->update();
    c1->checkBounds(width);
    c2->update();
    c2->checkBounds(width);
}

void Env::draw_assets(QPainter &painter){
    for (const Particle& particle: particles) {
        painter.drawEllipse(particle.pos.x, particle.pos.y, particle.radius, particle.radius);
    }
}

void Env::apply_external_forces(){
    for (Particle& particle : particles) {
        particle.vel.y += g * dt;
    }
    
    for (Particle& particle : particles) {
        particle.expected_pos.x = particle.pos.x + dt * particle.vel.x;
        particle.expected_pos.y = particle.pos.y + dt * particle.vel.y;
    }
}

void Env::paint(QPainter *painter, int width, int height){
    // Draw ground
    (*painter).setPen(Qt::NoPen);
    (*painter).setBrush(QColor(100, 200, 100));
    QRect visuals = QRect(0, ground.height, width, height - ground.height);
    (*painter).drawRect(visuals);
    draw_assets(*painter);
    c1->draw((*painter));

}

void Env::keyPressEvent(QKeyEvent *event){
     if (event->key() == Qt::Key_Up) {
        if (!c1->get_jumping()) {
            c1->set_jumping(true);
            c1->set_moving(false);
            
        }
    }
    if (event->key() == Qt::Key_Down) {
        if (!c1->get_jumping() && !c1->get_sliding()) {
            if (c1->get_moving()){
                c1->set_sliding(true);
            } else {
                c1->set_lowering(true);
                qDebug()<<"lowering";
            }
        }
    }
    if (event->key() == Qt::Key_Right) {
        c1->set_right(true);
        c1->set_left(false);
        if(c1->get_jumping() || c1->get_sliding()){
            c1->set_moving(false);
        } else {
            c1->set_moving(true);
        }
    } if (event->key() == Qt::Key_Left) {
        c1->set_left(true);
        c1->set_right(false);

        if(c1->get_jumping() || c1->get_sliding()){
            c1->set_moving(false);
        } else {
            c1->set_moving(true);
        }
    } if (event->key() == Qt::Key_A) {
        c1->set_sword_attacking(true);
        c1->set_lowering(false);
        c1->set_sliding(false);
        c1->set_jumping(false);
    } 

}

void Env::keyReleaseEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_Right) {
        c1->set_right(false);
        c1->set_moving(false);
    } else if (event->key() == Qt::Key_Left) {
        c1->set_left(false);
        c1->set_moving(false);
    } else if (event->key() == Qt::Key_Down) {
        c1->set_lowering(false);
    } else if (event->key() == Qt::Key_A) {
        c1->set_sword_attacking(false);
    }
}