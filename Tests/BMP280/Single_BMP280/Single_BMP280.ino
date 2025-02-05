/*
 * Johnny Caselato Guimarães - 2022
 * TOPUS - Projetos Aeroespaciais - EESC | USP
 * 
 * Example: Single BMP280
 * 
 * - This example aims to show how to do basic initialisation and calibrate
 * a single BMP280 connected to an arduino, as well as read and show pressure
 * altitude (from sea level) and temperature.
 * 
 * - If you want to know the elevation from your location and compare the altitude readings,
 * I recommend using: https://whatismyelevation.com/pt/ or https://earth.google.com/
 * and search for your address.
 */
#include <TOPUS_sensor.h>   //Including lib

#define BMP_interval 500    //Interval to perform a read operation [ms]

unsigned long t0;           //Variable for store the time of the last reading

TOPUS_bmp bmp;              //Instantiate a new TOPUS_bmp object for the sensor

String BMP_getdata(){       //Function that return a String which contains all the data
  String bmp_data = "";                               
  float temperature_aux, pressure_aux, altitude_aux;  //Auxiliary variables that will receive the readings
  int tries = 0;  //Counts the number of iterations to get the values

  //Tries to update the data up to 500 times. If there's no response, all variables are set to zero
  while(!bmp.getMeasurements(temperature_aux, pressure_aux, altitude_aux) && tries < 500){
    temperature_aux = pressure_aux = altitude_aux = 0;
    tries++;
  }

  //Insert data on the data string
  bmp_data.concat("Temp: "); bmp_data.concat(temperature_aux); bmp_data.concat("ºC");
  bmp_data.concat(" |Pres: "); bmp_data.concat(pressure_aux); bmp_data.concat("hPa");
  bmp_data.concat(" |Alti: "); bmp_data.concat(altitude_aux); bmp_data.concat("m");
  
  return bmp_data;
}

// # Start Setup # 
void setup(){
  Serial.begin(9600); //Start serial at 9600 baud rate
  Serial.flush();     //Clear Serial data buffer
  
  Serial.print("\nStarting the BMP280, please don't move the sensor... ");
  if(!bmp.begin(BMP280_I2C_ALT_ADDR)){ Serial.println("[BMP not found]"); } //Trying to start the module
  else{ Serial.println("[OK]"); }

  //Initial tested and recommended settings
  bmp.setPresOversampling(OVERSAMPLING_X8);
  bmp.setTempOversampling(OVERSAMPLING_X1);
  bmp.setIIRFilter(IIR_FILTER_8);
  bmp.setTimeStandby(TIME_STANDBY_62MS);
  bmp.startNormalConversion();

  t0 = millis();  //Set the start for the time register
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
  if((millis() - t0) >= BMP_interval){
    Serial.println(BMP_getdata());
    t0 = millis();
  }
}
// # Finish Main Loop #
