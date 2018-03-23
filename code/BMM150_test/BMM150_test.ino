#include "bmm150.h"
#include <Wire.h>


int8_t set_sensor_settings(struct bmm150_dev *dev);
int8_t read_sensor_data(struct bmm150_dev *dev);
bmm150_com_fptr_t user_i2c_read();
bmm150_com_fptr_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len);


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
    dev.delay_ms = &delay;
    //dev.read = &TwoWire::read;

    rslt = bmm150_init(&dev);

    set_sensor_settings(&dev);
}


void loop() {
    // put your main code here, to run repeatedly:
    read_sensor_data(&dev);
    delay(1000);
}


bmm150_com_fptr_t user_i2c_read(){
    return BMM150_OK;
}


bmm150_com_fptr_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len){
    return BMM150_OK;
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
    outputString = outputString + "MAG X : ";
    outputString = outputString + dev->data.x;
    outputString = outputString + " \t MAG Y : ";
    outputString = outputString + dev->data.y;
    outputString = outputString + " \t MAG Z : ";
    outputString = outputString + dev->data.z;
    //Serial.println("MAG X : " + dev->data.x + " \t MAG Y : " + dev->data.y + " \t MAG Z : " + dev->data.z);
    Serial.println(outputString);
    
    return rslt;
}
