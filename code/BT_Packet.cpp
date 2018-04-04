#include "BT_Packet.h"

BT_Packet::BT_Packet(){
    updatePending = false;
}

void BT_Packet::setGPS(uint8_t lat, uint8_t lng){
    this.lat = lat;
    this.lng = lng;
    updatePending = true;
}

bool updatesPending(){
    return updatePending;
}

uint32_t getPacket(){
    uint32_t packet;
    packet = UUID << (4 * 8);
    packet &= lat << (2 * 8);
    packet &= lng;
    return packet;
}