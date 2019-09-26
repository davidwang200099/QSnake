#include "gamecontroller.h"

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
    player=NULL;
}

void Gamecontroller::initSingle(){
    rewardfood=NULL;
    rewardremoved=true;
    rewardtime=REWARD_INTERVAL/1000;
    mark[0]=mark[1]=0;
    scene.installEventFilter(this);//initialize basic data

    scene.setSceneRect(-HALF_PICTURE_WIDTH,-HALF_PICTURE_HEIGHT, 600, 400);
    scene.installEventFilter(this);

    snakeA=new Snake(mark[0],*markmonitorA,*this,100,0);
    snakeB=new Snake(mark[1],*markmonitorB,*this,INT_MIN,0,Qt::green);
    scene.addItem(snakeA);//put the snake on the scene

    for(int i=0;i<DEFAULT_WALL_COUNT;i++) {
        wall=new Wall;
        if(immortal) wall->setData(GD_Type,DEFAULT);
        while(wall->boundingRect().contains(QPointF(100,0))||
              wall->boundingRect().contains(QPointF(-100,0))||
              wall->px+DEFAULT_WALL_WIDTH>X_MAX||
              wall->py+DEFAULT_WALL_WIDTH>Y_MAX
            )//if colliding with other items
            wall->setPosition();//renew the position
        scene.addItem(wall);
        walls.push_back(wall);//deal with the walls
    }
test:
    food=new Food;
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}//if colliding with the snake
    for(int i=0;i<walls.size();i++){
      if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
          {delete food;goto test;}//if colliding with the walls
      }
      scene.addItem(food);//deal with the food


    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);//time begins to flow

    gametimer.start(1000);

}

void Gamecontroller::initDouble(){
    rewardfood=NULL;
    rewardremoved=true;
    rewardtime=REWARD_INTERVAL/1000;
    mark[0]=mark[1]=0;
    scene.setSceneRect(-HALF_PICTURE_WIDTH,-HALF_PICTURE_HEIGHT, 600, 400);
    scene.installEventFilter(this);//basic data

    snakeA=new Snake(mark[0],*markmonitorA,*this,100,0);
    snakeB=new Snake(mark[1],*markmonitorB,*this,-100,0,Qt::green);
    scene.addItem(snakeA);
    scene.addItem(snakeB);//snake

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
        walls.push_back(wall);//deal with the wall
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
    scene.addItem(food);//deal with the food

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

void Gamecontroller::countdown(){
    rewardmonitor->display(rewardtime);
    if(rewardtime>0) rewardtime--;
    else rewardtime=REWARD_INTERVAL/1000;
}

void Gamecontroller::switchimmortalMode(){
    immortal=immortal?false:true;
    for(int i=0;i<walls.size();i++)
        if(walls[i]->data(GD_Type)==WALL)
            walls[i]->setData(GD_Type,DEFAULT);
        else walls[i]->setData(GD_Type,WALL);//let the wall become invalid
}

void Gamecontroller::SnakeAteFood(Snake *snake,Food *food){
  if(food->data(GD_Type)==REWARD)
    disconnect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::removeReward);
  scene.removeItem(food);//stop counting down and remove the food

  switch(food->FunctionOfFood){//give the snake special effects accordingly
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
      if(snake==this->snakeA){//make the snake gray and begin to count down
      connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::resetSnakeAColor);
      connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
      specialtimer->start(SPECIAL_INTERVAL);
      }else {
       connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::resetSnakeBColor);
       specialtimer[1].start(SPECIAL_INTERVAL);
       connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
       }
      break;
    case Food::ACCELERATE:
      snake->effect=Snake::SPEEDUP;
      snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
      if(snake==this->snakeA){//make the snake begin to accelerate
      disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
      connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
      timer->start(DEFAULT_PERIOD/2);
      connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::resetSnakeASpeed);
      connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
      specialtimer->start(SPECIAL_INTERVAL);
      }else {
          disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
          connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
          timer[1].start(DEFAULT_PERIOD/2);
          connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::resetSnakeBSpeed);
          connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
          specialtimer[1].start(SPECIAL_INTERVAL);
       }
      break;
    case Food::SLOWDOWN:
          snake->effect=Snake::SLOWDOWN;
          snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
          if(snake==this->snakeA){//make the snake slow down
          disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
          connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
          timer->start(DEFAULT_PERIOD*2);
          connect(specialtimer,&QTimer::timeout,this,&Gamecontroller::resetSnakeASpeed);
          connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
          specialtimer->start(SPECIAL_INTERVAL);
          }else {
              disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
              connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
              timer[1].start(DEFAULT_PERIOD*2);
              connect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::resetSnakeBSpeed);
              connect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
              specialtimer[1].start(SPECIAL_INTERVAL);
           }
      break;
      case Food::BACKWARD:
          snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
          snake->moveBackward();
          break;
      case Food::RANDOMMOVE:
          snake->growing++;snake->score+=2;snake->monitor.display(snake->score);
          test://avoid colliding with the wall
          QPoint newPos=QPoint((rand()%60-30)*10,(rand()%40-20)*10);
          for(int i=0;i<DEFAULT_WALL_COUNT;i++)
             if(walls[i]->contains(newPos)) {goto test;}
          snake->randommove(newPos);
  }
  if(food->data(GD_Type)==FOOD){
    delete food;
    initNewFood();
    }
  else {
    delete food;
    rewardremoved=true;
    //food=0;
    }
}

void Gamecontroller::stopGame(){

    disconnect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::removeReward);
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);

    switch(snakeA->effect){//deal with the special effects
      case Snake::SLOWDOWN:case Snake::SPEEDUP:
disconnect(specialtimer,&QTimer::timeout,this,&Gamecontroller::resetSnakeASpeed);
      break;
      case Snake::THROUGHWALL:
disconnect(specialtimer,&QTimer::timeout,this,&Gamecontroller::resetSnakeAColor);
    }
    snakeA->effect=Snake::NOEFFECT;

  if(mode==DOUBLE)
    switch(snakeB->effect){
      case Snake::SLOWDOWN:case Snake::SPEEDUP:
disconnect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::resetSnakeBSpeed);
      break;
      case Snake::THROUGHWALL:
disconnect(specialtimer+1,&QTimer::timeout,this,&Gamecontroller::resetSnakeBColor);
    }
  snakeB->effect=Snake::NOEFFECT;

}

void Gamecontroller::restartGame(){
    snakeA->alive=true;
    snakeA->reshape();
    if(!snakeA->scene()) scene.addItem(snakeA);
    if(mode==DOUBLE){
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
    rewardmonitor->display(rewardtime);//initialize the data and items as before

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

void Gamecontroller::newGame(){
    disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    if(mode==DOUBLE)
    disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
    stopGame();
    restartGame();
}

void Gamecontroller::playBGM(){
    if(!player){
    player=new QMediaPlayer;
    list.clear();
    list.addMedia(QUrl::fromLocalFile("./bgm1.mp3"));
    list.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(&list);
    player->play();
    }else {delete player;player=NULL;}
}

void Gamecontroller::askforName(QDialog *dialog){
    QLabel *label=new QLabel(dialog);//initialize the dialog and other QObject
    label->setText("Please tell us your name:");
    label->setGeometry(150,100,300,40);

    QLineEdit *ed=new QLineEdit(dialog);
    namerecorder=ed;
    ed->setGeometry(150,150,300,40);

    QPushButton *button_sure=new QPushButton(dialog);
    button_sure->setGeometry(150,200,120,40);
    button_sure->setText(tr("确定"));
    connect(button_sure,&QPushButton::clicked,this,&Gamecontroller::getPlayerName);

    QPushButton *button_exit=new QPushButton(dialog);
    button_exit->setGeometry(300,200,120,40);
    button_exit->setText(tr("取消"));
    connect(button_exit,&QPushButton::clicked,this,&Gamecontroller::endGame);

    dialog->resize(600,450);
    dialog->show();
}

void Gamecontroller::getPlayerName(){
    ifstream fin;
    fin.open("PlayerName.txt");
    string str=namerecorder->text().toStdString();//initialize new record
    int newplayermark=mark[0];
    string time=QDateTime::currentDateTime().toString("yyyy-MM-dd,hh:mm:ss").toStdString();
    string newrecord="\t"+str+"\t"+time;

    string line[DEFAULT_RECORDNUM];
    int playermarks[DEFAULT_RECORDNUM];

    for(int i=0;i<DEFAULT_RECORDNUM;i++){//decide whether the new record should be recorded into the file
        fin>>playermarks[i];
        getline(fin,line[i],'\n');
        if(newplayermark>playermarks[i]) {//compare the new record with old ones
            std::swap(newplayermark,playermarks[i]);
            std::swap(newrecord,line[i]);
        }
    }
    ofstream fout;
    fout.open("PlayerName.txt");//rewrite the left record into the file
    for(int i=0;i<DEFAULT_RECORDNUM;i++){
        fout<<playermarks[i]<<line[i]<<"\n";
    }
    fout.close();
    exit(0);
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
          dialog=new QDialog;
          askforName(dialog);
          //exit(0);
      }
    }
    else{
        snake->alive=false;
        if(snake==snakeA){
            scene.removeItem(snakeA);//deal with dead snake respectively
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

void Gamecontroller::handlekeyPress(QKeyEvent *event){
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


void Gamecontroller::initNewFood(){
    test:
    food=new Food;//init food,avoiding colliding items
    if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    if(mode==DOUBLE&&snakeB->shape().contains(snakeB->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    for(int i=0;i<walls.size();i++)
        if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(food->px+5,food->py+5))))
        {delete food;goto test;}
    scene.addItem(food);
    if(rewardremoved&&snakeA->effect==Snake::NOEFFECT&&snakeB->effect==Snake::NOEFFECT)
        initReward();
}

void Gamecontroller::initReward(){
    if((rand()%10)>5&&mode!=AUTO) {
        rewardremoved=false;
    test:
        rewardfood=new Food(REWARD);//avoid collidings
        if(snakeA->shape().contains(snakeA->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        if(mode==DOUBLE&&snakeB->shape().contains(snakeB->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        for(int i=0;i<walls.size();i++)
            if(walls[i]->boundingRect().contains(walls[i]->mapFromScene(QPointF(rewardfood->px+5,rewardfood->py+5))))
            {delete rewardfood;goto test;}
        scene.addItem(rewardfood);
        connect(&rewardtimer,&QTimer::timeout,this,&Gamecontroller::removeReward);
        rewardtimer.start(REWARD_INTERVAL);
    }
}

void Gamecontroller::removeReward(){
    if(!rewardremoved){
      disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
      rewardmonitor->display(0);
      scene.removeItem(rewardfood);
      delete rewardfood;
      rewardremoved=true;
    }
}

void Gamecontroller::resetSnakeAColor(){
    snakeA->effect=Snake::NOEFFECT;
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
    rewardtime=REWARD_INTERVAL/1000;
    rewardmonitor->display(0);
    snakeA->ColorOfSnake=Qt::red;
}

void Gamecontroller::resetSnakeBColor(){
    snakeB->effect=Snake::NOEFFECT;
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
    rewardtime=REWARD_INTERVAL/1000;
    rewardmonitor->display(0);
    snakeB->ColorOfSnake=Qt::green;
}

void Gamecontroller::resetSnakeASpeed(){
    snakeA->effect=Snake::NOEFFECT;
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
    rewardmonitor->display(0);
    disconnect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    connect(timer,&QTimer::timeout,this,&Gamecontroller::SnakeAAdvance);
    timer->start(DEFAULT_PERIOD);
    rewardtime=REWARD_INTERVAL/1000;
}

void Gamecontroller::resetSnakeBSpeed(){
    snakeB->effect=Snake::NOEFFECT;
    disconnect(&gametimer,&QTimer::timeout,this,&Gamecontroller::countdown);
    rewardmonitor->display(0);
    disconnect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
    connect(timer+1,&QTimer::timeout,this,&Gamecontroller::SnakeBAdvance);
    timer[1].start(DEFAULT_PERIOD);
    rewardtime=REWARD_INTERVAL/1000;

}

bool Gamecontroller::eventFilter(QObject *object, QEvent *event){
    if (event->type() == QEvent::KeyPress) {//mainly deal with keyboard events
        handlekeyPress((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}

