

#include "mpu6050.h"

//void i2c_setup(i2c_inst_t* I2C_ID, uint pI2C_SDA_PIN, uint pI2C_SCL_PIN) {
void i2c_setup(i2c_inst_t* I2C_ID) {
    // sets up i2c IF in RP Pico
    i2c_init(I2C_ID, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

}


uint8_t readI2CByte(i2c_inst_t* I2C_ID, uint8_t subAddress){
    // reads 1 Byte
    uint8_t data1B;
    i2c_write_blocking(I2C_ID, addr, &subAddress, 1, true);
    i2c_read_blocking(I2C_ID, addr, &data1B, 1, false);
    return data1B;
}

void mpu6050_selftest(i2c_inst_t* I2C_ID, float destination[6]) 
// Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
   uint8_t rawData[4];
   uint8_t selfTest[6];
   float factoryTrim[6];
   uint8_t dst2[2];

   // Configure the accelerometer for self-test
   // 0xF0 = 11110000
   dst2[0] = ACCEL_CONFIG_REG;
   dst2[1] = 0xF0; // 0b_111_10_000
   i2c_write_blocking(I2C_ID, addr, dst2, 2, true); // Enable self test on all three axes and set accelerometer range to +/- 8 g
   // 0xE0 = 11100000
   dst2[0] = GYRO_CONFIG_REG;
   dst2[1] = 0xE0;
   i2c_write_blocking(I2C_ID, addr, dst2, 2, true); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
   sleep_ms(250); // Delay a while to let the device execute the self-test

   rawData[0] = readI2CByte(I2C_ID, SELF_TEST_X); // X-axis self-test results
   rawData[1] = readI2CByte(I2C_ID, SELF_TEST_Y); // Y-axis self-test results
   rawData[2] = readI2CByte(I2C_ID, SELF_TEST_Z); // Z-axis self-test results
   rawData[3] = readI2CByte(I2C_ID, SELF_TEST_A); // Mixed-axis self-test results
   // Extract the acceleration test results first
   selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4 ; // XA_TEST result is a five-bit unsigned integer
   selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 4 ; // YA_TEST result is a five-bit unsigned integer
   selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 4 ; // ZA_TEST result is a five-bit unsigned integer
   // Extract the gyration test results first
   selfTest[3] = rawData[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer, bits 5-7 get nulled with "& 0x1F"
   selfTest[4] = rawData[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer, bits 5-7 get nulled ...
   selfTest[5] = rawData[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer, bits 5-7 get nulled ...
   // Process results to allow final comparison with factory set values
   factoryTrim[0] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[0] - 1.0)/30.0))); // FT[Xa] factory trim calculation
   factoryTrim[1] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[1] - 1.0)/30.0))); // FT[Ya] factory trim calculation
   factoryTrim[2] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[2] - 1.0)/30.0))); // FT[Za] factory trim calculation
   factoryTrim[3] =  ( 25.0*131.0)*(pow( 1.046 , ((float)selfTest[3] - 1.0) ));             // FT[Xg] factory trim calculation
   factoryTrim[4] =  (-25.0*131.0)*(pow( 1.046 , ((float)selfTest[4] - 1.0) ));             // FT[Yg] factory trim calculation
   factoryTrim[5] =  ( 25.0*131.0)*(pow( 1.046 , ((float)selfTest[5] - 1.0) ));             // FT[Zg] factory trim calculation
   
 //  Output self-test results and factory trim calculation if desired
 //  Serial.println(selfTest[0]); Serial.println(selfTest[1]); Serial.println(selfTest[2]);
 //  Serial.println(selfTest[3]); Serial.println(selfTest[4]); Serial.println(selfTest[5]);
 //  Serial.println(factoryTrim[0]); Serial.println(factoryTrim[1]); Serial.println(factoryTrim[2]);
 //  Serial.println(factoryTrim[3]); Serial.println(factoryTrim[4]); Serial.println(factoryTrim[5]);

 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get to percent, must multiply by 100 and subtract result from 100
   for (int i = 0; i < 6; i++) {
     destination[i] = 100.0 + 100.0*((float)selfTest[i] - factoryTrim[i])/factoryTrim[i]; // Report percent differences
   }
   
}

void mpu6050_reset(i2c_inst_t* I2C_ID) {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x00}; // 107, 0
    i2c_write_blocking(I2C_ID, addr, buf, 2, false);

    // configure accel.
    uint8_t maskacc = AFS_SEL << 3; // 0b0000XX000
    uint8_t acc[] = {ACCEL_CONFIG_REG, maskacc};  
    i2c_write_blocking(I2C_ID, addr, acc, 2, false); // true to keep master control of bus
    
    // configure gyro
    uint8_t maskgyro = FS_SEL << 3; // 0b0000XX000
    uint8_t gyro[] = {GYRO_CONFIG_REG, maskgyro};  
    i2c_write_blocking(I2C_ID, addr, gyro, 2, false); // true to keep master control of bus

}

void mpu6050_read_raw(i2c_inst_t* I2C_ID, int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    //uint8_t acc[] = {ACCEL_CONFIG_REG, 0b00011000}; // AFS_SEL = 3, +-16 g
    //i2c_write_blocking(I2C_ID, addr, acc, 1, true); // true to keep master control of bus

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B; // 59
    i2c_write_blocking(I2C_ID, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_ID, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43; // 67
    i2c_write_blocking(I2C_ID, addr, &val, 1, true);
    i2c_read_blocking(I2C_ID, addr, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41; // 65
    i2c_write_blocking(I2C_ID, addr, &val, 1, true);
    i2c_read_blocking(I2C_ID, addr, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050_read_cal(i2c_inst_t* I2C_ID, float accel[3], float gyro[3], float *temp) {
    // reads calibrated values of ACC and GYRO
    int16_t rawaccel[3], rawgyro[3], rawtemp;

    mpu6050_read_raw(I2C_ID, rawaccel, rawgyro, &rawtemp);

    // gyro

    float scalegyro = 131.0 / (float)pow(2,FS_SEL);

    for (int i = 0; i < 3; i++) {
        gyro[i] = (float)rawgyro[i]/scalegyro; // deg / s 
    }

    // accel.


    float scaleacc = (float)pow(2,14-AFS_SEL);

    for (int i = 0; i < 3; i++) {
        accel[i] = (float)rawaccel[i]/scaleacc; // g 
    }


    // temperature
    *temp = (float) (rawtemp / 340.0) + 36.53;

}