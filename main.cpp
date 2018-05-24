#include "gpsreceiver.h"


#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GpsReceiver w;
    w.show();
    return a.exec();
}
