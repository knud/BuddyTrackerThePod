#include "Buddy.h"

Buddy::Buddy();

uint16_t Buddy::getUUID(){
    return UUID;
}

uint8_t Buddy::getLat(){
    return lat;
}

uint8_t Buddy::getLng(){
    return lng;
}
