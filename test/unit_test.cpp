#include "../LSM6DSOX.h"
#include "LSM6DSOXSample.h"
#include <gtest/gtest.h>
const bool DEBUG = true;

class LSM6DSOXTest : public LSM6DSOX,
                     public testing::Test,
                     public testing::WithParamInterface<uint8_t>
{
};

/**
* @brief Checks if device is LSM6DSOX 
*/
TEST_F (LSM6DSOXTest, whoAmI)
{
    uint8_t b = i2cReadByte (LSM6DSOX_WHO_AM_I_REG);
    EXPECT_EQ (b, (LSM6DSOX_WHO_AM_I_VALUE));
}

/**
* @brief Tests if data interrupt pin 1 opens and closes correctly
*/
TEST_F (LSM6DSOXTest, INT1Open)
{
    int fd = i2cOpen (LSM6DSOX_INT1_CTRL);
    EXPECT_GE (fd, 0);
    close (fd);
}

/**
*@brief Tests what we (me and the people living in the walls) are writing to int1 register
*/
TEST_F (LSM6DSOXTest, INT1Write)
{
    uint8_t data = LSM6DSOX_GYRO_NC | LSM6DSOX_XL_NC;
    i2cWriteByte (LSM6DSOX_INT1_CTRL, data);
    EXPECT_EQ (i2cReadByte (LSM6DSOX_INT1_CTRL), 0x03);
}

/**
* @brief Test that the correct bits have been written to accelerometer register 
*/
TEST_F (LSM6DSOXTest, initAccelerometer)
{
    initAccelerometer ();
    EXPECT_EQ (i2cReadByte (LSM6DSOX_CTRL1_XL) >> 5,
               1); //[0][1][0][0][0][0][0][0] =64
}

/**
* @brief Test that the correct bits have been written to gyroscope register 
*/
TEST_F (LSM6DSOXTest, initGyro)
{
    initGyro ();
    EXPECT_EQ (i2cReadByte (LSM6DSOX_CTRL2_G) >> 5,
               1); //[0][1][0][0][0][0][0][0] =64
}

/**
* @brief having an issue with not detecting rising edges in int1 pin so this reads xl data available
*/
TEST_F (LSM6DSOXTest, readAccelerometer)
{
    RawData ad;
    ad = readAccelerometer ();
    EXPECT_NE (ad.x, 0);
    EXPECT_NE (ad.y, 0);
    EXPECT_NE (ad.z, 0);
    EXPECT_NE (ad.x, 999);
    EXPECT_NE (ad.y, 999);
    EXPECT_NE (ad.z, 999);
    EXPECT_NE (i2cReadByte (LSM6DSOX_STATUS_REG), 0X01);
}

/**
* @brief having an issue with not detecting rising edges in int1 pin so this reads xl data available
*/
TEST_F (LSM6DSOXTest, readGyroscope)
{
    RawData gd;
    gd = readGyroscope ();
    EXPECT_NE (gd.x, 0);
    EXPECT_NE (gd.y, 0);
    EXPECT_NE (gd.z, 0);
    EXPECT_NE (gd.x, 999);
    EXPECT_NE (gd.y, 999);
    EXPECT_NE (gd.z, 999);
    EXPECT_NE (i2cReadByte (LSM6DSOX_STATUS_REG), 0X02);
}

TEST_P (LSM6DSOXTest, flushData)
{
    EXPECT_EQ (flushData (GetParam ()), GetParam ());
}

INSTANTIATE_TEST_CASE_P (Codes, LSM6DSOXTest,
                         ::testing::Values (uint8_t (0), uint8_t (1),
                                            uint8_t (2), uint8_t (3)));

//TESTING XL SCALE
//fs     //scale  //expected
class LSM6DSOXXLScaleTest
    : public LSM6DSOX,
      public testing::Test,
      public testing::WithParamInterface<
          std::tuple<uint8_t, LSMSettings::XL_SCALE, uint8_t> >
{
};

TEST (ReceivingDataTest, datatest)
{
    LSM6DSOXSample sample;
    sample.valid = false;
    LSM6DSOX lsm6dS0x;
    lsm6dS0x.registerCallback ([&] (const LSM6DSOXSample &s) { sample = s; });
    LSMSettings lsmsettings;
    lsmsettings.samplingRate = LSMSettings::ODR_12_5;
    lsmsettings.gyroCalibration = true;
    try
    {
        lsm6dS0x.start (lsmsettings);
    }
    catch (const char *msg)
    {
        ADD_FAILURE () << "Unexpected exception: " << msg;
    }
    fprintf (stderr, "Successfully started.");
    sleep (1);
    EXPECT_TRUE (sample.valid);
    lsm6dS0x.stop ();
}
