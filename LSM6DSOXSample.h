#ifndef LSM6DSOX_SAMPLE
#define LSM6DSOX_SAMPLE
#include <cstdint>
#include <stdlib.h>
#include <string>
#include "LSM6DSOX_Registers.h"

struct RawData
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;
};

struct LSM6DSOXSample
{
    /**
    * Acceleration on x axis (m^2/s)
    */
    float ax = 0;
    /**
    * Acceleration on y axis (m^2/s)
    */
    float ay = 0;
    /**
    * Acceleration on z axis (m^2/s)
    */
    float az = 0;
    /**
    * Rotation on x axis (deg/s)
    */
    float gx = 0;

    /**
    * Rotation on y axis (deg/s)
    */
    float gy = 0;

    /**
    * Rotation on z axis (deg/s)
    */
    float gz = 0;

    float temperature = 0;

    bool valid = true;
};

struct LSMSettings
{
    std::string device = "/dev/i2c-1";

    uint8_t address = LSM6DSOX_ADDRESS;

    unsigned int LMS6DSOX_DRDY_GPIO
        = 17; // RPi physical pin 15, connected to data ready pin on LSM

    std::string DRDY_CHIP = "/dev/gpiochip0";

    /**
    * @brief Linear acceleration sensitivity
    */
    enum XL_SCALE
    {
        XL_2G = 2,
        XL_4G = 4,
        XL_8G = 8,
        XL_16G = 16
    };

    /**
    * @brief Possible output data rate (ODR)
    */
    enum ODR : uint8_t
    {
        ODR_12_5 = 0x01, //12.5
        ODR_26 = 0x02,   //26 Hz
        ODR_52 = 0x03,   //52 Hz
        ODR_104 = 0x04,  //104 Hz
        ODR_208 = 0x05,  //208 Hz
        ODR_416 = 0x06,  //416 Hz
        ODR_833 = 0x07,  //833 Hz
        ODR_1666 = 0x08, //1666 Hz
        ODR_3332 = 0x09, //3332 Hz
        ODR_6664 = 0x0A  //6664 Hz
    };

    enum XL_POWER_MODES
    {
        XL_POWER_DOWN,
        XL_ULTRALOW,
        XL_LOW,
        XL_NORMAL,
        XL_HIGH_PERF
    };

    /**
    * @brief Requested output data rate also called Sampling rate!
    */
    ODR samplingRate = ODR_52;

    XL_SCALE xl_scale = XL_2G;

    /**
    *@brief Angular rate sensitivity
    */
    enum GYRO_SCALE
    {
        GYRO_125_DPS = 125,
        GYRO_250_DPS = 250,
        GYRO_500_DPS = 500,
        GYRO_1000_DPS = 1000,
        GYRO_2000_DPS = 2000
    };

    /**
     * Default full scale of the Gyro.
     */
    GYRO_SCALE gyro_scale = GYRO_125_DPS;

    /**
     * If self calibration of the offset should be done at startup.
     */
    bool gyroCalibration = true;
};

#endif
