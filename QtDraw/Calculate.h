#pragma once

#if !defined(__STRICT_ANSI__) || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_USE_MATH_DEFINES)
#define M_PI                 3.14159265358979323846
#endif

#include <qpoint.h>
#include <qrect.h>
#include <cmath>

struct IRect
{
	QPoint topLeft;
	QPoint topRight;
	QPoint bottomLeft;
	QPoint bottomRight;
	QRect bound;
};


class CCalculate
{
public:
	static QPoint rotatePoint(const QPoint& p1, const QPoint& p2, const double& angle);
	static IRect rotateRect(const QPoint& p, const QRect& rect, const double& angle);
};

