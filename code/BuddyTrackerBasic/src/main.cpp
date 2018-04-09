#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include "BT_Packet.h"
#include "Buddy.h"

// unordered map is a pain.
// Instead, store Buddy objects in an array and
// have a way to iterate through and locate them.

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("BuddyTracker");

    LoRa.setPins(8, 4, 3);
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop() {

}