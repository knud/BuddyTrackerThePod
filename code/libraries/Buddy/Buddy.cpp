#include <Arduino.h>
#include "Buddy.h"

Buddy::Buddy(void);

uint16_t Buddy::getUUID(void){
    return UUID;
}

uint32_t Buddy::getLat(void){
    return lat;
}

uint32_t Buddy::getLng(void){
    return lng;
}
