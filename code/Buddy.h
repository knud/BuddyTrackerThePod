#ifndef BUDDY_H
#define BUDDY_H


class Buddy {
    public:
        Buddy(void);
        
        uint16_t getUUID(void);
        uint8_t getLat(void);
        uint8_t getLng(void);
    private:
        uint16_t UUID;
        uint8_t lat;
        uint8_t lng;
};


#endif