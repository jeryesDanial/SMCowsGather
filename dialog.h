// dialog.h

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include "drone.h"
#include "cow.h"
#include <map>
#include <QVector>
#include <iostream>
#include <fstream>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0,int a1=1,int a2=1,int speed=1,int speedCow=1,
                    int DronesRdiosCluster=55,int ClosestStation=30000,double CowsX =0.0,
                    double CowsY=0.0,int FXsize=500,int FYSize=500,double RD = 0,double PM=0);
    /*Functions of convex Hull*/
    QVector<Cow*> convex_hull(QVector<Cow*>  P);
    double cross(const QPointF &O, const QPointF &A, const QPointF &B);
    QVector<Cow*> sortPoints(QVector<Cow*> A);
    /*end of Functions of convex Hull*/
    QPointF computeMiddlePoint();
    QVector<Cow*> convexCows;
    QVector<Cow*> tempConvex;
    void setDronesOnConvex(QPointF target,int numberOfDrones);
    QPointF e;
    QVector<QPointF> stationPoints;
    bool setDronesManualy=false;
    /*another round of drones*/
    void prepareDronesOnConvex(QPointF target, int numberOfDrones);
    void setParameters(int numberOfDrones,int numberOfCows);
    QString systemState;
    QPointF getDronePoint(QPointF convexCowPlace, QPointF middle);
    void setUpScene(int numberOfDrones,int numberOfCows,int DroneSped,int CowsSped);
    void InitializeStationPoints(int numberOfDrones);
    void startDroneMovingCow();
    QPointF getClosestStation(Drone* cow);
    int droneRadius=55;
    void drawConvex();
    int arrivedCowsToMidStations=0;
    ~Dialog();
    int ndrones=5;
    int ncows=10;
    int DroneSpeed;
    int CowsSpeed;
    int GameDeltaTime;
    int DroneClosestStation;
    double _CowsX;
    double _CowsY;
    std::ofstream file;
    int _FormXSize,_FormYSize;
    double _RD ;
    double _PM;
public slots:
        void update();
private slots:
        void on_pushButton_clicked();

private:

    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QVector<Drone*> droneInstances;
    QVector<Cow*> cowInstances;
    QTimer *timerForDrone;
    QTimer* timerForDrone2;
    QTimer *timerForCow;
    QTimer *timerOfGame;
};

#endif // DIALOG_H
