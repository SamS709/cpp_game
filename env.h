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
    void apply_static_constraints();
    void update_velocities_and_positions();
    void draw_assets(QPainter &painter);

private:
    Character *c1;
    Character *c2;
    double t;
    double dt;
    double g = 981;
    double width;
    double height;
    Rectangle *ground;
    vector<MovableCircle*> particles;
};



#endif