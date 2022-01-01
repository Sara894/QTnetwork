#pragma once

#include <QtWidgets>
#include <QtGui>
#include <QPixmap>
#include "ui_PaintScene.h"

class PaintScene : public QWidget
{
	Q_OBJECT
	bool draw;
	QVector <QPointF> vv;
	QImage pic;
public:
	PaintScene(QWidget* parent = Q_NULLPTR);
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	~PaintScene();
public slots:
	void clear();

private:
	Ui::PaintScene ui;
};
