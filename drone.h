// myitem.h

#ifndef DRONE_H
#define DRONE_H

#include <QPainter>
#include <QGraphicsItem>
#include <cow.h>
// class for customization
class Drone :public QGraphicsItem
{
public:
    Drone(qreal x,qreal y,qreal rotationAngle,int id,int SPD);

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
     void addNearbyCow(Cow* a);
    // item state
    QVector<Cow*> myNearByCows;
    QPointF currentTarget;
    bool isMoving;
    bool PoweredOn;
    bool arrivedToTarget;
    int myID;
    void setTarget(QPointF targetPoint);
    //  bool directionForwardBackward; we need a way to flip direction
    bool Pressed;
protected:

    void advance(int phase);
    qreal angle;
    qreal speed;
};

#endif // MYITEM_H
