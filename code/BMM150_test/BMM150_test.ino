#include "bmm150.h"
#include <Wire.h>


int8_t set_sensor_settings(struct bmm150_dev *dev);
int8_t read_sensor_data(struct bmm150_dev *dev);


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
    //dev.read = &TwoWire::read;
    dev.read = &read_sensor_data;
    dev.write = &TwoWire::write;
    dev.delay_ms = 100;

    rslt = bmm150_init(&dev);

    set_sensor_settings(&dev);
}


void loop() {
    // put your main code here, to run repeatedly:
    read_sensor_data(&dev);
    delay(1000);
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
