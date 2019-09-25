#ifndef FOOD_H
#define FOOD_H
#include <QGraphicsItem>
#include "constant.h"

const QColor color[]={Qt::green,Qt::gray,Qt::red,Qt::yellow,Qt::blue,Qt::magenta,Qt::cyan};

class Food:public QGraphicsItem
{
        typedef enum{NOFUNCTION,
                     TRANSPANRENT,
                     GETMORESCORE,
                     ACCELERATE,
                     SLOWDOWN,
                     BACKWARD,
                     RANDOMMOVE
        } Function;
        friend class Gamecontroller;
public:
    Function FunctionOfFood;
    QColor ColorOfFood;
    Food(ObjectType t=FOOD);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;
    int px;
    int py;
};

#endif // FOOD_H
