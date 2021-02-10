// myitem.cpp

#include "drone.h"
#include <QDebug>

Drone::Drone(qreal x,qreal y,qreal rotationAngle,int id,int SPD)
{
    speed = SPD;/*five pixels at a time*/
    angle  = rotationAngle;
    setRotation(angle);
    isMoving = false;
    PoweredOn = true;
    arrivedToTarget = false;
    PoweredOn = true;
    setPos(x,y);
    myID = id;

}

QRectF Drone::boundingRect() const
{
    // outer most edges
    return QRect(0,0,35,35);
}


void Drone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*QRectF rec= boundingRect();
    QBrush Brush(Qt::gray);
    painter->fillRect(rec,Brush);
    painter->drawRect(rec);*/
    QRectF rec= boundingRect();
    myID = myID%20;
    switch(myID) {
        case 0 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone0.png"));
                  break;
        case 1 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone1.png"));
              break;
        case 2 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone2.png"));
          break;
        case 3 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone3.png"));
           break;
        case 4 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone4.png"));
          break;
        case 5 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone5.png"));
          break;
        case 6 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone6.png"));
          break;
         case 7 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone7.png"));
          break;
        case 8 :  painter->drawImage(rec,QImage(":/imgeResource/images/drone8.png"));
            break;
        case 9:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone0.jpg"));
            break;
        case 10:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone1.jpg"));
            break;
        case 11:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone2.jpg"));
            break;
        case 12:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone3.jpg"));
            break;
        case 13:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone4.jpg"));
            break;
        case 14:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone5.jpg"));
            break;
        case 15:
                  painter->drawImage(rec,QImage(":/imgeResource/images/drone6.jpg"));
            break;

    }

}


void Drone::addNearbyCow(Cow* a)
{
    myNearByCows.push_back(a);
}

void Drone::setTarget(QPointF targetPoint)
{
    this->currentTarget = targetPoint;

    QLineF lineBetween(this->pos(),targetPoint);
    this->setRotation(90-lineBetween.angle());
     qDebug() << "setTarget " << targetPoint << "  setRotation  " << 90-lineBetween.angle();
}
void Drone::advance(int phase)
{
    qDebug() << phase;
    if(!isMoving){
     qDebug() << "not moving";
     return;
    }else{
        qDebug() << "moving";
    }

    if(!PoweredOn){
        qDebug() << "power down" ;
        return;
    }else{
        qDebug() << "power" ;
    }

    QPointF e = currentTarget;
    if(!phase)
    {

        qDebug() << "not phase";
         return;
    }else{
         qDebug() << "phase";
    }

    QPointF Location = this->pos();
    if(Location.x()<e.x()+10 && Location.x()>e.x() - 10 && Location.y() < e.y()+10 &&  Location.y() > e.y()-10){
        arrivedToTarget=true;
        isMoving = false;
        qDebug() << "arrived to target and not moving";
        return;
    }else{
        qDebug() << "contnue moving to target";
         isMoving = true;
    }

    setPos(mapToParent(0,-speed));
}


