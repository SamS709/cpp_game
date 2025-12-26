#ifndef ENV_H
#define ENV_H

#include "assets.h"
#include "character.h"
#include <QKeyEvent>
#include <vector>

using namespace std;



struct Ground{
    double height = 500.0;
    double norm = -1.0; 
};

class Env {

public:
    Env(Character *c1_, Character *c2_, double dt_, double width_, double height_);
    void update(int width);
    void paint(QPainter *painter);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void load_env_assets();
    void apply_external_forces();
    void update_expected_poses();
    void apply_static_constraints();
    void apply_static_constraint(MovableAsset *a);
    void resolve_aabb_collision(MovableAsset *movable, Rectangle *rect);
    void update_velocity_and_position(MovableAsset *a);

    void update_velocities_and_positions();
    void draw_assets(QPainter &painter);

    bool intersect(MovableCircle, Rectangle);

private:
    Character *c1;
    Character *c2;
    double t;
    double dt;
    double g = 10000;
    double width;
    double height;
    double rest {0.8};
    double speed_move {75.0};
    double speed_run; 
    double speed_jump {2500.0};
    Rectangle *ground;
    Rectangle *obstacle;
    vector<MovableCircle*> particles;
};



#endif