#include "ImageViewer.h"
ImageViewer::ImageViewer(int width, int height)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSceneRect(-10000000, -10000000, 20000000, 20000000);

	mp_scene = new QGraphicsScene();
	mp_pixmapItem = new QGraphicsPixmapItem();
	QString img_path = "D:\\Users\\15653\\Pictures\\1124254.png";

	mp_scene->setBackgroundBrush(QColor(0, 0, 0, 0));
	mp_scene->addItem(mp_pixmapItem);
	//mp_pixmapItem->setPixmap(QPixmap(img_path));

	//changeRotate(0);
	setScene(mp_scene);
	setStyleSheet("padding:0px;border:0px");
	this->centerOn(width / 2, height / 2);
	mp_animationTimer = new QTimer(this);
	connect(mp_animationTimer, &QTimer::timeout, this, &ImageViewer::onAnimationTimer, Qt::UniqueConnection);


	loadImage(img_path);

}


//void ImageViewer::drawBackground(QPainter* painter, const QRectF& rect)
//{
//	//QGraphicsView::drawBackground(painter, rect);
//}
//
//void ImageViewer::drawForeground(QPainter* painter, const QRectF& rect)
//{
//}

void ImageViewer::resizeEvent(QResizeEvent* event)
{

	if (m_contain)
	{
		resizeToContain();
	}
	else
	{
		double wb = (double)this->width() / this->height();
		double ib = (double)m_baseRect.bound.width() / m_baseRect.bound.height();
		if (wb > ib)
		{
			m_minZoom = (double)this->height() / m_baseRect.bound.height();
		}
		else
		{
			m_minZoom = (double)this->width() / m_baseRect.bound.width();
		}
		m_minZoom = m_minZoom > 1 ? 1 : m_minZoom;
		double diff_width = this->width() - m_baseRect.bound.width() * m_zoom;
		double diff_height = this->height() - m_baseRect.bound.height() * m_zoom;
		double diff_x = (m_baseRect.topLeft.x() - m_baseRect.bound.left()) * m_zoom;
		double diff_y = (m_baseRect.topLeft.y() - m_baseRect.bound.top()) * m_zoom;
		bool reDraw = false;
		QPoint move(0, 0);
		//QPoint v_p = m_drawPoint;//CCalculate::rotatePoint(move, m_drawPoint, m_rotation);
		if (diff_width > 0)
		{
			reDraw = true;

			move.setX(std::round(diff_width / 2 + diff_x));

		}
		else
		{
			if (m_baseRect.bound.width() * m_zoom + m_drawPoint.x() - diff_x < this->width())
			{
				reDraw = true;
				move.setX(std::round(this->width() - m_baseRect.bound.width() * m_zoom + diff_x));
			}
			else
			{
				if (m_drawPoint.x()-diff_x>0) {
					reDraw = true;
					move.setX(diff_x);
				}
				else {
					move.setX(m_drawPoint.x());
				}
			}

		}
		if (diff_height > 0)
		{
			reDraw = true;

			move.setY(std::round(diff_height / 2 + diff_y));
		}
		else
		{
			if (m_baseRect.bound.height() * m_zoom + m_drawPoint.y() - diff_y < this->height())
			{
				reDraw = true;
				move.setY(std::round(this->height() - m_baseRect.bound.height() * m_zoom + diff_y));
			}
			else
			{
				if (m_drawPoint.y() - diff_y > 0) {
					reDraw = true;
					move.setY(diff_y);
				}
				else {
					move.setY(m_drawPoint.y());
				}
			}
		}
		if (reDraw)
		{
			m_drawPoint = move; //CCalculate::rotatePoint(QPoint(0, 0), move, m_rotation * -1);
			//DrawImage(drawPoint, drawSize);
			refresh();
		}
	}
}

void ImageViewer::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case 32:
		if (m_contain)
		{
			resizeToTile();
		}
		else
		{
			resizeToContain();
		}
		break;
	default:
		break;
	}
}

void ImageViewer::mouseMoveEvent(QMouseEvent* event)
{
	QPoint mousePoint = event->pos();
	if (m_canMove)
	{
		double left = mousePoint.x() - m_downPoint.x();
		double top = mousePoint.y() - m_downPoint.y();
		QPoint move(0, 0);
		double diff_width = this->width() - m_downRect.bound.width();
		double diff_height = this->height() - m_downRect.bound.height();
		double diff_x = m_downRect.topLeft.x() - m_downRect.bound.left();
		double diff_y = m_downRect.topLeft.y() - m_downRect.bound.top();
		if (m_downRect.bound.width() > this->width())
		{
			if (m_downRect.bound.left() + left < 0)
			{
				if (m_downRect.bound.left() + left > diff_width)
				{
					move.setX(m_downPoint_D.x() + left);
				}
				else
				{
					move.setX(diff_width + diff_x);
				}
			}
			else
			{
				move.setX(diff_x);
			}
		}
		else
		{
			move.setX(diff_width / 2 + diff_x);
		}
		if (m_downRect.bound.height() > this->height())
		{
			if (m_downRect.bound.top() + top < 0)
			{
				if (m_downRect.bound.top() + top > diff_height)
				{
					move.setY(m_downPoint_D.y() + top);
				}
				else
				{
					move.setY(diff_height + diff_y);
				}
			}
			else
			{
				move.setY(diff_y);
			}
		}
		else
		{
			move.setY(diff_height / 2 + diff_y);
		}
		m_drawPoint = move;;// CCalculate::rotatePoint(QPoint(0, 0), move, m_rotation * -1);
		refresh();
	}
	else if (m_canScale)
	{

		double diff_y = mousePoint.y() - m_downPoint.y();
		double _zoom = m_downZoom + diff_y / -200;
		_zoom = limitedZoom(_zoom);
		if (_zoom != m_zoom && _zoom > 0)
		{
			zoomTo(m_downPoint, _zoom);

		}

	}
}

void ImageViewer::mousePressEvent(QMouseEvent* event)
{

	m_downPoint = event->pos();
	if (event->button() == Qt::LeftButton) {
		if (m_contain) {
			return;
		}
		m_canMove = true;
		m_downPoint_D = m_drawPoint;//CCalculate::rotatePoint(QPoint(0, 0), m_drawPoint, m_rotation);
		m_downRect = CCalculate::rotateRect(m_drawPoint, QRect(m_drawPoint, getDrawSize()), m_rotation);
	}
	else if (event->button() == Qt::RightButton) {
		m_canScale = true;
		m_downZoom = m_zoom;

	}
}


void ImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
	m_canMove = false;
	m_canScale = false;
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
	QPoint wheelPoint = event->pos();
	double _zoom = 1;
	if (event->delta() > 0) {
		_zoom = m_zoom + m_zoom * m_zoomStep;
	}
	else {
		_zoom = m_zoom - m_zoom * m_zoomStep;
	}
	_zoom = limitedZoom(_zoom);
	if (_zoom != m_zoom && _zoom > 0)
	{
		zoomTo(wheelPoint, _zoom);
		if (m_canMove)
		{
			m_downPoint = event->pos();
			m_downPoint_D = m_drawPoint;//CCalculate::rotatePoint(QPoint(0, 0), m_drawPoint, m_rotation);
			m_downRect = CCalculate::rotateRect(m_drawPoint, QRect(m_drawPoint, getDrawSize()), m_rotation);
		}
	}
}


void ImageViewer::dragEnterEvent(QDragEnterEvent* event)
{
	if (!event->mimeData()->urls()[0].toLocalFile().right(3).compare("jpg", Qt::CaseInsensitive)
		|| !event->mimeData()->urls()[0].toLocalFile().right(3).compare("gif", Qt::CaseInsensitive)
		|| !event->mimeData()->urls()[0].toLocalFile().right(3).compare("png", Qt::CaseInsensitive)
		|| !event->mimeData()->urls()[0].toLocalFile().right(3).compare("bmp", Qt::CaseInsensitive)
		|| !event->mimeData()->urls()[0].toLocalFile().right(4).compare("jpeg", Qt::CaseInsensitive)
		)
	{
		event->accept();//接受鼠标拖入事件
	}
	else
	{
		event->ignore();//否则不接受鼠标事件
	}
}

void ImageViewer::dragMoveEvent(QDragMoveEvent* event)
{
	event->accept();
}

void ImageViewer::dropEvent(QDropEvent* event)
{
	QString url = event->mimeData()->urls().first().toLocalFile();;
	loadImage(url);
}


void ImageViewer::loadImage(const QString& url)
{
	QString s = url.right(3);
	reset();
	if (!url.right(3).compare("gif", Qt::CaseInsensitive)) {
		mp_movie = new QMovie(url);
		mp_movie->jumpToFrame(0);
		mp_pixmapItem->setPixmap(mp_movie->currentPixmap());
		startAnimation();
	}
	else {
		QPixmap img(url);
		mp_pixmapItem->setPixmap(img);
	}

	mp_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
	mp_pixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	mp_pixmapItem->update();
	changeRotate(0);

	resizeToContain();
}
double ImageViewer::limitedZoom(double zoom)
{
	if (zoom > m_maxZoom)
	{
		zoom = m_maxZoom;
	}
	else if (zoom < m_minZoom)
	{
		zoom = m_minZoom;
	}
	if (zoom == m_minZoom)
	{
		if (mp_pixmapItem->pixmap().height() < this->height() && mp_pixmapItem->pixmap().width() < this->width())
		{
			resizeToTile();
		}
		else
		{
			resizeToContain();
		}
		return -1;
	}
	else
	{
		m_contain = false;
	}
	return zoom;
}
void ImageViewer::zoomTo(const QPoint& point, const double& zoom)
{
	//视觉矩形
	IRect v_Rect = CCalculate::rotateRect(m_drawPoint, QRect(m_drawPoint, QSize(mp_pixmapItem->pixmap().width() * zoom, mp_pixmapItem->pixmap().height() * zoom)), m_rotation);
	QPoint move(0, 0);

	double x = point.x() - v_Rect.topLeft.x();
	double y = point.y() - v_Rect.topLeft.y();
	double m_x = x - (x / m_zoom * zoom);
	double m_y = y - (y / m_zoom * zoom);
	double diff_width = this->width() - v_Rect.bound.width();
	double diff_height = this->height() - v_Rect.bound.height();
	double diff_x = v_Rect.topLeft.x() - v_Rect.bound.left();
	double diff_y = v_Rect.topLeft.y() - v_Rect.bound.top();
	if (diff_width < 0)
	{
		if (v_Rect.bound.left() + m_x < 0)
		{
			if (v_Rect.bound.left() + m_x > diff_width)
			{
				move.setX(std::round(v_Rect.topLeft.x() + m_x));
			}
			else
			{
				move.setX(std::round(diff_width + diff_x));
			}
		}
		else
		{
			move.setX(diff_x);
		}
	}
	else
	{
		move.setX(std::round(diff_width / 2 + diff_x));
	}
	if (diff_height < 0)
	{
		if (v_Rect.bound.top() + m_y < 0)
		{
			if (v_Rect.bound.top() + m_y > diff_height)
			{
				move.setY(std::round(v_Rect.topLeft.y() + m_y));
			}
			else
			{
				move.setY(std::round(diff_height + diff_y));
			}
		}
		else
		{
			move.setY(diff_y);
		}
	}
	else
	{
		move.setY(std::round(diff_height / 2 + diff_y));
	}
	m_drawPoint = move;// CCalculate::rotatePoint(m_drawPoint, move, m_rotation * -1);
	m_zoom = zoom;
	refresh();
}


void ImageViewer::resizeToTile()
{
	m_contain = false;
	m_zoom = 1;
	//drawSize = new Size(baseSource.PixelWidth, baseSource.PixelHeight);
	IRect ir = CCalculate::rotateRect(QPoint(0, 0), QRect(QPoint(0, 0), getDrawSize()), m_rotation);
	double diff_width = this->width() - ir.bound.width();
	double diff_height = this->height() - ir.bound.height();
	double diff_x = ir.topLeft.x() - ir.bound.left();
	double diff_y = ir.topLeft.y() - ir.bound.top();
	m_drawPoint = QPoint(diff_width / 2 + diff_x, diff_height / 2 + diff_y);

	refresh();
	//DrawImage(drawPoint, drawSize);
}
void ImageViewer::resizeToContain()
{
	m_contain = true;
	double wb = (double)this->width() / this->height();
	double ib = (double)m_baseRect.bound.width() / m_baseRect.bound.height();
	if (wb > ib)
	{
		m_zoom = (double)this->height() / m_baseRect.bound.height();
		//drawSize.width() = mp_pixmapItem->pixmap().width * m_zoom;
		//drawSize.height() = baseSource.PixelHeight * zoom;
		m_drawPoint.setX(std::round(m_baseRect.topLeft.x() - m_baseRect.bound.left() * m_zoom + (this->width() - m_baseRect.bound.width() * m_zoom) / 2));
		m_drawPoint.setY(std::round(m_baseRect.topLeft.y() - m_baseRect.bound.top() * m_zoom));
	}
	else
	{
		m_zoom = (double)this->width() / m_baseRect.bound.width();
		/*drawSize.width() = baseSource.PixelWidth * zoom;
		drawSize.height() = baseSource.PixelHeight * zoom;*/
		m_drawPoint.setX(std::round(m_baseRect.topLeft.x() - m_baseRect.bound.left() * m_zoom));
		m_drawPoint.setY(std::round(m_baseRect.topLeft.y() - m_baseRect.bound.top() * m_zoom + (this->height() - m_baseRect.bound.height() * m_zoom) / 2));
	}
	//m_drawPoint = CCalculate::rotatePoint(m_drawPoint, m_drawPoint, m_rotation * -1);
	m_minZoom = m_zoom > 1 ? 1 : m_zoom;
	refresh();
}
void ImageViewer::changeRotate(double angle)
{
	m_rotation = angle;
	m_baseRect = CCalculate::rotateRect(QPoint(0, 0), QRect(QPoint(0, 0), QSize(mp_pixmapItem->pixmap().width(), mp_pixmapItem->pixmap().height())), m_rotation);
}

void ImageViewer::startAnimation()
{
	stopAnimation();
	mp_animationTimer->start(mp_movie->nextFrameDelay());
}

void ImageViewer::stopAnimation()
{
	mp_animationTimer->stop();
}

void ImageViewer::onAnimationTimer()
{
	if (mp_movie->currentFrameNumber() == mp_movie->frameCount() - 1) {
		mp_movie->jumpToFrame(0);
	}
	else {
		if (!mp_movie->jumpToNextFrame()) {
			//qDebug() << "[Error] QMovie:" << movie->lastErrorString();
			this->stopAnimation();
			return;
		}
	}
	mp_pixmapItem->setPixmap(std::move(mp_movie->currentPixmap()));
	mp_animationTimer->setInterval(mp_movie->nextFrameDelay());
}




void ImageViewer::refresh()
{
	//mp_pixmapItem.
	//this.resetsc
	//this->scale(1.1,1.1);
	//mp_pixmapItem->setRotation(0);
	//QPoint p = CCalculate::rotatePoint(QPoint(0, 0), m_drawPoint, m_rotation * -1);

	mp_pixmapItem->setRotation(m_rotation);

	mp_pixmapItem->setScale(m_zoom);
	mp_pixmapItem->setPos(m_drawPoint);
	//mp_pixmapItem->update();
	//mp_pixmapItem->setPos(0,0);
	//mp_pixmapItem->setOffset(100,0);

	//this->rotate();

	//mp_pixmapItem->setPos(m_zoom * 10, m_zoom * 10);
}

void ImageViewer::reset()
{
	stopAnimation();
	m_drawPoint = QPoint(0, 0);
	mp_movie = nullptr;
}

QSize ImageViewer::getDrawSize()
{
	return QSize(mp_pixmapItem->pixmap().width() * m_zoom, mp_pixmapItem->pixmap().height() * m_zoom);
}