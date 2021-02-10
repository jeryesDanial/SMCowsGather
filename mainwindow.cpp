#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    //Create dialog window size + drones,cows number


    int dronesNo = ui->lineEdit->text().toInt();
    int cows = ui->lineEdit_2->text().toInt();
    int speed =  ui->lineEdit_3->text().toInt();
    int speedCows =  ui->lineEdit_4->text().toInt();
    int DroneRdiosClusterMainWindow =  ui->lineEdit_5->text().toInt();
    int DroneCloseStation = ui->lineEdit_6->text().toInt();
    double CowsStabilizeX = ui->lineEdit_7->text().toDouble();
    double CowsStabilizeY = ui->lineEdit_8->text().toDouble();
    int FormXSize = ui->lineEdit_9->text().toInt();
    int FormYSize = ui->lineEdit_10->text().toInt();
    double RD = ui->lineEdit_12->text().toDouble();
    double PM = ui->lineEdit_13->text().toDouble();



    myDialog = new Dialog(0,dronesNo,cows,speed,speedCows,DroneRdiosClusterMainWindow,DroneCloseStation,CowsStabilizeX,CowsStabilizeY,FormXSize,FormYSize,RD,PM);



    myDialog->show();
    //myDialog->size();

}
