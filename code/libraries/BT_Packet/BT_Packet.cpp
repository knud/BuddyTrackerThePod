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

byte *BT_Packet::getPacket(){
    byte packet[PACKET_LENGTH];
    for(int i = 0; i < 8; i++){
        packet[i] = (UUID >> (8 * i)) % 0xff;
    }
    packet[9] = lat >> 8;
    packet[10] = lat % 0xff;
    packet[11] = lng >> 8;
    packet[12] = lng % 0xff;
    return packet;
}