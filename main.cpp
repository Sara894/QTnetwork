#include "mainwindow.h"
#include "PaintScene.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    //l.display(4);
    QApplication a(argc, argv);
    QLCDNumber l;
    l.display("C");
    mainwindow w;
    PaintScene p;
    //p.show();
    //l.show();
    w.show();
    return a.exec();
}
