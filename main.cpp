#include <QApplication>
#include <QMessageBox>
#include "gamecontroller.h"
#include "mainwindow.h"
int main(int argc,char **argv){
    QApplication app(argc,argv);
    srand(time(NULL));
    MainWindow w;
    w.askUser();
    w.show();
    return app.exec();
}
