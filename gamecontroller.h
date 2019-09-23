#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <QObject>
#include <QTimer>
#include <QLCDNumber>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include "snake.h"
#include "food.h"
#include "wall.h"
#include <vector>
#include <QGraphicsView>
#include <QPushButton>
#include <QMainWindow>

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

    void HandlekeyPress(QKeyEvent *event);
    void SnakeAteFood(Snake *snake,Food *food);
    void InitNewFood();
    void InitReward();
    void RemoveReward(){
        if(!rewardremoved){
          disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
          rewardmonitor->display(0);
          scene.removeItem(rewardfood);
          delete rewardfood;
          rewardremoved=true;
        }
    }
    void gameover(Snake *snake);

    void ResetSnakeAColor(){
        snakeA->effect=Snake::NOEFFECT;
        disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
        rewardtime=REWARD_INTERVAL/1000;
        rewardmonitor->display(0);
        snakeA->ColorOfSnake=Qt::red;
    }
    void ResetSnakeBColor(){
        snakeB->effect=Snake::NOEFFECT;
        disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
        rewardtime=REWARD_INTERVAL/1000;
        rewardmonitor->display(0);
        snakeB->ColorOfSnake=Qt::green;
    }
    void ResetSnakeASpeed(){
        snakeA->effect=Snake::NOEFFECT;
        disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
        rewardmonitor->display(0);
        disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
        connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
        timer->start(DEFAULT_PERIOD);
        rewardtime=REWARD_INTERVAL/1000;
    }
    void ResetSnakeBSpeed(){
        snakeB->effect=Snake::NOEFFECT;
        disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
        rewardmonitor->display(0);
        disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
        connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
        timer[1].start(DEFAULT_PERIOD);
        rewardtime=REWARD_INTERVAL/1000;

    }
    void SnakeAAdvance(){snakeA->advance(0);snakeA->advance(1);}
    void SnakeBAdvance(){snakeB->advance(0);snakeB->advance(1);}
    void countdown(){
        rewardmonitor->display(rewardtime);
        if(rewardtime>0) rewardtime--;
        else rewardtime=REWARD_INTERVAL/1000;
    }
    void switchimmortalMode(){
        immortal=immortal?false:true;
        for(int i=0;i<walls.size();i++)
            if(walls[i]->data(GD_Type)==WALL)
                walls[i]->setData(GD_Type,DEFAULT);
            else walls[i]->setData(GD_Type,WALL);
    }
    //void autoRedirect();
    bool eventFilter(QObject *object, QEvent *event);

    void stopGame();
    void restartGame();
    void newGame(){
        disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
        if(mode==DOUBLE)
        disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
        stopGame();
        restartGame();
    }
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
    //QGraphicsView &view;

    std::vector<Wall *> walls;
    //std::vector<QPushButton *> vp;
    QTimer timer[2];
    QTimer specialtimer[2];
    QTimer rewardtimer;
    QTimer gametimer;

    //QLCDNumber timemonitor;
    QLabel* labelA;
    QLabel* labelB;
    QLCDNumber* markmonitorA;
    QLCDNumber* markmonitorB;
    QLCDNumber* rewardmonitor;

};

#endif // GAMECONTROLLER_H
