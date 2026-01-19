#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QDir>
#include <vector>
#include <functional>

using namespace std;

class SpriteLoader {
public:
    SpriteLoader(float scale = 100.0f, bool draw_boxes = false, QColor color_ = QColor(0,0,0,0));
    
    // Load a sequence of sprites with automatic hitbox detection
    bool loadSequence(
        const QString& basePath,
        const QString& sequenceName,
        int startFrame,
        int endFrame,
        QVector<QPixmap>& sprites,
        vector<vector<float>>& asset_dims,
        vector<vector<float>>& character_dims,
        vector<vector<float>>* sword_dims = nullptr
    );
    
    // Load a single sprite with hitbox detection
    bool loadSprite(
        const QString& filePath,
        QPixmap& sprite,
        vector<float>& asset_dim,
        vector<float>& character_dim,
        vector<float>* sword_dim = nullptr
    );
    
    // Set hitbox detection for a sprite
    void set_hitbox(
        QPixmap& sprite,
        vector<vector<float>>* asset_dims,
        vector<vector<float>>* character_dims,
        vector<vector<float>>* sword_dims = nullptr
    );
    
    void setScale(float scale) { c_scale = scale; }
    void setDrawBoxes(bool draw) { draw_boxes = draw; }
    
private:
    float c_scale;
    bool draw_boxes;
    QColor color;
    
    // Helper functions for pixel detection
    bool isAssetPixel(QRgb pixel);
    bool isCharacterPixel(QRgb pixel);
    bool isSwordPixel(QRgb pixel);
};


class VisualContainer: public SpriteLoader{

public:

    VisualContainer(float scale = 100.0f, bool draw_boxes = false)
    : SpriteLoader(scale, draw_boxes ){
        load_sprites();
    };

    void load_sprites();

    QVector<QPixmap> bomb_sprites;
    QVector<QPixmap> projectile_sprites;
    QPixmap speed_sprite;
    QPixmap hp_sprite;


};

#endif // UTILS_H
