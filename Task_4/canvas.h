#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QGraphicsView>
#include <QPen>
#include "point.h"



class Canvas : public QObject
{
    Q_OBJECT

public:
    explicit Canvas(QGraphicsView *canvas, QObject *parent = nullptr);

    enum Line{AB, CD};


    void drawPoint(const Point &pt);
    void removePoint(const Point &pt);
    void removePoints();
    void drawLine(const QPointF &pt, const QPointF &pt2, Canvas::Line);
    void drawLine(const QLineF &pt);
    void clear();
    void center();
    void update();

    void countConvexHull();
    void drawConvexHull();

    QVector<Point *> getPointsVector() const;
    QPen &getDefaultPen();

    QPen &getPointPen();

    void appendPoint(const qreal &x, const qreal &y, const qint8 &cnt);
signals:

private:
    QGraphicsView *view_;
    QGraphicsScene scene_;
    QPen penDefault_;
    QPen pointPen_;

    QVector<Point *> pointsVector;
    QVector<QPointF> convexHull;
    QLineF lineAB;
    QLineF lineCD;

    float getScale();
};

#endif // CANVAS_H
