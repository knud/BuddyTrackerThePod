#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <unordered_map>
#include "BT_Packet.h"
#include "Buddy.h"

unordered_map<uint32_t, Buddy> buddyMap;

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