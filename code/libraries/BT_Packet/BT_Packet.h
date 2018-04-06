#ifndef BT_PACKET_H
#define BT_PACKET_H


class BT_Packet {
    public:
        void setGPS(uint8_t lat, uint8_t lng);
        bool updatesPending();
        uint32_t getPacket();
    protected:
        uint16_t UUID;
        uint8_t lat;
        uint8_t lon;
        bool updatePending;
};


#endif