/*
 * Johnny Caselato Guimarães - 2022
 * TOPUS - Projetos Aeroespaciais - EESC | USP
 * 
 * Example: Single MPU6050
 * 
 * - This example aims to show how to do basic initialisation and calibrate
 * a single MPU 6050 connected to an arduino, as well as read and show gyro,
 * angle and acceleration data.
 */
#include <TOPUS_sensor.h>   //Including lib

#define MPU_interval 500    //Interval to perform a read operation [ms]

unsigned long t0;           //Variable for store the time of the last reading

TOPUS_mpu mpu(Wire, 0x68);  //Instantiate a new TOPUS_mpu object for the sensor on the default MPU6050 adress

String MPU_getdata(){       //Function that return a String which contains all the data
  String mpu_data = "";
  mpu.Execute();  //Update the sensor readings

  //Get the lastest values and insert them on the data string
  mpu_data.concat("AngX: ");   mpu_data.concat(mpu.GetAngX());
  mpu_data.concat(" |AngY: "); mpu_data.concat(mpu.GetAngY());
  mpu_data.concat(" |AngZ: "); mpu_data.concat(mpu.GetAngZ());
  mpu_data.concat(" |AccX: "); mpu_data.concat(mpu.GetAccX()*GRAVITY);
  mpu_data.concat(" |AccY: "); mpu_data.concat(mpu.GetAccY()*GRAVITY);
  mpu_data.concat(" |AccZ: "); mpu_data.concat(mpu.GetAccZ()*GRAVITY);
  
  return mpu_data;
}

// # Start Setup # 
void setup() {
  Serial.begin(9600); //Start serial at 9600 baud rate
  Serial.flush();     //Clear Serial data buffer
  
  Serial.print("Starting the MPU6050, please, don't move the sensor... ");
  
  mpu.Initialize();   //Start the module
  mpu.Calibrate();    //Calibrate method may takes up to 5s to be completed
  Serial.println("[OK]");

  t0 = millis();      //Set the start for the time register
  delay(500);
}
// # Finish Setup #

// # Start Main Loop #
void loop(){
  /*
   * At Every loop execution, the difference between the current time and
   * the last execution time (t0) is compared to the predefined interval.
   * So, it's not necessary to use a "delay" function.
   */
  if((millis() - t0) >= MPU_interval){
    Serial.println(MPU_getdata());
    t0 = millis();
  }
}
// # Finish Main Loop #
