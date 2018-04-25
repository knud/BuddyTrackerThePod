#ifndef BT_GPS_H
#define BT_GPS_H

#include <Adafruit_GPS.h>

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

class BT_GPS {
    public:
        BT_GPS(void);

        bool newDataAvailable(void);
        // TODO: add getLat, getLng, etc.
    protected:
        Adafruit_GPS GPS;
};

#endif