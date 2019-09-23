#include <QPainter>
#include "food.h"
#include "snake.h"
#include "constant.h"
#include <QKeyEvent>
#include "gamecontroller.h"
#include <QMessageBox>
Snake::Snake(int &mark,QLCDNumber &monitor,Gamecontroller &ctrl,int x,int y,QColor color,int le):ctrl(ctrl),
  growing(le),LengthOfSnake(le),ColorOfSnake(color),score(mark),monitor(monitor){
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFocus();
    LengthOfSnake=le;
    immortal=false;
    alive=true;
    PosOfSnake=QPoint(x,y);
    moveDirection=NoMove;
    effect=NOEFFECT;
    setData(GD_Type,SNAKE);
}

void Snake::reshape(){
    LengthOfSnake=DEFAULT_LENGTH;
    growing=DEFAULT_LENGTH;
    body.clear();
    ColorOfSnake=DEFAULT_COLOR;
    //while(body.size()!=1) body.pop_back();
    setPos(PosOfSnake=QPoint(100,0));
    setMoveDirection(Snake::NoMove);
}

QRectF Snake::boundingRect() const{
  int minX = PosOfSnake.x();
  int minY = PosOfSnake.y();
  int maxX = PosOfSnake.x();
  int maxY = PosOfSnake.y();

  foreach (QPoint p, body) {
      maxX = p.x() > maxX ? p.x() : maxX;
      maxY = p.y() > maxY ? p.y() : maxY;
      minX = p.x() < minX ? p.x() : minX;
      minY = p.y() < minY ? p.y() : minY;
  }

  QPointF tl = mapFromScene(QPointF(minX, minY));
  QPointF br = mapFromScene(QPointF(maxX, maxY));

  QRectF bound = QRectF(tl.x(),  // x
                        tl.y(),  // y
                        br.x() - tl.x() + SNAKE_SIZE,      // width
                        br.y() - tl.y() + SNAKE_SIZE      //height
                        );
  return bound;
}

QPainterPath Snake::shape() const{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QPointF j=mapFromScene(PosOfSnake);
    path.addEllipse(j.x(),j.y(),SNAKE_SIZE,SNAKE_SIZE);
    foreach(QPointF p,body){
        QPointF i=mapFromScene(p);
        path.addRect(i.x(),i.y(),SNAKE_SIZE,SNAKE_SIZE);
      }
    return path;
}



void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->fillPath(shape(), ColorOfSnake);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction){
    if (moveDirection == MoveLeft && direction == MoveRight)
        return;
    if (moveDirection == MoveRight && direction == MoveLeft)
        return;
    if (moveDirection == MoveUp && direction == MoveDown)
        return;
    if (moveDirection == MoveDown && direction == MoveUp)
        return;
    moveDirection = direction;
}

void Snake::advance(int phase){
    if(!phase) return;
    if(moveDirection==NoMove) return;

    if(growing>0){
        QPoint tailPoint = PosOfSnake;
        body << tailPoint;
        growing -= 1;
        LengthOfSnake++;
    }else {
        if(!body.isEmpty()) body.removeFirst();
        body << PosOfSnake;
    }

        switch (moveDirection) {
        case MoveLeft:
            PosOfSnake.rx() -= SNAKE_SIZE;
            if(PosOfSnake.rx()<X_MIN) PosOfSnake.rx()=X_MAX;
            if(ctrl.mode==AUTO)
              if(ctrl.food->px==PosOfSnake.x())
                setMoveDirection(ctrl.food->px<PosOfSnake.y()?Snake::MoveUp:Snake::MoveDown);

            break;
        case MoveRight:
            PosOfSnake.rx() += SNAKE_SIZE;
            if(PosOfSnake.rx()>X_MAX) PosOfSnake.rx()=X_MIN;
           if(ctrl.mode==AUTO)
              if(ctrl.food->px==PosOfSnake.x())
                setMoveDirection(ctrl.food->px<PosOfSnake.y()?Snake::MoveUp:Snake::MoveDown);
            break;
        case MoveUp:
            PosOfSnake.ry() -= SNAKE_SIZE;
            if(PosOfSnake.ry()<Y_MIN) PosOfSnake.ry()=Y_MAX;
            if(ctrl.mode==AUTO)
              if(ctrl.food->py==PosOfSnake.y())
                setMoveDirection(ctrl.food->px<PosOfSnake.x()?Snake::MoveLeft:Snake::MoveRight);
            break;
        case MoveDown:
            PosOfSnake.ry() += SNAKE_SIZE;
            if(PosOfSnake.ry()>Y_MAX) PosOfSnake.ry()=Y_MIN;
            if(ctrl.mode==AUTO)
              if(ctrl.food->py==PosOfSnake.y())
                setMoveDirection(ctrl.food->px<PosOfSnake.x()?Snake::MoveLeft:Snake::MoveRight);
            break;
    }
    setPos(PosOfSnake);
    handleCollision();
}


void Snake::handleCollision(){
    QList<QGraphicsItem*> l=collidingItems();
    foreach(QGraphicsItem *item,l){
        if(item->data(GD_Type)==FOOD||item->data(GD_Type)==REWARD)
          ctrl.SnakeAteFood(this,(Food *)item);
        else if(ColorOfSnake!=Qt::gray&&item->data(GD_Type)==WALL)
            ctrl.gameover(this);
        }
    if(moveDirection!=NoMove&&body.contains(PosOfSnake)&&ColorOfSnake!=Qt::gray)
        ctrl.gameover(this);
}
