#include "gamecontroller.h"

#include <QMessageBox>
#include <QGraphicsScene>

Gamecontroller::Gamecontroller(Mode mode,QGraphicsScene &scene,QObject *parent)
    :scene(scene),QObject(parent),mode(mode){
    markmonitorA=new QLCDNumber;
    markmonitorB=new QLCDNumber;
    labelA=new QLabel;
    labelB=new QLabel;
    rewardmonitor=new QLCDNumber;
    immortal=false;
    switch(mode){
        case SINGLE:initSingle();break;
        case DOUBLE:initDouble();break;
        case AUTO:initAuto();break;
    }  
}

void Gamecontroller::initSingle(){
    rewardfood=NULL;
    rewardremoved=true;
    rewardtime=REWARD_INTERVAL/1000;
    mark[0]=mark[1]=0;
    scene.installEventFilter(this);

    scene.setSceneRect(-HALF_PICTURE_WIDTH,-HALF_PICTURE_HEIGHT, 600, 400);
    scene.installEventFilter(this);

    snakeA=new Snake(mark[0],*markmonitorA,*this,100,0);
    snakeB=new Snake(mark[1],*markmonitorB,*this,INT_MIN,0,Qt::green);
    scene.addItem(snakeA);

    for(int i=0;i<DEFAULT_WALL_COUNT;i++) {
        wall=new Wall;
        if(immortal) wall->setData(GD_Type,DEFAULT);
        while(wall->boundingRect().contains(QPointF(100,0))||
              wall->boundingRect().contains(QPointF(-100,0))||
              wall->px+DEFAULT_WALL_WIDTH>X_MAX||
              wall->py+DEFAULT_WALL_WIDTH>Y_MAX
            )
            wall->setPosition();
        scene.addItem(wall);
        walls.push_back(wall);
    }

test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    for(int i=0;i<walls.size();i++){
      if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
          {delete food;goto test;}
      }
      scene.addItem(food);


    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);

    gametimer.start(1000);

}

void Gamecontroller::initDouble(){
    rewardfood=NULL;
    rewardremoved=true;
    rewardtime=REWARD_INTERVAL/1000;
    mark[0]=mark[1]=0;
    scene.setSceneRect(-HALF_PICTURE_WIDTH,-HALF_PICTURE_HEIGHT, 600, 400);
    scene.installEventFilter(this);

    snakeA=new Snake(mark[0],*markmonitorA,*this,100,0);
    snakeB=new Snake(mark[1],*markmonitorB,*this,-100,0,Qt::green);
    scene.addItem(snakeA);
    scene.addItem(snakeB);

    for(int i=0;i<DEFAULT_WALL_COUNT;i++) {  
        wall=new Wall;
        if(immortal) wall->setData(GD_Type,DEFAULT);
        while(wall->boundingRect().contains(QPointF(100,0))||
              wall->boundingRect().contains(QPointF(-100,0))||
              wall->px+DEFAULT_WALL_WIDTH>X_MAX||
              wall->py+DEFAULT_WALL_WIDTH>Y_MAX
             )
            wall->setPosition();
        scene.addItem(wall);
        walls.push_back(wall);
    }

test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    if(snakeB->shape().contains(snakeB->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
  for(int i=0;i<walls.size();i++){
    if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    }
    scene.addItem(food);

    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);
    connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
    timer[1].start(DEFAULT_PERIOD);

    gametimer.start(1000);
}

void Gamecontroller::initAuto(){
    rewardfood=NULL;
    rewardremoved=true;
    rewardtime=REWARD_INTERVAL/1000;
    mark[0]=mark[1]=0;
    scene.setSceneRect(-HALF_PICTURE_WIDTH,-HALF_PICTURE_HEIGHT, 600, 400);

    snakeA=new Snake(mark[0],*markmonitorA,*this,100,0);
    snakeB=new Snake(mark[1],*markmonitorB,*this,INT_MIN,0,Qt::green);
    scene.addItem(snakeA);
    snakeA->setMoveDirection(Snake::MoveUp);
test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    scene.addItem(food);

    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);

    gametimer.start(1000);
}

void Gamecontroller::SnakeAteFood(Snake *snake,Food *food){
  if(food->data(GD_Type)==REWARD)
    disconnect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::RemoveReward);
  scene.removeItem(food);

  switch(food->FunctionOfFood){
    case Food::NOFUNCTION:
          snake->growing++;snake->score++;snake->monitor.display(snake->score);
          break;
    case Food::GETMORESCORE:
         snake->growing+=2;snake->score+=2;snake->monitor.display(snake->score);
         break;
    case Food::TRANSPANRENT:
      snake->effect=Snake::THROUGHWALL;
      snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
      snake->ColorOfSnake=Qt::gray;
      if(snake==this->snakeA){
      connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::ResetSnakeAColor);
      connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
      specialtimer->start(SPECIAL_INTERVAL);
      }else {
       connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::ResetSnakeBColor);
       specialtimer[1].start(SPECIAL_INTERVAL);
       connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
       }
      break;
    case Food::ACCELERATE:
      snake->effect=Snake::SPEEDUP;
      snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
      if(snake==this->snakeA){
      disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
      connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
      timer->start(DEFAULT_PERIOD/2);
      connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::ResetSnakeASpeed);
      connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
      specialtimer->start(SPECIAL_INTERVAL);
      }else {
          disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
          connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
          timer[1].start(DEFAULT_PERIOD/2);
          connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::ResetSnakeBSpeed);
          connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
          specialtimer[1].start(SPECIAL_INTERVAL);
       }
      break;
    case Food::SLOWDOWN:
          snake->effect=Snake::SLOWDOWN;
          snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
          if(snake==this->snakeA){
          disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
          connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
          timer->start(DEFAULT_PERIOD*2);
          connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::ResetSnakeASpeed);
          connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
          specialtimer->start(SPECIAL_INTERVAL);
          }else {
              disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
              connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
              timer[1].start(DEFAULT_PERIOD*2);
              connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::ResetSnakeBSpeed);
              connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
              specialtimer[1].start(SPECIAL_INTERVAL);
           }
      break;
      default:snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
  }
  if(food->data(GD_Type)==FOOD){
    delete food;
    InitNewFood();
    }
  else {
    delete food;
    rewardremoved=true;
    //food=0;
    }
}

void Gamecontroller::stopGame(){

    disconnect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::RemoveReward);
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);

    switch(snakeA->effect){
      case Snake::SLOWDOWN:case Snake::SPEEDUP:
disconnect(specialtimer,&QTimer::timeout,this,&Gamecontroller::ResetSnakeASpeed);
      break;
      case Snake::THROUGHWALL:
disconnect(specialtimer,&QTimer::timeout,this,&Gamecontroller::ResetSnakeAColor);
    }
    snakeA->effect=Snake::NOEFFECT;

  if(mode==DOUBLE)
    switch(snakeB->effect){
      case Snake::SLOWDOWN:case Snake::SPEEDUP:
disconnect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::ResetSnakeBSpeed);
      break;
      case Snake::THROUGHWALL:
disconnect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::ResetSnakeBColor);
    }
  snakeB->effect=Snake::NOEFFECT;

}

void Gamecontroller::restartGame(){
    //scene.removeItem(snakeA);
    snakeA->alive=true;
    snakeA->reshape();
    if(!snakeA->scene()) scene.addItem(snakeA);
    if(mode==DOUBLE){
        //if(snakeA->scene()) scene.removeItem(snakeA);
        //if(snakeB->scene()) scene.removeItem(snakeB);
        snakeB->alive=true;
        snakeB->reshape();
        snakeB->ColorOfSnake=Qt::green;
        snakeB->setPos(snakeB->PosOfSnake=QPoint(-100,0));
        if(!snakeB->scene()) scene.addItem(snakeB);
    }
    scene.removeItem(food);delete food;
    if(!rewardremoved) {
        rewardremoved=true;
        scene.removeItem(rewardfood);
        delete rewardfood;
    }


    mark[0]=mark[1]=rewardtime=0;
    markmonitorA->display(mark[0]);
    markmonitorB->display(mark[1]);
    rewardmonitor->display(rewardtime);

    if(mode!=AUTO)
        for(int i=0;i<walls.size();i++){
            scene.removeItem(walls[i]);
            delete walls[i];
            walls[i]=new Wall;
            if(immortal) walls[i]->setData(GD_Type,DEFAULT);
            while(walls[i]->boundingRect().contains(QPointF(100,0))||
                  walls[i]->boundingRect().contains(QPointF(-100,0))||
                  walls[i]->px+DEFAULT_WALL_WIDTH>X_MAX||
                  walls[i]->py+DEFAULT_WALL_WIDTH>Y_MAX
            )
                walls[i]->setPosition();
            scene.addItem(walls[i]);
        }

    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);
    if(mode==DOUBLE){
        connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
        timer[1].start(DEFAULT_PERIOD);
    }

    test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
      {delete food;goto test;}
    if(mode==DOUBLE&&snakeB->shape().contains(snakeB->mapFromScene(QPointF(food->px+5,food->py+5))))
      {delete food;goto test;}
    for(int i=0;i<walls.size();i++)
      if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
      {delete food;goto test;}
    scene.addItem(food);
}


void Gamecontroller::gameover(Snake *snake){
    if(mode==SINGLE||mode==AUTO) {
        disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
        stopGame();
        if (QMessageBox::Yes == QMessageBox::information(NULL,
                              "Game Over", "Again?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes)) {
          scene.removeItem(snake);
          restartGame();
      } else {
          exit(0);
      }
    }
    else{
        snake->alive=false;
        if(snake==snakeA){
            scene.removeItem(snakeA);
            disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
        }
        else{
            scene.removeItem(snakeB);
            disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
        }
        if(!(snakeA->alive)&&!(snakeB->alive)) {
            stopGame();
            if(QMessageBox::Yes == QMessageBox::information(NULL,
                              "Game Over", "Again?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes)) {
               restartGame();
            } else exit(0);
        }
   }
}



void Gamecontroller::HandlekeyPress(QKeyEvent *event){
  switch (event->key()) {
      case Qt::Key_Left:
          snakeA->setMoveDirection(Snake::MoveLeft);
          break;
      case Qt::Key_Right:
          snakeA->setMoveDirection(Snake::MoveRight);
          break;
      case Qt::Key_Up:
          snakeA->setMoveDirection(Snake::MoveUp);
          break;
      case Qt::Key_Down:
          snakeA->setMoveDirection(Snake::MoveDown);
          break;
      case Qt::Key_A:
           snakeB->setMoveDirection(Snake::MoveLeft);
          break;
      case Qt::Key_D:
           snakeB->setMoveDirection(Snake::MoveRight);
          break;
      case Qt::Key_W:
           snakeB->setMoveDirection(Snake::MoveUp);
          break;
      case Qt::Key_S:
           snakeB->setMoveDirection(Snake::MoveDown);
          break;
      case Qt::Key_Space:
           snakeA->setMoveDirection(Snake::MoveDown);
  }
}


void Gamecontroller::InitNewFood(){
    test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    if(mode==DOUBLE&&snakeB->shape().contains(snakeB->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    for(int i=0;i<walls.size();i++)
        if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    scene.addItem(food);
    if(rewardremoved&&snakeA->effect==Snake::NOEFFECT&&snakeB->effect==Snake::NOEFFECT)
        InitReward();
}

void Gamecontroller::InitReward(){
    if((rand()%10)>5&&mode!=AUTO) {
        rewardremoved=false;
    test:
        rewardfood=new Food(REWARD);
        if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        if(mode==DOUBLE&&snakeB->shape().contains(snakeB->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        for(int i=0;i<walls.size();i++)
            if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        scene.addItem(rewardfood);
        connect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::RemoveReward);
        rewardtimer.start(REWARD_INTERVAL);
    }
}

bool Gamecontroller::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        HandlekeyPress((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}

