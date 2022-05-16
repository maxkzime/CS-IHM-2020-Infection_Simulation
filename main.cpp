// CS-IHM-2020/Infection-Simulation/main.cpp
// BODIN Maxime C2
// 22/16/05

#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
