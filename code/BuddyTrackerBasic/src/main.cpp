#include <Arduino.h>
#include <LinkedList.h>
#include <LoRa.h>
#include <SPI.h>
#include "BT_Packet.h"
#include "Buddy.h"


void onReceive(uint8_t packetSize);
void sendPacket(BT_Packet packet);
void updateBuddy(uint64_t UUID, uint16_t lat, uint16_t lng);
uint8_t findBuddyIndex(uint64_t UUID);


// TODO: define comparator for sorting LinkedList
LinkedList<Buddy> buddies;

// error value: -1 (unsigned)
uint32_t myLat = -1;
uint32_t myLng = -1;


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


void onReceive(uint8_t packetSize) {
    if (packetSize == 0) return; // if there's no packet, return

    while (LoRa.available()) {
        // read UUID
        uint64_t UUID = 0;
        UUID |= LoRa.read() << (0 * 8);
        UUID |= LoRa.read() << (1 * 8);
        UUID |= LoRa.read() << (2 * 8);
        UUID |= LoRa.read() << (3 * 8);
        UUID |= LoRa.read() << (4 * 8);
        UUID |= LoRa.read() << (5 * 8);
        UUID |= LoRa.read() << (6 * 8);
        UUID |= LoRa.read() << (7 * 8);

        // read partial lat
        uint16_t lat_partial = 0;
        lat_partial |= LoRa.read() << (0 * 8);
        lat_partial |= LoRa.read() << (1 * 8);

        // read partial lat
        uint16_t lng_partial = 0;
        lng_partial |= LoRa.read() << (0 * 8);
        lng_partial |= LoRa.read() << (1 * 8);

        // save data
        updateBuddy(UUID, lat_partial, lng_partial);
    }
}


void sendPacket(BT_Packet packet){
    LoRa.beginPacket();
    LoRa.write(packet.getPacket());
    LoRa.endPacket();
}


// currently adds unknown buddies
// shows everyone
void updateBuddy(uint64_t UUID, uint16_t lat_partial, uint16_t lng_partial){
    uint8_t index = findBuddyIndex(UUID);
    
    // add unknown buddies
    if(index == -1){
        Buddy newBuddy = Buddy(UUID);
        buddies.add(newBuddy);
        index = buddies.size() - 1;
    }

    // can't compute other Buddy's location without knowning own location
    if(myLat == -1 || myLng == -1){
        return;
    }
    // clear LSBs
    uint32_t lat = myLat & 0x00;
    uint32_t lng = myLng & 0x00;
    // replace LSBs
    lat &= lat_partial;
    lng &= lng_partial;

    Buddy currentBuddy = buddies.get(index);
    currentBuddy.setLat(lat);
    currentBuddy.setLng(lng);

}


// returns 0 if no match
uint8_t findBuddyIndex(uint64_t UUID){
    for(uint8_t i = 0; i < buddies.size(); i++){
        if(UUID == buddies.get(i).getUUID()) return i;
    }
    return 0;
}