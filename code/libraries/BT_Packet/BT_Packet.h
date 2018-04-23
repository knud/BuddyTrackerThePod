#ifndef BT_PACKET_H
#define BT_PACKET_H


class BT_Packet {
    public:
        BT_Packet();
        
        void setGPS(uint32_t lat, uint32_t lng);
        bool updatesPending();
        uint32_t getPacket();
    protected:
        uint16_t UUID;
        uint8_t lat;
        uint8_t lng;
        bool updatePending;
};


#endif