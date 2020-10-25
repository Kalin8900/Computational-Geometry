#include "point.h"

Point::Point(const QPointF &point, const QString &name, const int size)
    : pos_(point),
      name_(name),
      size_(size)
{
    QPoint topLeft(point.x() - size / 2, point.y() - size / 2);
    QSizeF rectSize(size, size);
    QRectF temp(topLeft, rectSize);
    rect_ = temp;
}

QString Point::name() const
{
    return name_;
}

QPointF Point::pos() const
{
    return pos_;
}

QRectF Point::rect() const
{
    return rect_;
}

Point &Point::operator=(const Point &rhs)
{
    pos_ = rhs.pos_;
    name_ = rhs.name_;
    rect_ = rhs.rect_;

    return *this;
}

void Point::setPos(const QPointF &pos)
{
    pos_ = pos;
}
