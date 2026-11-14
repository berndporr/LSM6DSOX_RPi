#include "LSM6DSOXSample.h"
#include <fcntl.h>
#include <functional>
#include <gpiod.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#ifndef LSM6DSOX_H
#define LSM6DSOX_H

/**
 * @brief LSM6DSOX driver class.
 * Main class performing the acquisition and then calling the
 * callback at the sampling rate.
 */
class LSM6DSOX
{
  public:
    LSM6DSOX () = default;

    ~LSM6DSOX () {}

    /**
     * @brief Callback prototype carrying one sample.
     */
    using LSM6DSOXCallback = std::function<void (const LSM6DSOXSample &)>;

    /**
     * @brief Max permitted gyro error. 
     * 0.017 it's according to the datasheet (1 degrees) so using ~5 times higher.
     */
    constexpr static float maxGyroOffset = 0.087;

    /**
     * @brief Starts the LSM6DSOX data acquisition.
     * Opens gpio chip, sets gpio settings, line config, request config and
     * requests, starts the event loop. It throws an exception if there are any problems.
     */
    void start (LSMSettings settings = LSMSettings ());

    /**
     * @brief Stops the LSM6DSOX data acquisition.
     * Terminates thread, releases lines.
     */
    void stop ();

    /**
     * @brief Registers the callback
     */
    void registerCallback (LSM6DSOXCallback cb);

    /**
     * Gets the sampling rate in Hz
     **/
    float getODR () const
    {
        switch (lsmsettings.samplingRate)
        {
        case LSMSettings::ODR_12_5:
            return 12.5;
        case LSMSettings::ODR_26:
            return 26;
        case LSMSettings::ODR_52:
            return 52;
        case LSMSettings::ODR_104:
            return 104;
        case LSMSettings::ODR_208:
            return 208;
        case LSMSettings::ODR_416:
            return 416;
        case LSMSettings::ODR_833:
            return 833;
        case LSMSettings::ODR_1666:
            return 1666;
        case LSMSettings::ODR_3332:
            return 3332;
        case LSMSettings::ODR_6664:
            return 6664;
        }
        return 0;
    }

  protected:
    std::thread thread;
    LSMSettings lsmsettings{};
    friend class LSM6DSOXTest; // for unit testing
    uint8_t running = false;
    LSM6DSOXCallback callback;
    float xlRes; // accelerometer resolution
    float gRes;  // gyroscope resolution
    RawData gyroOffset; // gyro offset
    bool gyroCalibrationFlag = false; // true means it's calibrating
    int nGyroCalibrationReadings = 0; // number of calibration readings

    struct gpiod_chip *chipDRDY = nullptr; // chip where the data ready pin is
    struct gpiod_line_settings *settings = nullptr;
    struct gpiod_line_config *line_cfg = nullptr;
    struct gpiod_request_config *req_cfg = nullptr;
    struct gpiod_line_request *request = nullptr;

    bool isxlFSHigh ();

    void worker ();

    void getData ();

    RawData readGyroscope ();

    RawData readAccelerometer ();

    /**
     * @brief reads bytes contiguously from the given address
     * @param address starting address
     * @param container array to which values will be written
     * @param nBytes number of bytes to read
     */
    void contiguousReadBytes (uint8_t address, uint8_t *container,
                              uint8_t nBytes);

    /**
     * @brief Opens i2c device at a certain @param address. Throws exceptions
     * Address is the chip address!!!
     */
    int i2cOpen (int address);

    /**
     * @brief Writes one byte of @param data from register at @param address
     */
    void i2cWriteByte (uint8_t address, uint8_t data);

    /**
     * @brief Reads one byte from register at @param address
     */
    uint8_t i2cReadByte (uint8_t address);

    /**
     * @brief Enable gyroscope, set sampling rate and data scale from gyroSettings
     */
    void initGyro ();

    /**
     * @brief Enable accelerometer, set sampling rate and data scale from
     * xlSettings
     */
    void initAccelerometer ();

    /**
     * @brief throws a different exception based on what's in the status register
     */
    const char *throwStatus (uint8_t statusByte);

    /**
     * @brief Bit of a hack, but uses status register readings to flush any
     * existing data and allowing interrupt pin to go low for the next reading
     */
    uint8_t flushData (uint8_t statusByte);

    /**
     *@brief Gyroscope resolution (16 bit)
     */
    float getGRes ();

    /**
     * @brief Accelerometer resolution (16 bit)
     */
    float getXlRes ();

    /**
     * @brief Get bits to write to gyro register CTRL2_G to set scale. Shifts bits
     * automatically so they can directly be written to register
     */
    uint8_t gyroScaleBits ();

    /**
     * @brief Get bits to write to accelerometer register CTRL1_XL to set scale.
     * Shifts bits automatically so they can directly be written to register
     */
    uint8_t xlScaleBits ();
};
#endif
