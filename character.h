#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <cmath>

class Character
{
public:
    Character(double startX, double startY, double time_betwwen_frames_);
    
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
    void getX(double x_) { x = x_; }
    void getY(double y_) { y = y_; }
    void get_mass(double mass_) { mass = mass_; } 
    void set_right(bool r);
    void set_left(bool l);
    void set_jumping(bool j);
    void set_sliding(bool s);
    void set_lowering(bool s);
    void set_sword_attacking(bool a);


    void set_moving(bool m);

    double getX() const { return x; }
    double getY() const { return y; }
    double get_mass() const { return mass; } 
    double get_y_jump(double t); 
    double get_x_sliding(double t);
    double get_x_sword_attacking(double t);


    bool get_moving();
    bool get_jumping();
    bool get_sliding();
    bool get_sword_attacking();


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
    bool facingRight;
    
    // Sprite animation
    QVector<QPixmap> move_frames;
    QVector<QPixmap> jump_frames;
    QVector<QPixmap> slide_frames;
    QVector<QPixmap> lower_frames;
    QVector<QPixmap> sword_attack_frames;
    QVector<double> bounds;
    QPixmap *currentSprite = nullptr;

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
    double jump_height {2.0};
    double slide_dist {4.0};
    double sword_attack_dist {1.0};
    double total_jump_time {0.5};
    double total_slide_time {0.5};
    double total_sword_attack_time {0.5};
    double time_between_slides {1.0};
    double time_between_sword_attacks {1.0};
    double jump_time;
    double slide_time;
    double sword_attack_time;
    double walk_step_time {0.0};
    double total_walk_step_time {0.5};  // Total duration for one walk cycle
    double time_between_frames;
};

#endif // CHARACTER_H
