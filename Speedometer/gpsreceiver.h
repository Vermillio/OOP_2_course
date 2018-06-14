#ifndef GPSRECEIVER_H
#define GPSRECEIVER_H
#include <QMainWindow>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QQuickView>
#include <QLabel>
#include <QGridLayout>

class GpsReceiver : public QMainWindow
{
    Q_OBJECT
public:
    GpsReceiver();
    void startGps();

private slots:
    void setGPSlocation(QGeoPositionInfo);
    void timeout();
private:
    QLabel * gpsLabel;
    QWidget * central;
    QGridLayout * layout;
    QQuickView view;
    QQuickItem *object;
    QGeoPositionInfoSource * source;
};

#endif // GPSRECEIVER_H
