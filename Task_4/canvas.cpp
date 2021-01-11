#include "canvas.h"
#include <QDebug>
#include <QGraphicsTextItem>
#include <algorithm>
#include <QStack>

Canvas::Canvas(QGraphicsView *canvas, QObject *parent)
    : QObject(parent),
      view_(canvas)
{
    penDefault_.setColor(Qt::white);
    pointPen_.setColor(Qt::red);

    view_->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
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

    //removePoints
}

void Canvas::clear()
{
    scene_.clear();
    view_->resetTransform();
    update();
}

QVector<Point *> Canvas::getPointsVector() const
{
    return pointsVector;
}

QPen &Canvas::getPointPen()
{
    return pointPen_;
}

QPen &Canvas::getDefaultPen()
{
    return penDefault_;
}

void Canvas::center()
{
    update();
}

void Canvas::update()
{
    scene_.update();
    view_->setScene(&scene_);
    view_->update();
}

bool isPointOnTheRight(QLineF line, QPointF point)
{
    const auto vec = line.dx() * (point.y() - line.y1()) - (point.x() - line.x1()) * line.dy();
    return vec > 0;
}

template<class T>
T getNextToTop(QStack<T> stack)
{
    auto top = stack.pop();
    auto toReturn = stack.top();
    stack.push(top);
    return toReturn;
}

void Canvas::countConvexHull()
{
    auto p0 = pointsVector.at(0)->pos();

    for(const auto point : pointsVector)
    {
        if(point->pos().x() < p0.x())
            p0 = point->pos();
        else if(point->pos().x() == p0.x() && point->pos().y() < p0.y())
            p0 = point->pos();
    }

    auto refPt = p0;
    refPt.setX(refPt.x() - 10); //point higher than p0

    QLineF compLine(p0, refPt);


    QVector<QPointF> sorted;

    for(const auto &el : pointsVector)
        sorted.push_back(el->pos());

    std::sort(sorted.begin(), sorted.end(), [compLine](QPointF lhs, QPointF rhs){
        QLineF lhsLine(compLine.p1(), lhs);
        QLineF rhsLine(compLine.p1(), rhs);
        return compLine.angleTo(rhsLine) < compLine.angleTo(lhsLine);
    });

    QStack<QPointF> stack;
    stack.push(sorted[0]);
    stack.push(sorted[1]);
    stack.push(sorted[2]);

    for(int i = 3; i < sorted.size(); ++i)
    {
        auto prev = getNextToTop(stack);
        auto last = stack.top();
        QLineF comparingLine(last, prev);

        while(isPointOnTheRight(comparingLine, sorted[i]))
        {
            stack.pop();
            prev = getNextToTop(stack);
            last = stack.top();
            comparingLine.setP1(last);
            comparingLine.setP2(prev);
        }

        stack.push(sorted[i]);
    }

    convexHull.swap(stack);
    qInfo() << convexHull;
}

void Canvas::drawConvexHull()
{
    scene_.addPolygon(QPolygonF(convexHull), penDefault_);
    update();
}

void Canvas::appendPoint(const qreal &x, const qreal &y, const qint8 &cnt)
{
    pointsVector.push_back(new Point({x, y}, QString::number(cnt)));
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



