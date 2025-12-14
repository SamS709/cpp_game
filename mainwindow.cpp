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
    character2 = new Character(400, 300, time_between_frames);
    env = new Env(character1, character2, time_between_frames);
        
    // Setup animation timer
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    animationTimer->start(time_between_frames); 
}

MainWindow::~MainWindow()
{
    delete character1;
    delete character2;
    delete env;
    delete ui;
}

void MainWindow::updateAnimation()
{
    env->update(width());
    update(); // Trigger repaint
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    painter.setRenderHint(QPainter::Antialiasing);
    // Draw background
    painter.fillRect(rect(), QColor(200, 230, 255));
    env->paint(&painter, width(), height());
    
}

void MainWindow::keyPressEvent(QKeyEvent *event)

{
    env->keyPressEvent(event);
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    env->keyReleaseEvent(event);
    
    QMainWindow::keyReleaseEvent(event);
}
