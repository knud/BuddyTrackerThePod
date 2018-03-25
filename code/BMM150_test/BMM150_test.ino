#include "bmm150.h"
#include <Wire.h>


int8_t set_sensor_settings(struct bmm150_dev *dev);
int8_t read_sensor_data(struct bmm150_dev *dev);
int8_t user_i2c_read(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t len);
int8_t user_i2c_write(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t len);
void delay_ms(uint32_t period);


struct bmm150_dev dev;
int8_t rslt = BMM150_OK;


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Wire.begin();

    /* Sensor interface over I2C */
    dev.dev_id = BMM150_DEFAULT_I2C_ADDRESS;
    dev.intf = BMM150_I2C_INTF;
    //dev.read = user_i2c_read;
    //dev.write = user_i2c_write;
    //dev.delay_ms = user_delay_ms;
    dev.read = &user_i2c_read;
    dev.write = &user_i2c_write;
    dev.delay_ms = &delay_ms;

    rslt = bmm150_init(&dev);

    set_sensor_settings(&dev);
}


void loop() {
    // put your main code here, to run repeatedly:
    read_sensor_data(&dev);
    delay(1000);
}


/*//bmm150_com_fptr_t user_i2c_read(){
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len){
    Wire.requestFrom(dev_id, len);
    for(uint8_t i = 0; i < len; i++){
        if(!Wire.available())
            return -1;
        *read_data = Wire.read();
        read_data++;
    }
    return BMM150_OK;
}


int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *write_data, uint16_t len){
    Wire.beginTransmission(dev_id);
    Wire.write(write_data, len);
    Wire.endTransmission();
    return BMM150_OK;
}*/


// https://gist.github.com/BoschSensortec/cdaa6aa244335388d914de7f59443860


/**
* @brief Task that delays for a ms period of time
* @param period    : Period of time in ms
*/
void delay_ms(uint32_t period){
    // Wait for a period amount of ms
    // The system may simply idle, sleep or even perform background tasks
    delay(period);
}


/**
* @brief Callback function for reading registers over I2C
* @param devId        : Library agnostic parameter to identify the device to communicate with
* @param regAddr    : Register address
* @param regData    : Pointer to the array containing the data to be read
* @param length : Length of the array of data
* @return   Zero for success, non-zero otherwise
*/
int8_t user_i2c_read(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t len){
    uint16_t i;
    int8_t rslt = 0;

    Wire.beginTransmission(devId);
    Wire.write(regAddr);
    rslt = Wire.endTransmission();
    
    Wire.requestFrom((int) devId, (int) len);
    for (i = 0; (i < len) && Wire.available(); i++) {
        regData[i] = Wire.read();
    }

    return rslt;
}

/**
* @brief Callback function for writing registers over I2C
* @param devId      : Library agnostic parameter to identify the device to communicate with
* @param regAddr    : Register address
* @param regData    : Pointer to the array containing the data to be written
* @param length : Length of the array of data
* @return   Zero for success, non-zero otherwise
*/
int8_t user_i2c_write(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t len){
    uint16_t i;
    int8_t rslt = 0;

    Wire.beginTransmission(devId);
    Wire.write(regAddr);
    for (i = 0; i < len; i++) {
        Wire.write(regData[i]);
    }
    rslt = Wire.endTransmission();

    return rslt;
}


int8_t set_sensor_settings(struct bmm150_dev *dev){
    int8_t rslt;

    /* Setting the power mode as normal */
    dev->settings.pwr_mode = BMM150_NORMAL_MODE;
    rslt = bmm150_set_op_mode(dev);
    
    /* Setting the preset mode as Low power mode 
    i.e. data rate = 10Hz XY-rep = 1 Z-rep = 2*/
    dev->settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
    rslt = bmm150_set_presetmode(dev);
    
    return rslt;    
}


int8_t read_sensor_data(struct bmm150_dev *dev){
    int8_t rslt;

    /* Mag data for X,Y,Z axis are stored inside the
    bmm150_dev structure in int16_t format */
    rslt = bmm150_read_mag_data(dev);

    /* Print the Mag data */
    Serial.println("\nMagnetometer data");
    //String outputString = "MAG X : " + dev->data.x + " \t MAG Y : " + dev->data.y + " \t MAG Z : " + dev->data.z;
    String outputString = "MAG X : ";
    outputString = outputString + dev->data.x;
    outputString = outputString + " \t MAG Y : ";
    outputString = outputString + dev->data.y;
    outputString = outputString + " \t MAG Z : ";
    outputString = outputString + dev->data.z;
    //Serial.println("MAG X : " + dev->data.x + " \t MAG Y : " + dev->data.y + " \t MAG Z : " + dev->data.z);
    Serial.println(outputString);
    
    return rslt;
}
