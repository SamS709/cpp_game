#include "env.h"


Env::Env(Character *c1_, Character *c2_, double dt_, double width_, double height_)
    : c1(c1_)
    , c2(c2_)
    , dt(dt_ / 1000.0)
    , width(width_)
    , height(height_) {

        // Load sprite frames
        c1->loadSpriteFrames("resources/images/characters/redhat");
        c2->loadSpriteFrames("resources/images/characters/redhat");
        load_env_assets();
    }

void Env::load_env_assets(){
    ground = new Rectangle({0.0, height - 100},{width, 100});
    MovableCircle *p1 = new MovableCircle(
        {50.0, 50.0},
        {0.0, 0.0},
        5.0,
        20.0
    );

    particles.push_back(p1);

}

void Env::apply_static_constraints(){
    for (MovableCircle *particle: particles) {
        double collision_dist = (particle->get_y_expected() + particle->get_radius()) - ground->get_y();
        if (collision_dist <= 0) {
            double delta_y = -collision_dist * ground->get_norm(particle->get_x(), particle->get_y());
            particle->update_expected_pos_collision(0.0, delta_y);
            
        }
    }

}


void Env::update_velocities_and_positions(){
    for (MovableCircle *particle : particles) {
        particle->update_vel(dt);
        particle->update_pos();

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
    for (MovableCircle *particle: particles) {
        painter.drawEllipse(particle->get_x(), particle->get_y(), particle->get_radius(), particle->get_radius());
    }
}

void Env::apply_external_forces(){
    for (MovableCircle *particle : particles) {
        double new_v_y = particle->get_v_y() + g * dt;
        particle->set_v_y(new_v_y);
    }
    
    for (MovableCircle *particle : particles) {
        particle->update_expected_pos(dt);
    }
}

void Env::paint(QPainter *painter){
    // Draw ground
    (*painter).setPen(Qt::NoPen);
    (*painter).setBrush(QColor(100, 200, 100));
    QRect visuals = QRect(ground->get_x(), ground->get_y(), ground->get_w(), ground->get_h());
    (*painter).drawRect(visuals);
    draw_assets(*painter);
    c1->draw((*painter));

}

void Env::keyPressEvent(QKeyEvent *event){
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
        c1->set_lowering(false);
        c1->set_sliding(false);
        c1->set_jumping(false);
        c1->set_sword_attacking(true);
        
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