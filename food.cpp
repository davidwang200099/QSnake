#include <QPainter>

#include "food.h"

Food::Food(ObjectType t)
{
        FunctionOfFood=(t==REWARD)?Function(rand()%FUNCTION_TYPE):NOFUNCTION;
        ColorOfFood=color[FunctionOfFood];
        setData(GD_Type,t);
        setPos(px=(rand()%60-30)*10,py=(rand()%40-20)*10);
}

QRectF Food::boundingRect() const{
    return QRectF(-SNAKE_SIZE,    -SNAKE_SIZE,
                   SNAKE_SIZE * 2, SNAKE_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillPath(shape(),ColorOfFood);
        painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(SNAKE_SIZE / 2, SNAKE_SIZE / 2), FOOD_RADIUS, FOOD_RADIUS);
    return p;
}
