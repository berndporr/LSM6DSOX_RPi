# LSM6DSOX for Raspberry Pi 5
C++ implementation of real-time reading from a LSM6DSOX IMU.

## Requirements

Raspberry Pi 5 with Debian Trixie OS, gpiod library version 2

`sudo apt-get install libgpiod cmake libgtest-dev libi2c-dev`

## Connect Pi 5 to LSM6DSOX
* GPIO17 (physical pin 11 on Pi) ->I1 (data ready pin 1)
* 3V (physical pin 1 on Pi) -> VDD
* GND -> GND
* SDA (physical pin 3) -> SDA
* SCL (physical pin 5) -> SCL

## Compile
`cmake .`

`make`

`sudo make install` : install library

## Test
`ctest` : run unit tests. Also tests if the gyro reports data.

## Example

Go to `demo`. There is `sample_printer` which prints the acceleration and gyro data on the screen till you press enter.

## Find the I2C address 

Install the i2c tools:
```
sudo apt install i2c-tools
```

Detect it:
```
i2cdetect -y 1
```
You should see a `6a`:

```
i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- 6a -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
```
