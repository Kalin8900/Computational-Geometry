#include "canvas.h"
#include <QDebug>
#include <QGraphicsTextItem>
#include <algorithm>

Canvas::Canvas(QGraphicsView *canvas, QObject *parent)
    : QObject(parent),
      view_(canvas)
{
    penDefault_.setColor(Qt::white);
    penAB_.setColor(Qt::white);
    penCD_.setColor(Qt::white);
    pointPen_.setColor(Qt::red);

    view_->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);

    pointsVector.append(&pointA);
    pointsVector.append(&pointB);
    pointsVector.append(&pointC);
    pointsVector.append(&pointD);
    pointsVector.append(&pointP);
}


void Canvas::drawPoint(const Point &pt)
{
    scene_.addRect(pt.rect(), pointPen_, QBrush(pointPen_.color()));
    QGraphicsTextItem *text = scene_.addText(pt.name());
    text->setPos(pt.pos().x() - 6, pt.pos().y() - 25);
    text->setDefaultTextColor(pointPen_.color());
    view_->setScene(&scene_);
    view_->update();
}

void Canvas::removePoint(const Point &pt)
{
    auto *point = scene_.itemAt(pt.pos(), view_->transform());
    auto *letter = scene_.itemAt({pt.pos().x() - 6, pt.pos().y() - 25}, view_->transform());
    if(point && letter)
    {
        scene_.removeItem(letter);
        scene_.removeItem(point);
    }
}

void Canvas::removePoints()
{
    clear();

    drawLine(pointA.pos(), pointB.pos(), Canvas::AB);
    drawLine(pointC.pos(), pointD.pos(), Canvas::CD);
}

void Canvas::drawLine(const QPointF &pt, const QPointF &pt2, Canvas::Line linename)
{
    switch (linename)
    {
    case Canvas::AB:
        scene_.addLine({pt, pt2}, penAB_);
        break;
    case Canvas::CD:
        scene_.addLine({pt, pt2}, penCD_);
        break;
    }
    update();
}

void Canvas::clear()
{
    scene_.clear();
    view_->resetTransform();
    update();
}

Point Canvas::getPointA() const
{
    return pointA;
}

void Canvas::setPointA(const Point &value)
{
    removePoint(pointA);
    pointA = value;
}

Point Canvas::getPointB() const
{
    return pointB;
}

void Canvas::setPointB(const Point &value)
{
    removePoint(pointB);
    pointB = value;
}

Point Canvas::getPointC() const
{
    return pointC;
}

void Canvas::setPointC(const Point &value)
{
    removePoint(pointC);
    pointC = value;
}

Point Canvas::getPointD() const
{
    return pointD;
}

void Canvas::setPointD(const Point &value)
{
    removePoint(pointD);
    pointD = value;
}

Point Canvas::getPointP() const
{
    return pointP;
}

void Canvas::setPointP(const Point &value)
{
    removePoint(pointP);
    pointP = value;
}

QVector<Point *> Canvas::getPointsVector() const
{
    return pointsVector;
}

QPen &Canvas::getPenAB()
{
    return penAB_;
}

QPen &Canvas::getPenCD()
{
    return penCD_;
}

QPen &Canvas::getPointPen()
{
    return pointPen_;
}

QPen Canvas::getDefaultPen()
{
    return penDefault_;
}

void Canvas::center()
{
    view_->centerOn(getPointP().pos());
    update();
}

void Canvas::update()
{
    scene_.update();
    view_->setScene(&scene_);
    view_->update();
}


float Canvas::getScale()
{
    float scale;
    float maxX = 0, maxY = 0;
    float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max();
    for(const auto &pt : pointsVector)
    {
        maxX = std::max(static_cast<float>(pt->pos().x()), maxX);
        maxY = std::max(static_cast<float>(pt->pos().y()), maxY);
        minX = std::min(static_cast<float>(pt->pos().x()), minX);
        minY = std::min(static_cast<float>(pt->pos().y()), minY);
    }


    scale = std::min(scene_.sceneRect().width() / (maxY - minY),
                     scene_.sceneRect().height() / (maxX - minX));

    return scale;
}



