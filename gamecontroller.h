#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <QObject>
#include <QTimer>
#include <QLCDNumber>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QLineEdit>
#include <QDateTime>
#include <QString>
#include <QDialog>
#include <vector>
#include <fstream>
#include <iostream>
#include "snake.h"
#include "food.h"
#include "wall.h"

using namespace std;
const int period[]={50,100,200};


class Gamecontroller:public QObject
{
    Q_OBJECT
    friend class Snake;
public:
    Gamecontroller(Mode mode,QGraphicsScene &scene,QObject *parent=NULL);
    ~Gamecontroller(){}

    void initSingle();
    void initDouble();
    void initAuto();

    void handlekeyPress(QKeyEvent *event);
    void SnakeAteFood(Snake *snake,Food *food);
    void initNewFood();
    void initReward();
    void removeReward();
    void gameover(Snake *snake);

    void resetSnakeAColor();
    void resetSnakeBColor();
    void resetSnakeASpeed();
    void resetSnakeBSpeed();
    void SnakeAAdvance(){snakeA->advance(0);snakeA->advance(1);}
    void SnakeBAdvance(){snakeB->advance(0);snakeB->advance(1);}
    void countdown();
    void switchimmortalMode();
    //void autoRedirect();
    bool eventFilter(QObject *object, QEvent *event);

    void stopGame();
    void restartGame();
    void newGame();
    void playBGM();
    void askforName(QDialog *dialog);
    void getPlayerName();
    void endGame(){exit(0);}
    //void initAutoForage();
    //void autoForage();
    Mode mode;
    bool rewardremoved;
    bool immortal;
    int mark[2];
    int rewardtime;

    Snake *snakeA;
    Snake *snakeB;
    Food *food;
    Food *rewardfood;
    Wall* wall;
    QGraphicsScene &scene;

    std::vector<Wall *> walls;
    QTimer timer[2];
    QTimer specialtimer[2];
    QTimer rewardtimer;
    QTimer gametimer;

    QLabel* labelA;
    QLabel* labelB;
    QLCDNumber* markmonitorA;
    QLCDNumber* markmonitorB;
    QLCDNumber* rewardmonitor;
    QMediaPlayer *player;
    QMediaPlaylist list;
    QDialog *dialog;
    QLineEdit *namerecorder;
};

#endif // GAMECONTROLLER_H
