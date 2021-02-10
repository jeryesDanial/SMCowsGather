#include "dialog.h"
#include "mainwindow.h"
#include <QApplication>
#include "loginpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow t;

    LoginPage lg;


     t.show();

    return a.exec();
}
