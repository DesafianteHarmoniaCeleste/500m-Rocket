#include <TOPUS_sensor.h>   //Including lib

#define BMP_interval 10    //Interval to perform a read operation [ms]

unsigned long t0;           //Variable for store the time of the last reading

TOPUS_bmp bmp;              //Instantiate a new TOPUS_bmp object for the sensor
float press=0;
class LKF_D {
  private:
    float _err_mes;
    float _err_est;
    float _data;
    float Kalman_gain;
    float last_est;
    float current_est;

  public:
    LKF_D(float mea_e, float est_e, float data) {
      _err_mes = mea_e;
      _err_est = est_e;
      _data = data;
      Kalman_gain = 0;
      last_est = 0;
      current_est = 0;
    }

    float calcula_Kalman_gain() {
      return (_err_mes / (_err_mes + _err_est));
    }

    float att_est(float data_mea) {
      Kalman_gain = calcula_Kalman_gain();
      current_est = last_est + Kalman_gain * (data_mea - last_est);
      _err_mes = Kalman_gain * Kalman_gain * _err_est + (1 - Kalman_gain) * (1 - Kalman_gain) * _err_mes;
      last_est = current_est;
      return current_est;
    }
};

LKF_D Filtro_BMP280(1.0 , 0.001, 800.0);

float BMP_getdata(){       //Function that return a String which contains all the data
  String bmp_data = "";                               
  float temperature_aux, pressure_aux, altitude_aux;  //Auxiliary variables that will receive the readings
  float KF_temp, KF_pressure, KF_alt;
  int tries = 0;  //Counts the number of iterations to get the values

  //Tries to update the data up to 500 times. If there's no response, all variables are set to zero
  while(!bmp.getMeasurements(temperature_aux, pressure_aux, altitude_aux) && tries < 500){
    temperature_aux = pressure_aux = altitude_aux = 0;
    tries++;
  }

  //Insert data on the data string
  //bmp_data.concat(temperature_aux); bmp_data.concat(" ");
  bmp_data.concat(pressure_aux); bmp_data.concat(" ");
  //bmp_data.concat(altitude_aux); bmp_data.concat(" ");

  KF_pressure = Filtro_BMP280.att_est(pressure_aux);
  bmp_data.concat(KF_pressure); bmp_data.concat(" ");
  
  return KF_pressure;
}

void setup() {
  Serial.begin(9600);
  Serial.flush();     //Clear Serial data buffer
  
  if(!bmp.begin(BMP280_I2C_ALT_ADDR)){} //Trying to start the module
  else{}

  //Initial tested and recommended settings
  bmp.setPresOversampling(OVERSAMPLING_X8);
  bmp.setTempOversampling(OVERSAMPLING_X1);
  bmp.setIIRFilter(IIR_FILTER_8);
  bmp.setTimeStandby(TIME_STANDBY_62MS);
  bmp.startNormalConversion();

  t0 = millis();  //Set the start for the time register
  delay(500);
  Serial.println("Temperatura, Pressão, Altitude, Tempo");

}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis() - t0) >= BMP_interval){

    Serial.println(BMP_getdata());
    t0 = millis();
    //Serial.println(t0);
  }
}
