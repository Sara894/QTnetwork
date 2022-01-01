#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainwindow.h"
#include "NetWork.h"
#include "PaintScene.h"
class mainwindow : public QMainWindow
{
    Q_OBJECT
    PaintScene* PS;
    NetWork NW;
    data_NetWork NW_config;
public:
    mainwindow(QWidget* parent = Q_NULLPTR);
    data_NetWork ReadDataNetWork(string path);
    void ReadTest(double* input, int input_n);
    QProgressBar** ph;
    QLabel** lbl;
signals:
    void endWriteToFile();
public slots:
    void StartGuess();
    void clearLCD();
private:
    Ui::mainwindowClass ui;
};
