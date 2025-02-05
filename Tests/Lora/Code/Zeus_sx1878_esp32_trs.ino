/*
  Johnny Caselato Guimarães - 06/2023
  Zeus project - LoRa SX1278 transmitter communication test code

  The following code was made in order to evaluate the parameters
  and validate the communication between two RA-02 (AI-thinker) modules,
  which will be used to get data from TESEU rocket, as part of the
  ZEUS mission development.

  Credits for Mohammad Damirchi from Electro Peak, where I got 
  the first example.

  Tested in 18/06/2023
*/

//Libraries
#include <SPI.h>
#include <LoRa.h>

uint16_t PacketNumber = 0;

//ESP32 wiring definition
#define NSS_PIN 33
#define RST_PIN 27
#define IO0_PIN 32
#define CHN_FRQ 433E6

//Other macros
//#define LON_PIN 33
//#define LTR_PIN 32
#define MAX_ATP 10

/*
  LoraConfig function applies especific parameters to the module registers
  aiming to get the maximum range possible, as it keeps a decent data
  transmition.
  Reference: https://www.deviceplus.com/arduino/arduino-long-range-communication-tutorial-loralib-library/
*/
//Start LoraConfiguration
void LoraConfiguration(){
  LoRa.sleep();
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(6);
  LoRa.idle();
  delay(10);
}
//End LoraConfiguration

void LoraSend(String Data = "", bool PrintData = 0){
  //digitalWrite(LTR_PIN, 1);
  String PacketData = "(PN="+String(PacketNumber)+") ";
  PacketData.concat(Data);

  if(PrintData){
    Serial.println(PacketData);
  }

  LoRa.beginPacket();
  LoRa.print(PacketData);
  LoRa.endPacket();

  PacketNumber++;
  //digitalWrite(LTR_PIN, 0);
}

//Start setup
void setup() {
  Serial.begin(115200);
  //pinMode(LON_PIN, OUTPUT);
  //pinMode(LTR_PIN, OUTPUT);
  //delay(1500);
  Serial.println("\nZEUS - SX1278 Transmitter test");

  bool LoraStart;
  LoRa.setPins(NSS_PIN, RST_PIN, IO0_PIN);
  for(int tries = 0; tries < MAX_ATP; tries++){
    Serial.print("Trying to initialize (attempt "+String(tries+1)+")");

    LoraStart = LoRa.begin(CHN_FRQ);
    if(LoraStart){
      Serial.println("- Module Started");
      //digitalWrite(LON_PIN, 1);
      tries = MAX_ATP;

      LoraConfiguration();
    } else{
      Serial.println("- Initialization Failed");
    }
    delay(1000);
  }

  if(!LoraStart){ 
    Serial.println("\nModule could not be founded/initialized!\n");
    while(1){
      /*digitalWrite(LON_PIN, 1);
      delay(250);
      digitalWrite(LON_PIN, 0);
      delay(250);*/
    }
  }  
}
//End setup

//Start main loop
void loop() {
  long timeStart = millis();
  LoraSend("Zeus LoRa test", 1);
  Serial.println("\t[Tranmission Time= "+String((millis()-timeStart))+"ms]");
  delay(100);
}
//End main loop
