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


    void doStuff(QPointF pt, QPointF pt2);

    void drawPoint(const Point &pt);
    void removePoint(const Point &pt);
    void removePoints();
    void drawLine(const QPointF &pt, const QPointF &pt2, Canvas::Line);
    void drawLine(const QLineF &pt);
    void clear();
    void center();
    void update();

    Point getPointA() const;
    void setPointA(const Point &value);

    Point getPointB() const;
    void setPointB(const Point &value);

    Point getPointC() const;
    void setPointC(const Point &value);

    Point getPointD() const;
    void setPointD(const Point &value);

    Point getPointP() const;
    void setPointP(const Point &value);


    QVector<Point *> getPointsVector() const;
    QPen getDefaultPen();

    QPen &getPenAB();

    QPen &getPenCD();

    QPen &getPointPen();

signals:

private:
    QGraphicsView *view_;
    QGraphicsScene scene_;
    QPen penDefault_;
    QPen penAB_;
    QPen penCD_;
    QPen pointPen_;

    Point pointA;
    Point pointB;
    Point pointC;
    Point pointD;
    Point pointP;

    QVector<Point *> pointsVector;
    QLineF lineAB;
    QLineF lineCD;

    float getScale();
};

#endif // CANVAS_H
