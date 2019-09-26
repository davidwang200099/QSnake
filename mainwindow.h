#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPushButton>
#include <QCheckBox>
#include <vector>
#include <qapplication.h>
#include <qmessagebox.h>
#include "gamecontroller.h"
#include "constant.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void askUser();
    void initGamecontroller(Mode mode);
    void initMonitor();
    void initFunction();
    void switchNightMode(){
        if(scene->backgroundBrush()==QBrush(Qt::black))
            scene->setBackgroundBrush(QPixmap("d.jpg"));
        else scene->setBackgroundBrush(QBrush(Qt::black));
    }
	void newGame();

    QGraphicsScene *scene;
    QGraphicsView *view;
    Gamecontroller *ctrl;
    Mode mode;

};

#endif // MAINWINDOW_H
