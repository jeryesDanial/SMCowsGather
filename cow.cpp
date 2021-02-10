#include "cow.h"
#include <QDebug>

Cow::Cow(qreal x,qreal y,qreal rotationAngle,int SPD,float CowsX,float CowsY,double RD,double PM)
{
    speed = SPD;/*five pixels at a time*/
    angle  = rotationAngle;
    setRotation(angle);
    int StartX = 40;
    int StartY = 40;
    isMoving = false;
    arrivedToTarget = false;
    SpeedX = CowsX;
    SpeedY = CowsY;
    RdCow = RD;
    PmCow = PM;
    setPos(mapToParent(x,y));


}

QRectF Cow::boundingRect() const
{
    // outer most edges
    // this row decide the cows size
     return QRect(0,0,60,60);
   //return QRect(0,0,20,20);
}
void Cow::setAnlge(qreal tangle)
{
    angle  = tangle;
    setRotation(angle);
}

void Cow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec= boundingRect();
    painter->setBrush(Qt::red);
    painter->drawImage(rec,QImage(":/imgeResource/images/cow.png"));

}

void Cow::setTarget(QPointF targetPoint)
{
    this->currentTarget = targetPoint;
    QLineF lineBetween(this->pos(),targetPoint);
    this->setRotation(90-lineBetween.angle());
}

void Cow::getFar()
{

   qreal w= 4;
    setPos(mapToParent(0,-w));
}


void Cow::advance(int phase)
{



     if (!isMoving && !arrivedToTarget && !isConvexPint){
         int randomNum = random()%100;
        // setPos(mapToParent(0.1,-0.01));

          if (randomNum < PmCow*100){
              qDebug() <<"randomNum= " << randomNum <<"PmCow= " << PmCow*100;
              SpeedX += RdCow/1000;
              SpeedY += RdCow/1000;
          }else{
               qDebug() <<"SpeedX= " << SpeedX <<"SpeedY= " << SpeedY;
          }
          setPos(mapToParent(SpeedX,SpeedY));
     }
    if(!isMoving){

          return;
    }

    if(arrivedToTarget)
        return;
    QPointF e = currentTarget;
    if(!phase)
        return;
    QPointF Location = this->pos();

      //qDebug() << this->pos();
      //qDebug() << e.x();
      //qDebug() << e.y();



    if(Location.x()<e.x()+10 && Location.x()>e.x() - 10 && Location.y() < e.y()+10 &&  Location.y() > e.y()-10){

        //qDebug() <<" moshleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
        isMoving = false;
        arrivedToTarget=true;

        return;
    }
   else{
       // qDebug() << "bakra advanceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
     setPos(mapToParent(0,-speed));
    }

}
/*Operator for sort*/
bool operator < (Cow const& lhs, Cow const& rhs)
{
    qreal x=lhs.pos().x(),y=lhs.pos().y(),p1=rhs.pos().x(),p2=rhs.pos().y();
    return x < p1 || (x == p1 && y < p2);
}

