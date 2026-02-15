#ifndef ENV_H
#define ENV_H

#include "collider.h"
#include "assets.h"
#include "character.h"
#include <QKeyEvent>
#include <vector>
#include "graphic_assets.h"
#include "utils.h"
#include <random>


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
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void load_env_assets();
    void apply_external_forces();
    void update_expected_positions();
    void update_velocities_and_positions();
    void apply_damping();
    void apply_friction();
    void update_velocity_and_position(MovableAsset *a);

    
    void draw_assets(QPainter &painter);
    void update_bonuses();


private:
    Character *c1;
    Character *c2;
    CommandPanel* panel1;
    CommandPanel* panel2;
    float t;
    float dt;
    float g = 2500;
    float max_hp {100.0};
    float friction = 0.001;
    float width;
    float height;
    float ground_height;
    float life_bar_width {100.0};
    float rest {0.8};
    float speed_move {75.0};
    float speed_run {150.0}; 
    float speed_jump {1000.0};
    float bomb_damages {20.0};
    float projectile_damages {10.0};
    float bonus_sample_time {5.0};
    float bonus_time {0.0};
    float bonus_next_spawn_time {10.0}; 
    int n_bonuses {3};
    Collider *collider;
    VisualContainer *visual_container;
    
    std::vector<std::unique_ptr<BonusBox>> bonuses;
    vector<MovableCircle*> particles;
    vector<Character*> characters;
    std::vector<std::unique_ptr<Asset>> assets;
    float bonuses_spawn_ranges[4];
    static std::mt19937 rng;
    

};



#endif