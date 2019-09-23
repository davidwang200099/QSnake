#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>
#include "constant.h"
class Wall:public QGraphicsItem
{
    friend class Gamecontroller;
public:
    typedef enum {HORIZONTAL,VERTICAL} Flag;
    Wall();
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);
    //QPainterPath shape() const;
    void setx(int x){px=x;}
    void sety(int y){py=y;}
    void setxy(int x,int y){px=x;py=y;}
    void setflag(Flag f){flag=f;}
    void setPosition();
public:

   int px;
   int py;

    Flag flag;
};

#endif // WALL_H
