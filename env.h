#ifndef ENV_H
#define ENV_H

#include "collider.h"
#include "assets.h"
#include "character.h"
#include <QKeyEvent>
#include <vector>
#include "lifebar.h"

using namespace std;

struct Ground{
    float height = 500.0;
    float norm = -1.0; 
};

class Env {

public:
    Env(Character *c1_, Character *c2_, float dt_, float width_, float height_);
    ~Env();
    void update(int width);
    void paint(QPainter *painter);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void load_env_assets();
    void apply_external_forces();
    void update_expected_positions();
    void update_velocities_and_positions();
    void apply_damping();
    void apply_friction();
    // void apply_static_constraints();
    // void apply_static_constraint(MovableAsset *a);
    // void resolve_aabb_collision(MovableAsset *movable, Rectangle *rect);
    void update_velocity_and_position(MovableAsset *a);

    
    void draw_assets(QPainter &painter);
    void handle_attacks();
    void handle_sword_attack(Character* attacker, Character* defender);

    bool check_rectangles_overlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

private:
    Character *c1;
    Character *c2;
    float t;
    float dt;
    float g = 2500;
    float max_hp {100.0};
    float friction = 0.001;
    float width;
    float height;
    float life_bar_width {100.0};
    float rest {0.8};
    float speed_move {75.0};
    float speed_run; 
    float speed_jump {1000.0};
    Collider *collider;
    vector<MovableCircle*> particles;
    vector<Character*> characters;
    std::vector<std::unique_ptr<Asset>> assets;
    

};



#endif