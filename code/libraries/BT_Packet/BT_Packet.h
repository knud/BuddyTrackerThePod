#ifndef BT_PACKET_H
#define BT_PACKET_H


#define PACKET_LENGTH 12


class BT_Packet {
    public:
        BT_Packet();
        
        void setGPS(uint32_t lat, uint32_t lng);
        bool updatesPending();
        byte *getPacket();
    protected:
        uint64_t UUID;
        uint8_t lat;
        uint8_t lng;
        bool updatePending;
};


#endif