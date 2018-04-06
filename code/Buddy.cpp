#include <Arduino.h>
#include "Buddy.h"

Buddy::Buddy(void);

uint16_t Buddy::getUUID(void){
    return UUID;
}

uint8_t Buddy::getLat(void){
    return lat;
}

uint8_t Buddy::getLng(void){
    return lng;
}
