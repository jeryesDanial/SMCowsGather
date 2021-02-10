#ifndef COW_H
#define COW_H

#include <QPainter>
#include <QGraphicsItem>
// class for customization
class Cow :public QGraphicsItem
{
public:
    Cow(qreal x,qreal y,qreal rotationAngle,int SPD,float CowsX = 0.0,float CowsY=0.0,double RD=0,double PM=0);
    QRectF boundingRect() const;
    // overriding paint()
    void setAnlge(qreal tangle);
    void getFar();
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
    friend bool operator< ( Cow const& a, Cow const& b);
    void setTarget(QPointF targetPoint);
    // item state
    bool Pressed;
    bool isMoving;
    bool taken;
    bool isConvexPint = false;
    QPointF currentTarget;
       bool arrivedToTarget;
    //Drone* myNearbyDrone;
  //  bool directionForwardBackward; we need a way to flip direction
    qreal angle;
    float SpeedX=0.0;
    float SpeedY=0.0;
    double RdCow,PmCow;
protected:

    void advance(int phase);
    qreal speed;
};

#endif // MYITEM_H
