#ifndef BUDDY_H
#define BUDDY_H


class Buddy {
    public:
        Buddy(uint16_t UUID);
        
        uint16_t getUUID(void);
        uint32_t getLat(void);
        uint32_t getLng(void);
        void setLat(uint32_t lat);
        void setLng(uint32_t lng);
    private:
        uint16_t UUID;
        uint32_t lat;
        uint32_t lng;
};


#endif