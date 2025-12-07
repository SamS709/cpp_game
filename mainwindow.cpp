#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QKeyEvent>


// cmake --build build/Desktop_Qt_6_10_0-Debug && ./build/Desktop_Qt_6_10_0-Debug/Stickman


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Stickman Walking Animation");
    resize(800, 600);
    double time_between_frames = 16.0;
    
    // Create character
    character1 = new Character(400, 300, time_between_frames);
    
    // Load sprite frames
    character1->loadSpriteFrames("/home/sami/dev/cours/cpp/Stickman/resources/images/characters/redhat");
    
    // Setup animation timer
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    animationTimer->start(time_between_frames); 
}

MainWindow::~MainWindow()
{
    delete character1;
    delete ui;
}

void MainWindow::updateAnimation()
{
    character1->update();
    character1->checkBounds(width());
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
    painter.drawRect(0, character1->getY() + 50, width(), height() - character1->getY() - 50);
    
    character1->draw(painter);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        if (!character1->get_jumping()) {
            character1->set_jumping(true);
            character1->set_moving(false);
        }
    }
    if (event->key() == Qt::Key_Right) {
        character1->set_right(true);
        character1->set_left(false);
        if(character1->get_jumping()){
            character1->set_moving(false);
        } else {
            character1->set_moving(true);
        }
    } if (event->key() == Qt::Key_Left) {
        character1->set_left(true);
        character1->set_right(false);

        if(character1->get_jumping()){
            character1->set_moving(false);
        } else {
            character1->set_moving(true);
        }
    } 
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        character1->set_moving(false);
    } else if (event->key() == Qt::Key_Left) {
        character1->set_moving(false);
    }
    QMainWindow::keyReleaseEvent(event);
}
