#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL4ST1ukzYa"
#define BLYNK_TEMPLATE_NAME "Forklift"
#define BLYNK_AUTH_TOKEN "mnDixmG0-dJPkC-IFD3MAmM6z3FUGHs5"
#define BLYNK_PRINT Serial


BlynkTimer timer1;

char ssid[] = "KiKi";
char pass[] = "kikikiki";
char auth[] = "mnDixmG0-dJPkC-IFD3MAmM6z3FUGHs5";

int forksHeight;
int xAxis;
int yAxis;
int LMPI;
int DMPI;
int motorPinL = D5;
int motorPinD = D6;
int step;

bool autoMode;

WidgetTerminal OutputTerminal(V3);

void setup() {
  timer1.setInterval(10000L, BatteryIndicator);

}

void loop() {
  Blynk.run();
  CalculateMotorPower();
  HandleSteps();
}

void HandleSteps(){

  switch (step) {
    case 0: // Idle

      break;
    case 1: // Začetek sledenja črti
      handleDriving();
      break;
    case 2: // Približaj paleti
      handleApproach(true);
    case 3: // Dvigni paleto
      handleForks(true);
      break;
    case 4: // Odmakni se (malo vzratno, da je dovol prostora za zasuk)
      handleApproach(false);
      break;
    case 5: // Zavrti se na mestu
        handleRotation(true);
      break;
    case 6: // Sledi črti nazajn
      handleDriving();
      break;
    case 7: // Spusti paleto
      handleForks(false);
      break;
    case 8: // Odmakni se od palete (malo vzratno, da je dovol prostora za zasuk)
      handleApproach(false);
      break;
    case 9: // Zavrti na mestu, če je avtomatski način potem se cikelj ponovi
      handleRotation(false);
      break;
  }

}

void CalculateMotorPower(){
  if(xAxis >= 0){
    LMPI = xAxis + yAxis;
    DMPI = yAxis;
  }
  else{
    LMPI = yAxis;
    DMPI = -xAxis + yAxis;
  }
  analogWrite(motorPinL,LMPI);
  analogWrite(motorPinD,DMPI);
}

void BatteryIndicator(){ // Prikaz Led barve glede na napetost, tu ne rabiš nič dodat, izvede se vsakih 10s.
  voltageSensorVal = analogRead(voltageSensorPin);             
  vIn = ((voltageSensorVal / 1024) * 3) * 5.05;
  if(vIn > 13) Blynk.setProperty(V0, "color", "#33ff6b");
  else if(vIn < 13 && vIn > 12.3 ) Blynk.setProperty(V0, "color", "#ffa533");
  else if (vIn < 12.3 && vIn > 11.3) Blynk.setProperty(V0, "color", "#ff3333");
  else if (vIn < 11.3) Blynk.setProperty(V0, "color", "#360f0f");
}

BLYNK_WRITE(V1){ // Mode, 0-ročni način, 1-avtomatski način
  autoMode = param[0].asInt();
} 

BLYNK_WRITE(V5){ //Tu dobiš vrednost slidera za handleForks. To vrednost je potem treba spremenit v rotacijo motorja.
  if(!autoMode){
    forksHeight = param[0].asInt();
  }
} 

BLYNK_WRITE(V6){ //Tu dobiš X vrednost Joysticka vsakič, ko vrednost spremeni in jo zapišemo kot globalno spremenljivko, da lahko uporabljaš drugje
  if(!autoMode){
    xAxis = param[0].asInt();
  }
}

BLYNK_WRITE(V7){ //Tu dobiš Y vrednost Joysticka vsakič, ko vrednost spremeni in jo zapišemo kot globalno spremenljivko, da lahko uporabljaš drugje
  if(!autoMode){
    yAxis = param[0].asInt();
  }
}


