#include "wall.h"
#include <cstdlib>
#include <QPainter>

Wall::Wall()
{   //int a;
    setData(GD_Type,WALL);
    setPosition();
    //flag=(a=rand()-1)%2?VERTICAL:HORIZONTAL;
}

void Wall::setPosition(){
    px=(rand()%(HALF_PICTURE_WIDTH/5)-HALF_PICTURE_WIDTH/10)*10;
    py=(rand()%(HALF_PICTURE_HEIGHT/5)-HALF_PICTURE_HEIGHT/10)*10;
    flag=Flag(rand()%2);
}

QRectF Wall::boundingRect() const{
    //qreal x=10;
    return flag?
    QRectF(px+0.5,py+0.5,DEFAULT_WALL_HEIGHT-1,DEFAULT_WALL_WIDTH-1):
    QRectF(px+0.5,py+0.5,DEFAULT_WALL_WIDTH-1,DEFAULT_WALL_HEIGHT-1);
}

/*QPainterPath Wall::shape() const{
    QPainterPath p;
    p.addRect(px+0.5,px+0.5,DEFAULT_WALL_HEIGHT-0.5,DEFAULT_WALL_WIDTH-0.5);
    return p;
}*/

void Wall::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::darkRed);
    //painter->fillPath(shape(),Qt::darkRed);
    painter->drawRect(boundingRect());
    painter->restore();
}
