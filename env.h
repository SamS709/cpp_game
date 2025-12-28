#ifndef ENV_H
#define ENV_H

#include "assets.h"
#include "character.h"
#include <QKeyEvent>
#include <vector>
#include "lifebar.h"

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
    void update_expected_positions();
    void update_velocities_and_positions();
    void add_static_contact_constraints();
    void apply_damping();
    void deleteContactConstraints();
    // void apply_static_constraints();
    // void apply_static_constraint(MovableAsset *a);
    // void resolve_aabb_collision(MovableAsset *movable, Rectangle *rect);
    void update_velocity_and_position(MovableAsset *a);
    void enforce_static_ground_constraints(const StaticConstraint& constraint, MovableCircle& particle);
    void project_constraints();

    
    void update_hp();
    void draw_assets(QPainter &painter);
    void handle_attacks();
    void handle_sword_attack(Character* attacker, Character* defender);

    bool intersect(MovableCircle, Rectangle);
    bool check_rectangles_overlap(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);

private:
    Character *c1;
    Character *c2;
    double t;
    double dt;
    double g = 1000;
    double max_hp {100.0};
    double width;
    double height;
    double life_bar_width {100.0};
    double rest {0.8};
    double speed_move {75.0};
    double speed_run; 
    double speed_jump {500.0};
    Rectangle *ground;
    Rectangle *obstacle;
    vector<MovableCircle*> particles;
    std::vector<std::unique_ptr<Collider>> colliders;
    std::vector<StaticConstraint> staticConstraints;
    std::vector<DynamicConstraint> dynamicConstraints;
    

};



#endif