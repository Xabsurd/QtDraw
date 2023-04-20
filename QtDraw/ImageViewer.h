#pragma once
#include<qgraphicsview.h>
#include <qcolor.h>
#include <QBrush>
#include <QTimer>
#include <QMovie>
#include <qgraphicsitem>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <qmimedata.h>
#include "Calculate.h"
#include <cmath>
class ImageViewer :public QGraphicsView
{
	Q_OBJECT;
public:
	ImageViewer(int width, int height);
	void loadImage(const QString& url);
	void zoomTo(const QPoint& point, const double& zoom);
	double limitedZoom(double zoom);
	QSize getDrawSize();
	void resizeToTile();
	void resizeToContain();
	void changeRotate(double angle);

protected:
	//void drawBackground(QPainter* painter, const QRectF& rect);
	//void drawForeground(QPainter* painter, const QRectF& rect);

	void resizeEvent(QResizeEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
protected slots:
	void onAnimationTimer();

private:
	void refresh();
	void reset();
	void startAnimation();
	void stopAnimation();
	QGraphicsScene* mp_scene=nullptr;
	QGraphicsPixmapItem* mp_pixmapItem = nullptr;
	QTimer* mp_animationTimer = nullptr;
	QMovie* mp_movie = nullptr;
	QPoint m_drawPoint;
	QPoint m_downPoint;
	QPoint m_downPoint_D;
	IRect m_downRect;
	IRect m_baseRect;
	const double m_maxZoom = 100;
	double m_minZoom = 0.01;
	double m_downZoom = 1;
	double m_zoom = 1;
	double m_rotation = 0;
	double m_zoomStep = 0.15;
	bool m_canMove = false;
	bool m_canScale = false;
	bool m_contain = true;
};

