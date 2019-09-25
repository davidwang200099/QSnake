#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this))
{
    resize(800, 600);

    view->resize(HALF_PICTURE_WIDTH*2,HALF_PICTURE_HEIGHT*2);
    scene->setBackgroundBrush(QPixmap("D:/cppfiles/QTProjects/snake2/d.jpg"));

}

MainWindow::~MainWindow()
{
    
}

void MainWindow::initFunction(){//init the button for the special functions
    QPushButton *buttonA=new QPushButton("nightmode",this);
    buttonA->setGeometry(0,450,150,40);
    connect(buttonA,&QPushButton::clicked,this,&MainWindow::switchNightMode);

    QCheckBox *buttonB=new QCheckBox("immortalmode",this);
    buttonB->setGeometry(200,450,150,40);
    connect(buttonB,&QCheckBox::clicked,this->ctrl,&Gamecontroller::switchimmortalMode);

    QPushButton *buttonC=new QPushButton("restart",this);
    buttonC->setGeometry(400,450,150,40);
    connect(buttonC,&QPushButton::clicked,this->ctrl,&Gamecontroller::newGame);

    QPushButton *buttonD=new QPushButton("play BGM",this);
    buttonD->setGeometry(0,500,150,40);
    connect(buttonD,&QPushButton::clicked,this->ctrl,&Gamecontroller::playBGM);
}

void MainWindow::askUser(){
    QMessageBox message;
    message.setIcon(QMessageBox::Information);
    message.setWindowTitle("Welcome to QSnake!");
    message.setText("Please select a mode");
    QAbstractButton *S_Button=(QAbstractButton *)message.addButton("single",QMessageBox::YesRole);
    QAbstractButton *D_Button=(QAbstractButton *)message.addButton("double",QMessageBox::ActionRole);
    QAbstractButton *A_Button=(QAbstractButton *)message.addButton("autoforage",QMessageBox::HelpRole);
    switch(message.exec()){
        case 0:mode=SINGLE;break;
        case 1:mode=DOUBLE;break;
        case 2:mode=AUTO;break;
        default:exit(0);
    }
    initGamecontroller(mode);
    initMonitor();
    initFunction();
}

void MainWindow::initGamecontroller(Mode mode){
    ctrl=new Gamecontroller(mode,*scene);
}

void MainWindow::initMonitor(){
    switch (mode) {
        case SINGLE:
            ctrl->markmonitorA->setParent(this);
            ctrl->labelA->setGeometry(650,80,100,20);
            ctrl->labelA->setParent(this);
            ctrl->labelA->setText("snake");
            ctrl->markmonitorA->setGeometry(650,100,100,40);
            break;
        case DOUBLE:
            ctrl->markmonitorA->setParent(this);
            ctrl->markmonitorB->setParent(this);
            ctrl->markmonitorA->setGeometry(650,100,100,40);
            ctrl->markmonitorB->setGeometry(650,240,100,40);
            ctrl->labelA->setParent(this);
            ctrl->labelB->setParent(this);
            ctrl->labelA->setGeometry(650,80,100,20);
            ctrl->labelA->setText("snake A");
            ctrl->labelB->setGeometry(650,220,100,20);
            ctrl->labelB->setText("snake B");
    }
    ctrl->rewardmonitor->setParent(this);
    ctrl->rewardmonitor->setGeometry(650,380,100,40);
}
