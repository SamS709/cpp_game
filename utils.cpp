#include "utils.h"

SpriteLoader::SpriteLoader(float scale, bool draw_boxes)
    : c_scale(scale)
    , draw_boxes(draw_boxes)
{
}

bool SpriteLoader::loadSequence(
    const QString& basePath,
    const QString& sequenceName,
    int startFrame,
    int endFrame,
    QVector<QPixmap>& sprites,
    vector<vector<float>>& asset_dims,
    vector<vector<float>>& character_dims,
    vector<vector<float>>* sword_dims)
{
    sprites.clear();
    asset_dims.clear();
    character_dims.clear();
    if (sword_dims) sword_dims->clear();
    
    bool anyLoaded = false;
    
    for (int i = startFrame; i <= endFrame; ++i) {
        QString framePath = basePath + QString("/%1/%2_%3.png")
            .arg(sequenceName)
            .arg(sequenceName)
            .arg(i, 3, 10, QChar('0'));
        
        QPixmap sprite(framePath);
        
        if (!sprite.isNull()) {
            sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
            set_hitbox(sprite, &asset_dims, &character_dims, sword_dims);
            sprites.append(sprite);
            anyLoaded = true;
            qDebug() << "Loaded:" << framePath;
        } else {
            qDebug() << "Failed to load:" << framePath;
        }
    }
    
    return anyLoaded;
}

bool SpriteLoader::loadSprite(
    const QString& filePath,
    QPixmap& sprite,
    vector<float>& asset_dim,
    vector<float>& character_dim,
    vector<float>* sword_dim)
{
    sprite = QPixmap(filePath);
    
    if (sprite.isNull()) {
        qDebug() << "Failed to load:" << filePath;
        return false;
    }
    
    sprite = sprite.scaledToHeight(c_scale, Qt::SmoothTransformation);
    
    vector<vector<float>> asset_dims_temp;
    vector<vector<float>> character_dims_temp;
    vector<vector<float>> sword_dims_temp;
    
    set_hitbox(sprite, &asset_dims_temp, &character_dims_temp, sword_dim ? &sword_dims_temp : nullptr);
    
    if (!asset_dims_temp.empty()) asset_dim = asset_dims_temp[0];
    if (!character_dims_temp.empty()) character_dim = character_dims_temp[0];
    if (sword_dim && !sword_dims_temp.empty()) *sword_dim = sword_dims_temp[0];
    
    qDebug() << "Loaded:" << filePath;
    return true;
}

void SpriteLoader::set_hitbox(
    QPixmap& sprite,
    vector<vector<float>>* asset_dims_,
    vector<vector<float>>* character_dims_,
    vector<vector<float>>* sword_dims_)
{
    QImage image = sprite.toImage();
    
    if (image.isNull() || !asset_dims_ || !character_dims_) {
        return;
    }
    
    int width = image.width();
    int height = image.height();
    
    // Find left bounds
    int leftBoundAsset = width;
    int leftBoundCharacter = width;
    int leftBoundSword = width;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QRgb pixel = image.pixel(x, y);
            
            if (leftBoundAsset == width && isAssetPixel(pixel)) {
                leftBoundAsset = x;
            }
            if (leftBoundCharacter == width && isCharacterPixel(pixel)) {
                leftBoundCharacter = x;
            }
            if (leftBoundSword == width && isSwordPixel(pixel)) {
                leftBoundSword = x;
            }
        }
    }
    
    // Find right bounds
    int rightBoundAsset = 0;
    int rightBoundCharacter = 0;
    int rightBoundSword = 0;
    for (int x = width - 1; x >= 0; x--) {
        for (int y = 0; y < height; y++) {
            QRgb pixel = image.pixel(x, y);
            if (rightBoundAsset == 0 && isAssetPixel(pixel)) {
                rightBoundAsset = x;
            }
            if (rightBoundCharacter == 0 && isCharacterPixel(pixel)) {
                rightBoundCharacter = x;
            }
            if (rightBoundSword == 0 && isSwordPixel(pixel)) {
                rightBoundSword = x;
            }
        }
    }
    
    // Find top bounds
    int topBoundAsset = height;
    int topBoundCharacter = height;
    int topBoundSword = height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            if (topBoundAsset == height && isAssetPixel(pixel)) {
                topBoundAsset = y;
            }
            if (topBoundCharacter == height && isCharacterPixel(pixel)) {
                topBoundCharacter = y;
            }
            if (topBoundSword == height && isSwordPixel(pixel)) {
                topBoundSword = y;
            }
        }
    }
    
    // Find bottom bounds
    int bottomBoundAsset = 0;
    int bottomBoundCharacter = 0;
    int bottomBoundSword = 0;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            if (bottomBoundAsset == 0 && isAssetPixel(pixel)) {
                bottomBoundAsset = y;
            }
            if (bottomBoundCharacter == 0 && isCharacterPixel(pixel)) {
                bottomBoundCharacter = y;
            }
            if (bottomBoundSword == 0 && isSwordPixel(pixel)) {
                bottomBoundSword = y;
            }
        }
    }
    
    // Calculate dimensions
    float widthAsset = rightBoundAsset - leftBoundAsset;
    float heightAsset = bottomBoundAsset - topBoundAsset;
    float widthCharacter = rightBoundCharacter - leftBoundCharacter;
    float heightCharacter = bottomBoundCharacter - topBoundCharacter;
    float widthSword = rightBoundSword - leftBoundSword;
    float heightSword = bottomBoundSword - topBoundSword;
    
    if (leftBoundSword == width && rightBoundSword == 0) {
        leftBoundSword = 0;
        rightBoundSword = 0;
        widthSword = 0;
        heightSword = 0;
    }
    
    // Store dimensions
    asset_dims_->push_back({
        static_cast<float>(leftBoundAsset),
        static_cast<float>(topBoundAsset),
        widthAsset / 2.0f,
        heightAsset / 2.0f
    });
    
    character_dims_->push_back({
        static_cast<float>(leftBoundCharacter),
        static_cast<float>(topBoundCharacter),
        widthCharacter / 2.0f,
        heightCharacter / 2.0f
    });
    
    if (sword_dims_) {
        sword_dims_->push_back({
            static_cast<float>(leftBoundSword),
            static_cast<float>(topBoundSword),
            widthSword / 2.0f,
            heightSword / 2.0f
        });
    }
    
    // Draw bounding boxes if enabled
    if (draw_boxes) {
        QPainter painter(&sprite);
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(leftBoundAsset, topBoundAsset, widthAsset, heightAsset);
        painter.setPen(QPen(Qt::green, 2));
        painter.drawRect(leftBoundCharacter, topBoundCharacter, widthCharacter, heightCharacter);
        if (sword_dims_) {
            painter.setPen(QPen(Qt::blue, 2));
            painter.drawRect(leftBoundSword, topBoundSword, widthSword, heightSword);
        }
    }
    
    qDebug() << "Hitbox - Asset:" << leftBoundAsset << rightBoundAsset << widthAsset << heightAsset
             << "Character:" << leftBoundCharacter << rightBoundCharacter << widthCharacter << heightCharacter;
}

bool SpriteLoader::isAssetPixel(QRgb pixel) {
    return (qAlpha(pixel) > 10);
}

bool SpriteLoader::isCharacterPixel(QRgb pixel) {
    if (qAlpha(pixel) <= 10) return false;
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);
    return (r < 5 && g < 5 && b < 5);
}

bool SpriteLoader::isSwordPixel(QRgb pixel) {
    if (qAlpha(pixel) <= 10) return false;
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);
    return (130 < r && r < 140 && 130 < g && g < 140 && 130 < b && b < 140);
}


void VisualContainer::load_sprites(){
    bomb_sprites.clear();
    vector<vector<float>> asset_dims, character_dims;
    loadSequence("resources/images/characters/redhat", "bomb", 1, 15, 
                       bomb_sprites, asset_dims, character_dims);
    projectile_sprites.clear();
    asset_dims.clear();
    character_dims.clear();
    loadSequence("resources/images/characters/redhat", "bomb", 1, 15, 
                       projectile_sprites, asset_dims, character_dims);
    
}