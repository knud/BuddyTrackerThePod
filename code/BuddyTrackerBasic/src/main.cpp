#include <Adafruit_GPS.h>
#include <Arduino.h>
#include <LinkedList.h>
#include <LoRa.h>
#include <SPI.h>
#include "BT_Packet.h"
#include "BT_UI.h"
#include "Buddy.h"


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false


void onReceive(uint8_t packetSize);
void sendPacket(BT_Packet packet);
void updateBuddy(uint64_t UUID, uint16_t lat, uint16_t lng);
void startGPS();
void handleGPS();
uint8_t findBuddyIndex(uint64_t UUID);


// TODO: define comparator for sorting LinkedList
LinkedList<Buddy*> buddies;

uint32_t timer = millis();
const uint32_t LAT_LNG_ERR = -1;
uint32_t myLat = LAT_LNG_ERR;
uint32_t myLng = LAT_LNG_ERR;
Uart GPSSerial (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
Adafruit_GPS GPS(&GPSSerial);


void setup() {
    Serial.begin(9600);
    startGPS();
    while (!Serial);

    Serial.println("BuddyTracker");

    LoRa.setPins(8, 4, 3);
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}


void loop() {
    // TODO: collision avoidance
    
    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
}


void onReceive(uint8_t packetSize) {
    if (packetSize == 0) return; // if there's no packet, return

    while (LoRa.available()) {
        // read UUID
        uint64_t UUID = 0;
        UUID |= LoRa.read() << (0 * 8);
        UUID |= LoRa.read() << (1 * 8);
        UUID |= LoRa.read() << (2 * 8);
        UUID |= LoRa.read() << (3 * 8);
        UUID |= LoRa.read() << (4 * 8);
        UUID |= LoRa.read() << (5 * 8);
        UUID |= LoRa.read() << (6 * 8);
        UUID |= LoRa.read() << (7 * 8);

        // read partial lat
        uint16_t lat_partial = 0;
        lat_partial |= LoRa.read() << (0 * 8);
        lat_partial |= LoRa.read() << (1 * 8);

        // read partial lat
        uint16_t lng_partial = 0;
        lng_partial |= LoRa.read() << (0 * 8);
        lng_partial |= LoRa.read() << (1 * 8);

        // save data
        updateBuddy(UUID, lat_partial, lng_partial);
    }
}


void sendPacket(BT_Packet packet){
    LoRa.beginPacket();
    byte *packetContents = packet.getPacket();
    for(int i = 0; i < PACKET_LENGTH; i++){
        LoRa.write( *(packetContents + i) );
    }
    LoRa.endPacket();
}


// currently adds unknown buddies
// shows everyone
void updateBuddy(uint64_t UUID, uint16_t lat_partial, uint16_t lng_partial){
    uint8_t index = findBuddyIndex(UUID);
    
    // add unknown buddies
    if(index == -1){
        Buddy *newBuddy = new Buddy(UUID);
        buddies.add(newBuddy);
        index = buddies.size() - 1;
    }

    // can't compute other Buddy's location without knowning own location
    if(myLat == LAT_LNG_ERR || myLng == LAT_LNG_ERR){
        return;
    }
    // clear LSBs
    uint32_t lat = myLat & 0x00;
    uint32_t lng = myLng & 0x00;
    // replace LSBs
    lat &= lat_partial;
    lng &= lng_partial;

    Buddy *currentBuddy = buddies.get(index);
    currentBuddy->setLat(lat);
    currentBuddy->setLng(lng);

}


void startGPS(){
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz
        
    // Request updates on antenna status, comment out to keep quiet
    GPS.sendCommand(PGCMD_ANTENNA);

    delay(1000);
    
    // Ask for firmware version
    GPSSerial.println(PMTK_Q_RELEASE);
}


// TODO: this is the default Adafruit code, modify to actually update values
void handleGPS(){
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
        if (c) Serial.print(c);
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
        Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
    }
    // if millis() or timer wraps around, we'll just reset it
    if (timer > millis()) timer = millis();
        
    // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) {
        timer = millis(); // reset the timer
        Serial.print("\nTime: ");
        Serial.print(GPS.hour, DEC); Serial.print(':');
        Serial.print(GPS.minute, DEC); Serial.print(':');
        Serial.print(GPS.seconds, DEC); Serial.print('.');
        Serial.println(GPS.milliseconds);
        Serial.print("Date: ");
        Serial.print(GPS.day, DEC); Serial.print('/');
        Serial.print(GPS.month, DEC); Serial.print("/20");
        Serial.println(GPS.year, DEC);
        Serial.print("Fix: "); Serial.print((int)GPS.fix);
        Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
        if (GPS.fix) {
            Serial.print("Location: ");
            Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
            Serial.print(", ");
            Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
            Serial.print("Speed (knots): "); Serial.println(GPS.speed);
            Serial.print("Angle: "); Serial.println(GPS.angle);
            Serial.print("Altitude: "); Serial.println(GPS.altitude);
            Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
        }
    }
}


// returns 0 if no match
uint8_t findBuddyIndex(uint64_t UUID){
    for(uint8_t i = 0; i < buddies.size(); i++){
        Buddy *currentBuddy = buddies.get(i);
        if(UUID == currentBuddy->getUUID()){
            return i;
        }
    }
    return 0;
}