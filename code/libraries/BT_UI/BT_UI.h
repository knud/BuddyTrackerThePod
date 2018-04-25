#ifndef BT_UI_H
#define BT_UI_H

#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS  8
#define LED_OFF     0
#define LED_ON      255

class BT_UI {
    public:
        BT_UI(uint8_t pin);

        void begin(void);
        void setBrightness(uint8_t b);
        void test(void);
        void show(void);
        void clear(void);

        void setBuddyLight(uint8_t light);
        void setWaypointLight(uint8_t light);
        uint8_t orientation2LED(float orientation);
    protected:
        Adafruit_NeoPixel strip;
        uint8_t stripPin;
        uint32_t buddyColor;
        uint32_t waypointColor;
        uint8_t fov;
};

#endif