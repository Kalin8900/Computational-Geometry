#ifndef POINT_H
#define POINT_H
#include <QPointF>
#include <QRectF>
#include <QString>

class Point {
public:
    Point(const QPointF &point, const QString &name, const int size = 4);

    Point() = default;

    QString name() const;
    QPointF pos() const;
    QRectF rect() const;

    Point &operator=(const Point &rhs);
    void setPos(const QPointF &pos);

private:
    QPointF pos_;
    QString name_;
    int size_;
    QRectF rect_;
};



#endif // POINT_H
