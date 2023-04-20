#include "Calculate.h"

QPoint CCalculate::rotatePoint(const QPoint& p1, const QPoint& p2, const double& angle)
{
	double radian = angle / 180 * M_PI;
	double x = p2.x() - p1.x();
	double y = p2.y() - p1.y();
	double x1 = x * cos(radian) - y * sin(radian) + p1.x();
	double y1 = x * sin(radian) + y * cos(radian) + p1.y();

	return QPoint(std::round(x1), std::round(y1));
}

IRect CCalculate::rotateRect(const QPoint& p, const QRect& rect, const double& angle)
{
	QPoint topLeft = rotatePoint(p, rect.topLeft(), angle);
	QPoint topRight = rotatePoint(p, rect.topRight(), angle);
	QPoint bottomLeft = rotatePoint(p, rect.bottomLeft(), angle);
	QPoint bottomRight = rotatePoint(p, rect.bottomRight(), angle);
	QPoint r_topLeft(std::min(std::min(std::min(topLeft.x(), topRight.x()), bottomLeft.x()), bottomRight.x()),
		std::min(std::min(std::min(topLeft.y(), topRight.y()), bottomLeft.y()), bottomRight.y()));
	QPoint r_bottomRight(std::max(std::max(std::max(topLeft.x(), topRight.x()), bottomLeft.x()), bottomRight.x()),
		std::max(std::max(std::max(topLeft.y(), topRight.y()), bottomLeft.y()), bottomRight.y()));
	IRect _rect = {
		topLeft,
		topRight,
		bottomLeft,
		bottomRight,
		QRect(r_topLeft,r_bottomRight)
	};
	return _rect;
}
