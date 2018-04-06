#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "BT_UI.h"

BT_UI::BT_UI(uint8_t pin){
    strip = Adafruit_NeoPixel(NUM_PIXELS, pin, NEO_GRBA + NEO_KHZ800);
    
    buddyColor = strip.Color(LED_OFF, LED_OFF, LED_ON);
    waypointColor = strip.Color(LED_ON, LED_OFF, LED_OFF);

    fov = 180;
}

void BT_UI::begin(void){
    strip.begin();
}

void BT_UI::setBrightness(uint8_t b){
    
}

void BT_UI::test(void){
    for(uint8_t i = 0; i < NUM_PIXELS; i++){
        strip.setPixelColor(i, LED_ON, LED_OFF, LED_OFF);
    }
    strip.show();
    delay(1000);
    for(uint8_t i = 0; i < NUM_PIXELS; i++){
        strip.setPixelColor(i, LED_OFF, LED_ON, LED_OFF);
    }
    strip.show();
    delay(1000);
    for(uint8_t i = 0; i < NUM_PIXELS; i++){
        strip.setPixelColor(i, LED_OFF, LED_OFF, LED_ON);
    }
    strip.show();
    delay(1000);
}

void BT_UI::show(void){
    strip.show();
}

void BT_UI::clear(void){
    strip.clear();
}

void BT_UI::setBuddyLight(uint8_t light){
    strip.setPixelColor(light, buddyColor);
}

void BT_UI::setWaypointLight(uint8_t light){
    strip.setPixelColor(light, waypointColor);
}

uint8_t BT_UI::orientation2LED(float orientation){
    uint8_t degreesPerLED = fov / STRIP_LENGTH;
    uint8_t offsetOrientation = orientation + fov / 2;

    if(offsetOrientation > 360){
        offsetOrientation -= 360;
    }
    if(offsetOrientation > fov){
        //TODO: what happens when this is returned?
        return -1;
    }

    return STRIP_LENGTH - offsetOrientation / degreesPerLED;
}
