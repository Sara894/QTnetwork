#include "PaintScene.h"

PaintScene::PaintScene(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
void PaintScene::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	QPalette Pal(palette());
	Pal.setColor(QPalette::Window, Qt::black);
	setAutoFillBackground(true);
	setPalette(Pal);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::blue, 11, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	for (int i = 0; i < vv.size(); ++i)
		if (i > 0) painter.drawEllipse(vv[i], 12, 12);
}
void PaintScene::mousePressEvent(QMouseEvent* pe) {
	draw = true;
}
void PaintScene::mouseMoveEvent(QMouseEvent* pe) {
	if (draw) {
		vv.push_back(pe->pos());
	}
	update();
}
void PaintScene::mouseReleaseEvent(QMouseEvent* pe) {
	draw = false;
	QPixmap scr;
	QPixmap pixmap(this->size());
	scr = this->grab(QRect(QPoint(0, 0), QSize(-1, 1)));
	QFile mFile("test.txt");
	mFile.open(QIODevice::WriteOnly);
	QTextStream fin(&mFile);
	fin.setRealNumberPrecision(2);
	QImage img;
	img = scr.toImage();
	img = img.scaled(28, 28);
	int w = img.size().width();
	int h = img.size().height();
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			fin << img.pixelColor(j, i).blue() / 255. << " ";
		}
		fin << "\n";
	}
	mFile.close();
}
void PaintScene::clear() {
	vv.clear();
	update();
}
PaintScene::~PaintScene()
{
}
