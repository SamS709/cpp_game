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
    void update();
    void draw(QPainter &painter);
    
    void set_right(bool r);
    void set_left(bool l);
    void set_jumping(bool j);
    void set_sliding(bool s);

    void set_moving(bool m);

    double getX() const { return x; }
    double getY() const { return y; }
    double get_y_jump(double t); 
    double get_x_sliding(double t);

    bool get_jumping();
    bool get_sliding();



    
    void checkBounds(int windowWidth);
    
private:
    double x;
    double y;
    bool moving;
    bool jumping;
    bool sliding;
    bool right;
    bool left;
    bool facingRight;
    
    // Sprite animation
    QVector<QPixmap> move_frames;
    QVector<QPixmap> jump_frames;
    QVector<QPixmap> slide_frames;
    QPixmap idleFrame;
    
    int current_move_frame;
    int current_jump_frame;
    int frameCounter;
    int framesPerSprite;  // Updates before switching sprite
    int frame_per_sprite_jump;
    int frame_per_sprite_slide;
    int total_jump_frames {5};
    double slide_dir;
    double jump_height {2.0};
    double slide_dist {4.0};
    double total_jump_time {0.5};
    double total_slide_time {0.5};
    double time_between_slides {1.0};
    double jump_time;
    double slide_time;
    double time_between_frames;
};

#endif // CHARACTER_H
