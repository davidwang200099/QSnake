#include "wall.h"
#include <cstdlib>
#include <QPainter>

Wall::Wall()
{
    setData(GD_Type,WALL);
    setPosition();
}

void Wall::setPosition(){
    px=(rand()%(HALF_PICTURE_WIDTH/5)-HALF_PICTURE_WIDTH/10)*10;
    py=(rand()%(HALF_PICTURE_HEIGHT/5)-HALF_PICTURE_HEIGHT/10)*10;
    flag=Flag(rand()%2);
}

QRectF Wall::boundingRect() const{
    return flag?
    QRectF(px+0.5,py+0.5,DEFAULT_WALL_HEIGHT-1,DEFAULT_WALL_WIDTH-1):
    QRectF(px+0.5,py+0.5,DEFAULT_WALL_WIDTH-1,DEFAULT_WALL_HEIGHT-1);
}

void Wall::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::darkRed);
    painter->drawRect(boundingRect());
    painter->restore();
}
