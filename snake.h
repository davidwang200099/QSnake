#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>
#include "constant.h"
#include <qiterator.h>
class Gamecontroller;
class QLCDNumber;

class Snake:public QGraphicsItem
{
    friend class Gamecontroller;
public:
    typedef enum {NoMove,MoveLeft='a',MoveRight='d',MoveUp='w',MoveDown='s'} Direction;
    typedef enum {NOEFFECT,SLOWDOWN,SPEEDUP,THROUGHWALL} Specialeffect;

    Snake(int &mark,QLCDNumber &monitor,Gamecontroller &ctrl,int x=0,int y=0,QColor color=DEFAULT_COLOR,int le=DEFAULT_LENGTH);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void advance(int phase);
    QPainterPath shape() const;
    void handleCollision();
    void setMoveDirection(Direction direction);
    void reshape();
    void ResetColor(){ColorOfSnake=DEFAULT_COLOR;effect=NOEFFECT;}
    void moveBackward();
    void randommove(QPoint newPos);
    void autoforage();
    Gamecontroller &ctrl;
    QList<QPoint> body;
    Direction moveDirection;
    int growing;
    int LengthOfSnake;
    int &score;
    QLCDNumber &monitor;
    QPoint PosOfSnake;
    QColor ColorOfSnake;
    Specialeffect effect;
    bool immortal;
    bool alive;
};

#endif // SNAKE_H
