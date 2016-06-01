// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
void InitI2C() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize device
    accelgyro.initialize();

    // verify connection
    accelgyro.testConnection();
}

//void GetAngle() {
//    // read raw accel/gyro measurements from device
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//
//    // these methods (and a few others) are also available
//    //accelgyro.getAcceleration(&ax, &ay, &az);
//    //accelgyro.getRotation(&gx, &gy, &gz);
//    roll = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * 180 / 3.141592;
//    pitch = -atan2(ax, sqrt(pow(ay, 2) + pow(az, 2))) * 180 / 3.141592;
//    gx = gx / 131;
//    gy = gy / 131;
//    gz = gz / 131;
//
//    // display tab-separated accel/gyro x/y/z values
//    Serial.print("roll, pitch:\t");
//    Serial.print(roll); Serial.print("\t");
//    Serial.print(pitch); Serial.print("\t");
//    Serial.print(az); Serial.print("\t");
//    Serial.print(gx); Serial.print("\t");
//    Serial.print(gy); Serial.print("\t");
//    Serial.println(gz);
//
//    // blink LED to indicate activity
//    blinkState = !blinkState;
//    digitalWrite(LED_PIN, blinkState);
//}
