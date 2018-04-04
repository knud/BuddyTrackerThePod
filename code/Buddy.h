#ifndef BUDDY_H
#define BUDDY_H


class Buddy {
    public:
        uint16_t getUUID();
        uint8_t getLat();
        uint8_t getLng();
    private:
        uint16_t UUID;
        uint8_t lat;
        uint8_t lng;
};


#endif