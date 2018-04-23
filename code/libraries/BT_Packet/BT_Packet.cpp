#include <Arduino.h>
#include "BT_Packet.h"

BT_Packet::BT_Packet(){
    updatePending = false;
}

void BT_Packet::setGPS(uint32_t lat, uint32_t lng){
    this->lat = (uint8_t)(lat & 0xffffffff);
    this->lng = (uint8_t)(lng & 0xffffffff);
    updatePending = true;
}

bool BT_Packet::updatesPending(){
    return updatePending;
}

uint32_t BT_Packet::getPacket(){
    uint32_t packet;
    packet = UUID << (4 * 8);
    packet &= lat << (2 * 8);
    packet &= lng;
    return packet;
}