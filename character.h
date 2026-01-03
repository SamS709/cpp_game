#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <cmath>
#include <vector>
#include "assets.h"
#include <QDebug>
#include "lifebar.h"

using namespace std;

class Character: public MovableRectangle
{
public:
    Character(Vec2 pos_, float time_betwwen_frames_, float mass, float max_hp_);
    
    void loadSpriteFrames(const QString &basePath);
    void load_jump_frames(const QString &basePath);
    void load_move_frames(const QString &basePath);
    void load_slide_frames(const QString &basePath);
    void load_lower_frames(const QString &basePath);
    void load_sword_attack_frames(const QString &basePath);


    void update(int width);
    void update_jump();
    void update_move();
    void update_slide();
    void update_lower();
    void update_sword_attack();

    void draw(QPainter &painter);    
    void set_x(float x_) {pos.x = x_;}
    void set_y(float y_) {pos.y = y_;}
    void set_x_expected(float x_expected_) { pos_exp.x = x_expected_ ; }
    void set_y_expected(float y_expected_) { pos_exp.y = y_expected_; }
    void update_vel(float dt){ v.x = (get_x_expected() - get_x()) / dt; v.y = (get_y_expected() - get_y()) / dt; }
    void update_expected_pos(float dt){ pos_exp.x = get_x() + v.x * dt; pos_exp.y = get_y() + v.y * dt; }
    void update_pos(){ pos.x = pos_exp.x; pos.y = pos_exp.y; }
    void update_expected_pos_collision(float delta_x, float delta_y){ pos_exp.x += delta_x; pos_exp.y += delta_y; }
    void set_c_scale(float s) {c_scale = s;}
    void set_hitbox(QPixmap& sprite, vector<vector<float>>* asset_dims_, vector<vector<float>>* character_dims_, vector<vector<float>>* sword_dims_);
    void set_draw_baxes(bool d) { draw_baxes = d; }
    void set_right(bool r);
    void set_right_pressed(bool r) {right_pressed = r;}
    void set_left(bool l);
    void set_left_pressed(bool l) {left_pressed = l;}
    void set_jumping(bool j);
    void set_sliding(bool s);
    void set_lowering(bool s);
    void set_sword_attacking(bool a);
    void set_speed_move(float s) {speed_move = s;}
    void set_speed_jump(float s) {speed_jump = s;}
    void set_speed_run(float s) {speed_run = s;}
    void set_hp(float hp_) {hp = hp_; lifebar->set_percentage(hp/max_hp);}
    void set_first_hit_sword_attack(bool f_h) {first_hit_sword_attack = f_h;}
    void set_lifebar_dims(float x, float y, float w, float h);

    float get_rest() const override { return 0.0; } // No bounce for characters

    // Drawing positions (with sprite offset)
    float get_y() const { return pos.y;} 
    float get_x() const { return pos.x;} 
    float get_x_expected() const { return pos_exp.x;} 
    float get_y_expected() const { return pos_exp.y;}


    void set_moving(bool m);

    float get_mass() const { return mass; } 
    float get_x_sliding(float t);
    float get_x_sword_attacking(float t);
    float get_c_scale() const { return c_scale; }
    float get_hp() const { return hp; }
    float get_current_sprite_width() {return currentSprite->width();}
    float get_sword_attack_damages() { return sword_attack_damages; }
    vector<float> get_current_asset_dims() {return current_asset_dims; }
    vector<float> get_current_character_dims() {return current_character_dims; }
    vector<float> get_current_sword_dims() {return current_sword_dims; }



    bool get_moving() { return moving; };
    bool get_jumping() { return jumping; };
    bool get_sliding() {return sliding; };
    bool get_attacking() { return attacking; }
    bool get_sword_attacking() {return sword_attacking; };
    bool get_right() { return facingRight; }
    bool get_right_pressed() { return right_pressed; }  // Check if right key is pressed
    bool get_left_pressed() { return left_pressed; }    // Check if left key is pressed
    bool get_first_hit_sword_attack() { return first_hit_sword_attack; }    // Check if left key is pressed


    void handle_rotate(QPainter &painte);
    
    void checkBounds(int windowWidth);   

private:
    float x;
    float y;
    bool moving = false;
    bool jumping = false;
    bool sliding = false;
    bool lowering = false;
    bool lowering_start = false;
    bool lowering_stop = false;
    bool attacking = false;
    bool sword_attacking = false;
    bool right;
    bool left;
    bool left_pressed;
    bool right_pressed;
    bool facingRight;
    bool first_hit_sword_attack;
    
    // Sprite animation
    QVector<QPixmap> move_frames;
    vector<vector<float>> move_frames_asset_dims;
    vector<vector<float>> jump_frames_asset_dims;
    vector<vector<float>> lower_frames_asset_dims;
    vector<vector<float>> sword_attack_frames_asset_dims;
    vector<vector<float>> move_frames_character_dims;
    vector<vector<float>> jump_frames_character_dims;
    vector<vector<float>> lower_frames_character_dims;
    vector<vector<float>> sword_attack_frames_character_dims;
    vector<vector<float>> sword_attack_frames_sword_dims;
    QVector<QPixmap> jump_frames;
    QVector<QPixmap> slide_frames;
    QVector<QPixmap> lower_frames;
    QVector<QPixmap> sword_attack_frames;
    QVector<float> bounds;
    Lifebar *lifebar;
    QPixmap *currentSprite = nullptr;
    vector<float> current_asset_dims;
    vector<float> current_character_dims;
    vector<float> current_sword_dims;
    vector<float> idle_dims;
    vector<float> idle_character_dims;
    

    QPixmap idleFrame;
    
    int current_move_frame;
    int current_jump_frame;
    int frameCounter;
    int framesPerSprite;  // Updates before switching sprite
    int frame_per_sprite_jump;
    int frame_per_sprite_slide;
    int frame_per_sprite_attack;
    int total_jump_frames {5};
    std::string slide_dir;
    std::string sword_attack_dir = "right";
    
    float mass {2.0};
    float max_hp {100.0};
    float jump_height {50.0};
    float speed_move;
    float speed_run;
    float speed_jump {2500.0};  // Initial upward velocity for jumps
    float slide_dist {4.0};
    float sword_attack_dist {1.0};
    float total_jump_time {0.5};
    float total_slide_time {0.5};
    float total_sword_attack_time {0.5};
    float total_lower_time {0.1};
    float time_between_slides {1.0};
    float time_between_sword_attacks {1.0};
    float lower_time {0.0};
    float jump_time;
    float slide_time;
    float sword_attack_time;
    float walk_step_time {10000.0};
    float total_walk_step_time {0.5};  // Total duration for one walk cycle
    float time_between_frames;
    float c_scale = 100.0;
    bool draw_baxes = true;
    float base_y {0.0};  // Base Y position when jump starts
    float hp {100.0};
    float sword_attack_damages {10.0};
};

#endif // CHARACTER_H
