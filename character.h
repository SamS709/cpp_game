#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <cmath>
#include <vector>
#include "assets.h"
# include <QDebug>
using namespace std;

class Character: public MovableAsset
{
public:
    Character(Vec2 pos_, double time_betwwen_frames_, double mass);
    
    void loadSpriteFrames(const QString &basePath);
    void load_jump_frames(const QString &basePath);
    void load_move_frames(const QString &basePath);
    void load_slide_frames(const QString &basePath);
    void load_lower_frames(const QString &basePath);
    void load_sword_attack_frames(const QString &basePath);


    void update();
    void update_jump();
    void update_move();
    void update_slide();
    void update_lower();
    void update_sword_attack();

    void draw(QPainter &painter);    
    void set_x(double x_) {pos.x = x_;}
    void set_y(double y_) {pos.y = y_;}
    void set_x_expected(double x_expected_) { pos_exp.x = x_expected_ ; }
    void set_y_expected(double y_expected_) { pos_exp.y = y_expected_; }
    void update_vel(double dt){ v.x = (get_x_expected() - get_x()) / dt; v.y = (get_y_expected() - get_y()) / dt; }
    void update_expected_pos(double dt){ pos_exp.x = get_x() + v.x * dt; pos_exp.y = get_y() + v.y * dt; }
    void update_pos(){ pos.x = pos_exp.x; pos.y = pos_exp.y; }
    void update_expected_pos_collision(double delta_x, double delta_y){ pos_exp.x += delta_x; pos_exp.y += delta_y; }
    void set_c_scale(double s) {c_scale = s;}
    void set_hitbox(QPixmap& sprite, vector<double>& dims_);
    void set_draw_baxes(bool d) { draw_baxes = d; }
    void set_right(bool r);
    void set_right_pressed(bool r) {right_pressed = r;}
    void set_left(bool l);
    void set_left_pressed(bool l) {left_pressed = l;}
    void set_jumping(bool j);
    void set_sliding(bool s);
    void set_lowering(bool s);
    void set_sword_attacking(bool a);
    void set_speed_move(double s) {speed_move = s;}
    void set_speed_jump(double s) {speed_jump = s;}
    void set_speed_run(double s) {speed_run = s;}

    double get_rest() const override { return 0.0; } // No bounce for characters

    // Drawing positions (with sprite offset)
    double get_y() { return pos.y;} //- current_dims[1] - current_dims[3]; }
    double get_x() { return pos.x;} //- current_dims[0] - current_dims[2]; }
    double get_x_expected() const { return pos_exp.x;} //- current_dims[0] - current_dims[2]; }
    double get_y_expected() const { return pos_exp.y;} //- current_dims[1] - current_dims[3]; }
    
    // Center positions for collision detection (without sprite offset)
    // double get_center_x() const { return pos.x; }
    // double get_center_y() const { return pos.y; }
    // double get_center_x_expected() const { return pos_exp.x; }
    // double get_center_y_expected() const { return pos_exp.y; }

    void set_moving(bool m);

    double get_mass() const { return mass; } 
    double get_y_jump(double t); 
    double get_x_sliding(double t);
    double get_x_sword_attacking(double t);
    double get_c_scale() const { return c_scale; }



    bool get_moving();
    bool get_jumping();
    bool get_sliding();
    bool get_sword_attacking();
    bool get_right() { return facingRight; }
    bool get_right_pressed() { return right_pressed; }  // Check if right key is pressed
    bool get_left_pressed() { return left_pressed; }    // Check if left key is pressed


    void handle_rotate(QPainter &painte);
    
    void checkBounds(int windowWidth);

    QRectF get_full_hitbox();
    
private:
    double x;
    double y;
    bool moving = false;
    bool jumping = false;
    bool sliding = false;
    bool lowering = false;
    bool sword_attacking = false;
    bool right;
    bool left;
    bool left_pressed;
    bool right_pressed;
    bool facingRight;
    
    // Sprite animation
    QVector<QPixmap> move_frames;
    vector<vector<double>> move_frames_dims;
    vector<vector<double>> jump_frames_dims;
    vector<vector<double>> lower_frames_dims;
    QVector<QPixmap> jump_frames;
    QVector<QPixmap> slide_frames;
    QVector<QPixmap> lower_frames;
    QVector<QPixmap> sword_attack_frames;
    QVector<double> bounds;
    QPixmap *currentSprite = nullptr;
    vector<double> current_dims;
    vector<double> idle_dims;

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
    
    double mass {2.0};
    double jump_height {50.0};
    double speed_move;
    double speed_run;
    double speed_jump {2500.0};  // Initial upward velocity for jumps
    double slide_dist {4.0};
    double sword_attack_dist {1.0};
    double total_jump_time {0.5};
    double total_slide_time {0.5};
    double total_sword_attack_time {0.5};
    double time_between_slides {1.0};
    double time_between_sword_attacks {1.0};
    double lower_time {1.0};
    double jump_time;
    double slide_time;
    double sword_attack_time;
    double walk_step_time {10000.0};
    double total_walk_step_time {0.5};  // Total duration for one walk cycle
    double time_between_frames;
    double c_scale = 72.0;
    bool draw_baxes = true;
    double base_y {0.0};  // Base Y position when jump starts
};

#endif // CHARACTER_H
