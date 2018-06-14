#include "gpsreceiver.h"

GpsReceiver::GpsReceiver()
{
    //init widgets
    layout = new QGridLayout();
    central=new QWidget();
    gpsLabel=new QLabel("No data");
    gpsLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(gpsLabel, 0, 0, Qt::AlignCenter);
    central->setLayout(layout);
    setCentralWidget(central);
    setWindowTitle("Speedometer");

    startGps();
}

void GpsReceiver::startGps()
{
    // obtain the location data source
    source = QGeoPositionInfoSource::createDefaultSource(this);

    // select positioning method
    source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

    // when the position has changed the setGPSLocation slot is called
    connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
            this, SLOT(GpsReceiver::setGPSlocation(QGeoPositionInfo)));

    // start listening for position updates
    source->startUpdates();
}

void GpsReceiver::timeout()
{
    setGPSlocation(source->lastKnownPosition());
    source->startUpdates();
}

void GpsReceiver::setGPSlocation(QGeoPositionInfo geoPositionInfo)
{
    QString text="location is unknown";

    if (geoPositionInfo.isValid())
    {
        // get the current location coordinates
        QGeoCoordinate geoCoordinate = geoPositionInfo.coordinate();

        // transform coordinates to lat/lon
        qreal latitude = geoCoordinate.latitude();
        qreal longitude = geoCoordinate.longitude();

        text=QString("Latitude=%1\nLongitude=%2")
                .arg(latitude,0,'g',8)
                .arg(longitude,0,'g',8);
    }

    //update label text
    gpsLabel->setText(text);
}
