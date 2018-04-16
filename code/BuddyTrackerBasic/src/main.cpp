#include <Arduino.h>
#include <LinkedList.h>
#include <LoRa.h>
#include <SPI.h>
#include "BT_Packet.h"
#include "Buddy.h"


// TODO: define comparator for sorting LinkedList
LinkedList<Buddy> buddies;

uint32_t myLat = -9999;
uint32_t myLat = -9999;


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
    // TODO: collision avoidance
    
    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
}


void onReceive(int packetSize) {
    if (packetSize == 0) return; // if there's no packet, return

    while (LoRa.available()) {
        // read UUID
        uint16_t UUID = 0;
        UUID |= LoRa.read() << (0 * 8);
        UUID |= LoRa.read() << (1 * 8);
        UUID |= LoRa.read() << (2 * 8);
        UUID |= LoRa.read() << (3 * 8);
        UUID |= LoRa.read() << (4 * 8);
        UUID |= LoRa.read() << (5 * 8);
        UUID |= LoRa.read() << (6 * 8);
        UUID |= LoRa.read() << (7 * 8);
        UUID |= LoRa.read() << (8 * 8);
        UUID |= LoRa.read() << (9 * 8);
        UUID |= LoRa.read() << (10 * 8);
        UUID |= LoRa.read() << (11 * 8);
        UUID |= LoRa.read() << (12 * 8);
        UUID |= LoRa.read() << (13 * 8);
        UUID |= LoRa.read() << (14 * 8);
        UUID |= LoRa.read() << (15 * 8);

        // read partial lat
        uint8_t lat_partial = 0;
        lat_partial |= LoRa.read() << (0 * 8);
        lat_partial |= LoRa.read() << (1 * 8);

        // read partial lat
        uint8_t lng_partial = 0;
        lng_partial |= LoRa.read() << (0 * 8);
        lng_partial |= LoRa.read() << (1 * 8);

        // save data
        updateBuddy(UUID, lat_partial, lng_partial);
    }
}


void sendPacket(BT_Packet packet){
    LoRa.beginPacket();
    Lora.write(packet.getPacket());
    LoRa.endPacket();
}


void updateBuddy(uint16_t UUID, uint32_t lat, uint32_t lng){
    uint8_t index = findBuddyIndex(UUID);
    if(index == -1){
        Buddy newBuddy = Buddy(UUID);
        buddies.add(newBuddy);
        index = buddies.size() - 1;
    }
    Buddy currentBuddy = buddies.get(index);
    currentBuddy.setLat(lat);
    currentBuddy.setLng(lng);

}


// returns -1 if no match
uint8_t findBuddyIndex(uint16_t UUID){
    for(uint8_t i = 0; i < buddies.size(); i++){
        if(UUID == buddies.get(i).getUUID()) return i;
    }
    return -1;
}