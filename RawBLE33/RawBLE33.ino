/*  
 *   RawBLE33 - Read Raw data of Accelerometer, Gyroscope, and Magnetometer

  This example reads the acceleration, gyroscope, and magnetometer values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 6 June 2021
  by Aditya Tejwani

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#define BLE33_SAMPLERATE_DELAY_MS (1000)

float x_acc, y_acc, z_acc, x_gyr, y_gyr, z_gyr, x_mag, y_mag, z_mag, scale_fac = 9.8;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  delay(BLE33_SAMPLERATE_DELAY_MS);
  
  Serial.print("Acceleration Sample Rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope Sample Rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" uT");

  Serial.println();
  Serial.println("\t\t    X\t\t\tY\t\tZ");
  Serial.println("-------------------------------------------------------------------");
}

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_acc, y_acc, z_acc);
    Serial.print("Accelerometer data: x = " + String(x_acc * scale_fac, 3) + "\t\t" + "y = " + String(y_acc * scale_fac, 3) + '\t' + "z = " + String(z_acc * scale_fac, 3) + '\n');
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x_gyr, y_gyr, z_gyr);
    Serial.print("Gyroscope data:     x = " + String(x_gyr, 3) + "\t\t" + "y = " + String(y_gyr, 3) + '\t' + "z = " + String(z_gyr, 3) + '\n');
  }
  
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x_mag, y_mag, z_mag);
    Serial.print("Magnetometer data:  x = " + String(x_mag, 3) + "\t\t" + "y = " + String(y_mag, 3) + '\t' + "z = " + String(z_mag, 3) + '\n');
  } 
delay(100);
}
