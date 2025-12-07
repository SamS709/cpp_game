#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Stickman Walking Animation");
    resize(800, 600);
    double time_between_frames = 16.0;
    
    // Create character
    character = new Character(400, 300, time_between_frames);
    
    // Load sprite frames
    character->loadSpriteFrames("/home/sami/dev/cours/cpp/Stickman/resources/images/characters/redhat");
    
    // Setup animation timer
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    animationTimer->start(time_between_frames); 
}

MainWindow::~MainWindow()
{
    delete character;
    delete ui;
}

void MainWindow::updateAnimation()
{
    character->update();
    character->checkBounds(width());
    update(); // Trigger repaint
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(rect(), QColor(200, 230, 255));
    
    // Draw ground
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(100, 200, 100));
    painter.drawRect(0, character->getY() + 50, width(), height() - character->getY() - 50);
    
    character->draw(painter);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        if (!character->get_jumping()) {
            character->set_jumping(true);
            character->set_moving(false);
        }
    }
    if (event->key() == Qt::Key_Right) {
        character->set_right(true);
        character->set_left(false);
        if(character->get_jumping()){
            character->set_moving(false);
        } else {
            character->set_moving(true);
        }
    } if (event->key() == Qt::Key_Left) {
        character->set_left(true);
        character->set_right(false);

        if(character->get_jumping()){
            character->set_moving(false);
        } else {
            character->set_moving(true);
        }
    } 
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        character->set_moving(false);
    } else if (event->key() == Qt::Key_Left) {
        character->set_moving(false);
    }
    QMainWindow::keyReleaseEvent(event);
}
