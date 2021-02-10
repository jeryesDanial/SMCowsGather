// dialog.cpp

#include "dialog.h"
#include "ui_dialog.h"
#include <time.h>
#include <QDebug>


/**
 * @brief Dialog::Dialog
 * @param parent
 * @param a1 size of drone
 * @param a2 size of cows
 */
Dialog::Dialog(QWidget *parent,int a1,int a2,int speed,int speedCows,int DronesRdiosCluster,int ClosestStation,double CowsX ,double CowsY,int FXsize,int FYSize,double RD,double PM):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    // create our object and add it to the scene

    ndrones=a1;
    ncows = a2;
    DroneSpeed = speed;
    CowsSpeed = speedCows;
    droneRadius = DronesRdiosCluster;
    DroneClosestStation = ClosestStation;
    _CowsX= CowsX;
    _CowsY = CowsY;
    _FormXSize= FXsize;
    _FormYSize = FYSize;
    _RD = RD;
    _PM = PM;
    qDebug() << "1" ;
    setUpScene(ndrones,ncows,DroneSpeed,CowsSpeed);

    qDebug() << "2";
     timerOfGame = new QTimer(this);
     GameDeltaTime = 0;//QDateTime::currentMSecsSinceEpoch();





}
/**
 * @brief Dialog::update check status of drones ! who arrived then flip direction! and run the timer start new convex ! do whatever you want
 * check if there's left cows nearby! ... set target to one of them ( outside of it ) of there's not ... find new station for drone one from the convex!
 * if all cows have father.. ( drone nearby from the convex then the drone can rest no need for it ) ... and then put the drone to move
 */
void Dialog::update()
{
    GameDeltaTime++;



    int c=0;
    int c2=0;
    int CounterArriveTargetDrone=0;
    for(int t=0;t<droneInstances.length();t++){
        //qDebug() << droneInstances[t]->arrivedToTarget;
        //qDebug() << droneInstances[t]->PoweredOn;
        if(droneInstances[t]->arrivedToTarget==true && droneInstances[t]->PoweredOn ==true ){
            c++;
        }
        if(droneInstances[t]->PoweredOn==true ){
            c2++;
        }
        if (droneInstances[t]->arrivedToTarget ){
            CounterArriveTargetDrone++;
        }

    }
    //qDebug()<< "CounterArriveTargetDrone= "  << CounterArriveTargetDrone << ",  poweredOnnnnnnnnnnnnn" << c2++ ;

    if(c!=c2){
        qDebug() << "fucccccccccccccccccccccccccccccccccccccccccccccccccccccckkkkkkkkkkkkkkkkkkkkkkkkk";
        return;
    }


    c=0;
    for(int t=0;t<cowInstances.length();t++){
       if(cowInstances[t]->arrivedToTarget==true)
           c++;
     }
    if(c>=cowInstances.length()){

        qDebug() << "check cows number";
       // return;
    }

    convexCows = convex_hull(cowInstances);

    for (int i = 0; i < convexCows.length(); i++) {

        convexCows[i]->isConvexPint = true;
    }

    drawConvex();
    /*check if all arrived*/
    /*check if arrived to one iteration to mid station and there are still cows outside stations*/
    bool isDroneOnMidStation=false;
    for(int i=0;i<stationPoints.length();i++){
        if(droneInstances[0]->currentTarget==stationPoints[i]){
            isDroneOnMidStation = true;//There's no bug here*/
            break;
        }
    }
    if(isDroneOnMidStation){

        for(int j=0;j< cowInstances.length();j++){
            if(cowInstances[j]->arrivedToTarget)
                cowInstances[j]->setOpacity(0.2);
        }
       bool flag = true;
       for(int i=0;i< convexCows.length();i++){
           if(convexCows[i]->arrivedToTarget==false){
               flag = false;
               qDebug() << "netka33333333333 honnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn";
           }

       }
       qDebug() << "They Arrived";
       if(flag){/*if arrived to mid stations*/
           /*check if not e to do*/
           if(droneInstances[0]->currentTarget!=e){

                    qDebug() << "They Arrived - 1 not e";
                    for(int j=0;j < cowInstances.length();j++){
                        //kinda reset
                            cowInstances[j]->setOpacity(1);
                            cowInstances[j]->setTarget(e);
                            cowInstances[j]->arrivedToTarget = false;
                            cowInstances[j]->isMoving = false;
                            cowInstances[j]->taken = false;
                           // cowInstances[j]->arrivedToTarget =false;
                    }
                    qDebug() << stationPoints.size();
                    qDebug() << stationPoints.length();
                    for(int i=0;i<stationPoints.size();i++){
                            stationPoints[i]=e;
                            droneInstances[i]->PoweredOn = true;

                    }
                    setDronesManualy = true;
               }


       }
         qDebug() << "Prepare drone on convex";

        prepareDronesOnConvex(e,droneInstances.length());
        qDebug() << "finish - Prepare drone on convex";
        for(int t=0;t<droneInstances.length();t++){
            //arrivedCowsToMidStations+=   droneInstances[t]->myNearByCows.size();
            droneInstances[t]->myNearByCows.clear();
            if(droneInstances[t]->PoweredOn==true)
                droneInstances[t]->isMoving = true;
            droneInstances[t]->arrivedToTarget = false;
         }

    }else{
                     ui->lineEdit->setText(QLocale().toString(GameDeltaTime));
                     qDebug() << "Arrived To Convex!";
                     //GameDeltaTime++;
                     qDebug() <<GameDeltaTime;//(QDateTime::currentMSecsSinceEpoch()-GameDeltaTime)/pow(10,12);// timerOfGame;
                     startDroneMovingCow();
                     file.open ("codebind.txt");
                     file <<"NQ:" << ndrones << ",NC:" << ncows << ",QV:" << DroneSpeed << ",CV:"  <<
                            CowsSpeed << ",EVX:" << _CowsX << ",EVY:" << _CowsY <<
                            ",QR:" << droneRadius << ",MainStationRaduos:"<<DroneClosestStation<<
                            ",FA:" <<_FormXSize  << ",RD:" << _RD << ",PM:" << _PM << ",TotalTime:" <<  GameDeltaTime  ;

                     file.close();


    }

}


/*Start moving again drones on convex!*/
void Dialog::startDroneMovingCow(){

    for(int i=0;i< droneInstances.length();i++){
         if(droneInstances[i]->PoweredOn==true){
                droneInstances[i]->myNearByCows.clear();
                droneInstances[i]->isMoving = true;
                droneInstances[i]->arrivedToTarget = false;
                for(int j=0;j< cowInstances.length();j++){
                    if(cowInstances[j]->x()<droneInstances[i]->x()+droneRadius && cowInstances[j]->x()>droneInstances[i]->x() - droneRadius && cowInstances[j]->y() < droneInstances[i]->y()+droneRadius &&  cowInstances[j]->y() > droneInstances[i]->y()-droneRadius ){
                        droneInstances[i]->addNearbyCow(cowInstances[j]);
                        cowInstances[j]->setTarget(getClosestStation(droneInstances[i])); /*stationPoints[i]);*/
                        droneInstances[i]->setTarget(getClosestStation(droneInstances[i])); /*(stationPoints[i]);*/
                        cowInstances[j]->isMoving = true;
                       // cowInstances[j]->taken = false;//changed here
                        cowInstances[j]->arrivedToTarget =false;
                    }
              }
         }
    }



}

QPointF Dialog::getClosestStation(Drone* drone){
    float min =30000;
    int minIndex=0;
    for(int i=0;i < stationPoints.length() ;i++){
       float distance1 = sqrt(pow(drone->pos().x() - stationPoints[i].x(), 2) + pow(drone->pos().y() - stationPoints[i].y(), 2));
        if(distance1 <= min){
            min=distance1;
            minIndex = i;
        }
    }
    return stationPoints[minIndex];
}

/**
 * @brief Dialog::convex_hull finds the convex hull of cows
 * @param P cows objects
 * @return convex cows
 */
QVector<Cow*> Dialog::convex_hull(QVector<Cow*>  P) {
    int n = P.size(), k = 0,k1=0;
    std::vector<QPointF> H(2 * n);
    QVector<Cow*> Result(2*n);

    P = sortPoints(P);

    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]->pos()) <= 0){
            k--;
            k1--;
        }

        H[k++] = P[i]->pos();
        Result[k1++] = P[i];
    }
    // Build upper hull
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(H[k - 2], H[k - 1], P[i]->pos()) <= 0) {
            k--;
            k1--;
        }
        H[k++] = P[i]->pos();
        Result[k1++] = P[i];
    }
    Result.resize(k - 1);
    return Result;
}
/**
 * @brief Dialog::cross
 * @param O
 * @param A
 * @param B
 * @return a value to be used in convex hull
 */
double Dialog::cross(const QPointF &O, const QPointF &A, const QPointF &B) {
    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
}
/**
 * @brief Dialog::sortPoints sorts the cow points for convex hull computation
 * @param A the cows objects
 * @return the sorted result
 */
QVector<Cow*> Dialog::sortPoints(QVector<Cow*> A){
    for(int i=0;i<A.size()-1;i++){
        for(int j=i+1;j<A.size();j++){
            qreal x=A[i]->pos().x(),y=A[i]->pos().y(),p1=A[j]->pos().x(),p2=A[j]->pos().y();
            bool res =  x < p1 || (x == p1 && y < p2);
            if(!res){
               Cow* temp = A[i];
               A[i] = A[j];
               A[j] = temp;
            }
        }
    }
    return A;
}

QPointF Dialog::computeMiddlePoint()
{
    int x=0,y=0;
    for(int i=0;i<cowInstances.length();i++){
    x+= cowInstances[i]->pos().x();
    y+= cowInstances[i]->pos().y();
    }
    return QPointF(x/cowInstances.length(),y/cowInstances.length());
}
/**
 * @brief Dialog::getDronePoint get the drone place to set before moving phase
 * @param convexCowPlace
 * @param middle
 * @return point of drone
 */
QPointF Dialog::getDronePoint(QPointF convexCowPlace, QPointF middle) {
    float slope, intercept;
    float x1 = convexCowPlace.x(), y1 = convexCowPlace.y(), x2 = middle.x(), y2 = middle.y();
    float dx, dy;
    dx = x2 - x1;
    dy = y2 - y1;
    slope = dy / dx;
    intercept = y1 - slope * x1;
    float xnew1, ynew1, xnew2, ynew2;
    xnew1 = (x1)+50 * (x1 - x2) / sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    ynew1 = slope*xnew1 + intercept;
    xnew2 = (x1)-50 * (x1 - x2) / sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    ynew2 = slope*xnew2 + intercept;
    float distance1 = sqrt(pow(xnew1 - x2, 2) + pow(ynew1 - y2, 2));
    float distance2 = sqrt(pow(xnew2 - x2, 2) + pow(ynew2 - y2, 2));
    if (distance1 > distance2)
        return QPointF(xnew1, ynew1);
    else
        return QPointF(xnew2, ynew2);
}

void Dialog::setUpScene(int numberOfDrones,int numberOfCows,int DroneSped,int CowsSped)
{
    timerForDrone = NULL;
    timerForDrone2 = NULL;
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // insert new instances to the cow vector its dynamic
    for (int i = 0; i < numberOfCows; i++) {
           QPointF thisPoint((rand() % _FormXSize), (rand() % _FormYSize));
     //      QLineF lineBetween(thisPoint,e);

           Cow *tempCow = new Cow(thisPoint.x(),thisPoint.y(),1,CowsSped,_CowsX,_CowsY,_RD,_PM);



           cowInstances.push_back(tempCow);
    }

    //add the main point for gathering

    e = QPointF(5,20);
    //this three rows ceate the gathiring point
    //double rad = 20;
    //scene->addEllipse(e.x()-double(DroneClosestStation), e.y()-double(DroneClosestStation), double(DroneClosestStation)*3.0, double(DroneClosestStation)*3.0,
    //            QPen(), QBrush(Qt::SolidPattern));

    //add the cows on the scene
    for (int i = 0; i < numberOfCows; i++) {
           Cow *tempCow = cowInstances[i];
            QLineF lineBetween(tempCow->pos(),e);
            tempCow->setAnlge(90-lineBetween.angle());
            scene->addItem(tempCow);

    }

    /*find convex of cow points*/
    convexCows = convex_hull(cowInstances);

       for (int i = 0; i < convexCows.length(); i++) {

           convexCows[i]->isConvexPint = true;
       }

    drawConvex();


    //add rectangle on the convex
    for (int i = 0; i < convexCows.length(); i++) {
        scene->addRect(convexCows[i]->pos().x(),convexCows[i]->pos().y(),10,10,QPen(Qt::red),QBrush(Qt::black));

    }
    /*set Drones in places*/
  /* Assume this is the middle point *///setDronesOnConvex this function insert the drone instance to the drones vector
    setDronesOnConvex( e ,numberOfDrones);

    InitializeStationPoints(numberOfDrones);
    for (int i = 0; i < droneInstances.length(); i++) {
        scene->addItem(droneInstances[i]);

    }

    for(int i=0;i< droneInstances.length();i++){
        if(droneInstances[i]->PoweredOn==true){
            for(int j=0;j< cowInstances.length();j++){

                        if(cowInstances[j]->x()<droneInstances[i]->x()+droneRadius && cowInstances[j]->x()>droneInstances[i]->x() - droneRadius && cowInstances[j]->y() < droneInstances[i]->y()+droneRadius &&  cowInstances[j]->y() > droneInstances[i]->y()-droneRadius /*&& cowInstances[j]->arrivedToTarget==false*/){

                            droneInstances[i]->addNearbyCow(cowInstances[j]);

                            cowInstances[j]->setTarget(getClosestStation(droneInstances[i]));/*(stationPoints[i]);*/
                            droneInstances[i]->setTarget(getClosestStation(droneInstances[i]));/*(stationPoints[i]);*/
                            droneInstances[i]->arrivedToTarget=false;
                        }
                 }
        }
    }
    for(int t=0;t<droneInstances.length();t++){
         if(droneInstances[t]->PoweredOn==true){
            droneInstances[t]->isMoving=true;
            for(int i=0;i<droneInstances[t]->myNearByCows.length();i++){
                   droneInstances[t]->myNearByCows.at(i)->isMoving=true;
            }
         }

    }


    /*start moving cows*/

    scene->update();
    /*Put this in comment now*/
    timerForDrone = new QTimer(this);
    connect(timerForDrone,SIGNAL(timeout()),scene,SLOT(advance()));
    //20 frame in second
    timerForDrone->start(20);

    timerForDrone2 = new QTimer(this);
    connect(timerForDrone2, SIGNAL(timeout()), this, SLOT(update()));
    timerForDrone2->start(20);

}

void Dialog::InitializeStationPoints(int numberOfDrones)
{
    for(int i=0;i<droneInstances.length();i++){
        QPointF g = droneInstances[i]->pos();
        QPointF s((g.x()+e.x())/1.3,(g.y()+e.y())/1.3);
        stationPoints.push_back(s);
        int rad = 10;
        //This row create the small point gathering
        //scene->addEllipse(s.x()-rad, s.y()-rad, rad*2.0, rad*2.0,
        //            QPen(), QBrush(Qt::SolidPattern));
    }
}

void Dialog::drawConvex()
{
    for(int  i=0;i<convexCows.size()-1;i++){
        QPointF w = convexCows[i]->pos();
        QPointF w2 = convexCows[i+1]->pos();
        QLineF lineBetween(w,w2);
        scene->addLine(lineBetween,QPen(Qt::blue));
    }
    QPointF w = convexCows[0]->pos();
    QPointF w2 = convexCows[convexCows.size()-1]->pos();
    QLineF lineBetween(w,w2);
    scene->addLine(lineBetween,QPen(Qt::blue));
}



/**
 * @brief Dialog::setDronesOnConvex randomly choose drones on convex, or by selecting with spaces
 * @param target - the middle point
 * @param numberOfDrones - number of drones to create
 */
void Dialog::setDronesOnConvex(QPointF target, int numberOfDrones)
{
    // TODO Add new way of choosing
    /*sample number of Drones from convex*/
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QVector<int> sampledIndices;
    tempConvex = convexCows;

    if (numberOfDrones < convexCows.length()){

        for(int i=0;i<numberOfDrones;i++){
            //int indexC = qrand()%convexCows.length();
            int indexC = qrand()%convexCows.length();

            qDebug() << "randommmmmmmmmmmmmmmmmmmmmm " <<indexC << "   " << convexCows.length();
             //while(sampledIndices.contains(indexC)){
             //       indexC = qrand()%convexCows.length();
             //}

             qDebug() << "sampledIndices.contains(indexC) " <<sampledIndices.contains(indexC) ;
             sampledIndices.push_back(indexC);
             Cow* currentCow = convexCows[indexC];
              /*Put drone map to cow and middle point or target*/
             QPointF w = getDronePoint(currentCow->pos(),target);
             QLineF lineBetween(currentCow->pos(),target);
              Drone* newDrone = new Drone(w.x(),w.y(),currentCow->angle,i,DroneSpeed);
              droneInstances.push_back(newDrone);


        }

    }else{
        for(int i=0;i<convexCows.length();i++){
           qDebug() << i ;
           sampledIndices.push_back(i);
           Cow* currentCow = convexCows[i];
            /*Put drone map to cow and middle point or target*/
           QPointF w = getDronePoint(currentCow->pos(),target);
           QLineF lineBetween(currentCow->pos(),target);
            Drone* newDrone = new Drone(w.x(),w.y(),currentCow->angle,i,DroneSpeed);
            droneInstances.push_back(newDrone);


        }

    }


}

//TODO upgrade this
void Dialog::prepareDronesOnConvex(QPointF target, int numberOfDrones)
{
    /*sample number of Drones from convex*/
    /*get flag to ignore arrived cows if true just ignore arrived cows!!1*/
     QVector<int> sampledIndices;
    int numberOfArrived=0;
    for(int i=0;i<convexCows.size();i++){
        if(convexCows[i]->arrivedToTarget==true){
            numberOfArrived++;
            sampledIndices.push_back(i);

        }
    }
    /*Turn of The drones with no need to work ;) */
    qsrand(QDateTime::currentMSecsSinceEpoch());
    if(numberOfDrones>convexCows.size()-numberOfArrived){

        int i;
        for(i=0;i<(convexCows.size()-numberOfArrived);i++){
            droneInstances[i]->PoweredOn = true;
        }

        for(;i<droneInstances.size();i++){
            droneInstances[i]->PoweredOn = false;
        }
    }


    for(int i=0;i<numberOfDrones;i++){
        //Choose indexC closest for current Drone!.. if already taken by another drone take another..
        if(droneInstances[i]->PoweredOn){
                int indexC = -1;
                float min = 1000;
                for(int j=0;j<convexCows.length();j++){
                   float dist = sqrt(pow(droneInstances[i]->pos().x() - convexCows[j]->pos().x(), 2) + pow(droneInstances[i]->pos().y() - convexCows[j]->pos().y(), 2));
                   if ((dist <= min) && !(convexCows[j]->taken)/* && !(convexCows[j]->arrivedToTarget)*/){
                       min = dist;
                       indexC = j;
                   }
                }
                if(min==1000){
                    droneInstances[i]->PoweredOn = false;
                    continue;
                }

//                while(sampledIndices.contains(indexC))
//                    indexC = qrand()%convexCows.length();
//                sampledIndices.push_back(indexC);
                Cow* currentCow = convexCows[indexC];
                currentCow->taken = true;
                /*Put drone map to cow and middle point or target*/
                QPointF w = getDronePoint(currentCow->pos(),droneInstances[i]->pos());
                QLineF lineBetween(droneInstances[i]->pos(),currentCow->pos());
             /*if(setDronesManualy){
                      droneInstances[i]->pos() = w;
                      droneInstances[i]->isMoving = false;
                      droneInstances[i]->arrivedToTarget = true;
                }*/
                droneInstances[i]->currentTarget = w;
                int rad= 4;
                if(stationPoints[i]==e){
                    scene->addEllipse(w.x()-rad, w.y()-rad, rad*2.0, rad*2.0,
                                QPen(Qt::yellow), QBrush(Qt::yellow));
                }

              if(setDronesManualy){
                    scene->addLine(lineBetween,QPen(Qt::blue));
              }
                if(!setDronesManualy)
                   droneInstances[i]->setRotation(90-lineBetween.angle());
                else
                   droneInstances[i]->setRotation(90-lineBetween.angle());
        }
    }
}

void Dialog::setParameters(int numberOfDrones, int numberOfCows)
{
    ndrones = numberOfDrones;
    ncows = numberOfCows;
}


Dialog::~Dialog()
{
    delete ui;
}




void Dialog::on_pushButton_clicked()
{


}
