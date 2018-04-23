#ifndef BUDDY_H
#define BUDDY_H


class Buddy {
    public:
        Buddy();
        Buddy(uint64_t UUID);
        
        uint64_t getUUID(void);
        uint32_t getLat(void);
        uint32_t getLng(void);
        void setLat(uint32_t lat);
        void setLng(uint32_t lng);
    private:
        uint64_t UUID;
        uint32_t lat;
        uint32_t lng;
};


#endif