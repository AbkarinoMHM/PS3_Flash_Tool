#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //set mainwindow background color
    QPalette pal = a.palette();
    pal.setColor(QPalette::Window, QColor(0, 170,255));
    a.setPalette(pal);
    //end

    w.show();

    return a.exec();
}
