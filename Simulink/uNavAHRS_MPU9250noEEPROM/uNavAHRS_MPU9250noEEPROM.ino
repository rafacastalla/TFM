/*
uNavAHRS_MPU9250.ino
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "uNavAHRS.h"
#include "MPU9250.h"

float AccelBiasX =  0.0000;
float AccelScaleFactorX =  1.0000;
float AccelBiasY =  0.0000;
float AccelScaleFactorY =  1.0000;
float AccelBiasZ =  -0.1581;
float AccelScaleFactorZ =  1.0059;
float MagBiasX =  -4.0163;
float MagScaleFactorX =  1.2318;
float MagBiasY =  5.3633;
float MagScaleFactorY =  1.2594;
float MagBiasZ =  -24.2476;
float MagScaleFactorZ =  0.7173;

// an MPU-9250 object on SPI bus 0 with chip select 10
MPU9250 Imu(SPI,10);
int status;
// a uNavAHRS object
uNavAHRS Filter;
// a flag for when the MPU-9250 has new data
volatile int newData;
// Load accel and mag bias 
// and scale factors from CalibrateMPU9250.ino
float axb, axs, ayb, ays, azb, azs;
float hxb, hxs, hyb, hys, hzb, hzs;
// timers to measure performance
unsigned long t[5] = {0,0,0,0,0};

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  
  //Serial.println("Started...");
  // start communication with IMU 
  status = Imu.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
    
  axb = AccelBiasX;
  axs = AccelScaleFactorX;
  ayb = AccelBiasY;
  ays = AccelScaleFactorY;
  azb = AccelBiasZ;
  azs = AccelScaleFactorZ;
  hxb = MagBiasX;
  hxs = MagScaleFactorX;
  hyb = MagBiasY;
  hys = MagScaleFactorY;
  hzb = MagBiasZ;
  hzs = MagScaleFactorZ;

  Imu.setAccelCalX(axb,axs);
  Imu.setAccelCalY(ayb,ays);
  Imu.setAccelCalZ(azb,azs);

  Imu.setMagCalX(hxb,hxs);
  Imu.setMagCalY(hyb,hys);
  Imu.setMagCalZ(hzb,hzs);

  // setting a 41 Hz DLPF bandwidth
  Imu.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 9 for a 100 Hz update rate
  Imu.setSrd(9);
  // enabling the data ready interrupt
  Imu.enableDataReadyInterrupt();
  // attaching the interrupt to microcontroller pin 1
  pinMode(2,INPUT);
  attachInterrupt(2,runFilter,RISING);
  //Serial.println("Running...");

  Filter.setInitializationDuration(30*1000*1000);

  t[1] = micros();
  t[0] = micros();
}

void loop() {
  if (newData == 1) {
    newData = 0;
    t[0] = micros();
    // read the sensor
    Imu.readSensor();

    // update the filter
    if (Filter.update(Imu.getGyroX_rads(),Imu.getGyroY_rads(),Imu.getGyroZ_rads(),Imu.getAccelX_mss(),Imu.getAccelY_mss(),Imu.getAccelZ_mss(),Imu.getMagX_uT(),Imu.getMagY_uT(),Imu.getMagZ_uT())) {
      /*
      tstop = micros();
      */

      Serial.print(Filter.getPitch_rad()*180.0f/PI);
      Serial.print("\t");
      Serial.print(Filter.getRoll_rad()*180.0f/PI);
      Serial.print("\t");
      Serial.print(Filter.getYaw_rad()*180.0f/PI);
      Serial.print("\t");
      Serial.print(Filter.getHeading_rad()*180.0f/PI);
      //Serial.print("\t");

      /*
      Serial.print(Filter.getGyroBiasX_rads(),6);
      Serial.print("\t");
      Serial.print(Filter.getGyroBiasY_rads(),6);
      Serial.print("\t");
      Serial.print(Filter.getGyroBiasZ_rads(),6);
      Serial.print("\t");
      */
      
      //Serial.print(t[0] - t[1]);
      Serial.println();
      t[1] = micros();
    }
  }
}

void runFilter() {
  newData = 1;
}
