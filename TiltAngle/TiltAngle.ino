/*
  TiltAngle - Measures tilt angles theta, phi, and psi.

  This program reads the acceleration values from the LSM9DS1
  sensor and calculates the tilt angles theta, phi, and psi, and continuously prints them to the Serial Monitor
  or Serial Plotter. The angle theta represents the pitch, phi represents the roll, and psi represents the yaw.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 1 Jul 2021
  by Aditya Tejwani

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>
#include <math.h>
#define BLE33_SAMPLERATE_DELAY_MS (1000)

double theta, phi, psi, theta_deg, phi_deg, psi_deg, Bx2, By2, Bz2, alpha = 0.01;
float x_acc, y_acc, z_acc, x_mag, y_mag, z_mag;

BLEService sensorService("8401a9ba-57bb-11ed-9b6a-0242ac120002");  // User defined service

BLEStringCharacteristic sensorCharacteristic("8401ac80-57bb-11ed-9b6a-0242ac120002",  // standard 16-bit characteristic UUID
    BLERead, 13); // remote clients will only be able to read this

void setup() {
  Serial.begin(9600);
  
  if (!BLE.begin()) {   // initialize BLE
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Nano33BLE");  // Set name for connection
  BLE.setAdvertisedService(sensorService); // Advertise service
  sensorService.addCharacteristic(sensorCharacteristic); // Add characteristic to service
  BLE.addService(sensorService); // Add service

  BLE.advertise();  // Start advertising
  Serial.print("Peripheral device MAC: ");
  Serial.println(BLE.address());
  Serial.println("Waiting for connections...");
  
  // Initialize the IMU
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {

BLEDevice central = BLE.central();  // Wait for a BLE central to connect

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central MAC: ");
    // print the central's BT address:
    Serial.println(central.address());
    
  // Read the x_acc, y_acc, and z_acc axis acceleration data
 
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_acc, y_acc, z_acc);
  }

  // Read the x_mag, y_mag, and z_mag axis magnetometer data
  
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x_mag, y_mag, z_mag);
  }
  
  // Calculate the theta (pitch), phi (roll) tilt angles in degrees

  theta = atan2(x_acc, z_acc);
  theta_deg = theta*180.0/PI;
  
  // If theta is equal to 90 degrees or -90 degrees, then y_acc and z_acc 
  // are near 0 and phi is unstable; to make phi stable z_acc has been substituted by z_acc + x_acc * alpha. alpha can have values in the range 0.01-0.05.
  
  if (theta == -PI/2 || theta == PI/2){
    z_acc = z_acc + x_acc * alpha;
  }
  
  phi = atan2(y_acc, z_acc);
  phi_deg = phi*180.0/PI;

  // Calculate the psi (yaw) tilt angle in degrees. psi becomes unstable near 180 degrees and -180 degrees.
  
  By2 = (z_mag*sin(phi) - y_mag*cos(phi));
  Bz2 = (y_mag*sin(phi) + z_mag*cos(phi));
  Bx2 = (x_mag*cos(theta) + Bz2*sin(theta));
  psi = atan2(By2,Bx2);
  psi_deg = psi*180.0/PI;

  // Print the theta, phi, and psi tilt angles

//  Serial.print("Pitch: ");
  Serial.print(theta_deg);
  Serial.print(',');
  Serial.print(phi_deg);
  Serial.print(',');
//  Serial.print("Yaw: ");
  Serial.println(psi_deg);

  sensorCharacteristic.writeValue(String(theta_deg));
  
  delay(BLE33_SAMPLERATE_DELAY_MS);
  }
}
