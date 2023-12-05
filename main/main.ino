#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL4ST1ukzYa"
#define BLYNK_TEMPLATE_NAME "Forklift"
#define BLYNK_AUTH_TOKEN "mnDixmG0-dJPkC-IFD3MAmM6z3FUGHs5"
#define BLYNK_PRINT Serial

BlynkTimer timer1;
BlynkTimer timer2;

char ssid[] = "KiKi";
char pass[] = "kikikiki";
char auth[] = "mnDixmG0-dJPkC-IFD3MAmM6z3FUGHs5";

int forksHeight;
int forksActualHeight;
int xAxis;
int yAxis;
int LMPI;
int DMPI;
int motorPinL = D5;
int motorPinD = D6;
int step;
int frontDistance;
int delayCount;

bool autoMode;
bool debug;
bool lIrSensor;
bool dIrSensor;
bool doOnce;

WidgetTerminal OutputTerminal(V3);

void setup() {
  timer1.setInterval(10000L, BatteryIndicator);
  timer2.setInterval(1000L, Debug);
}

void loop() {
  Blynk.run();
  CalculateMotorPower();
  HandleSteps();
  HandleForks(forksHeight);
  delayCount++;
}

void HandleSteps(){

  switch (step) {
    case 0: // Idle

      break;
    case 1: // Začetek sledenja črti
      
      break;
    case 2: // Približaj paleti
      
    case 3: // Dvigni paleto
      forksHeight = 160;
      if(CheckIfForksHeightsMatch()){
        step++;
      }
      break;
    case 4: // Odmakni se (malo vzratno, da je dovol prostora za zasuk)
   
      break;
    case 5: // Zavrti se na mestu
      if(!doOnce){
        TurnAround();
        delayCount = 0;
        doOnce = true;
      }
      if(CheckIfDelayEnded(200)){
        step++;
        doOnce = false;
      }
      break;
    case 6: // Sledi črti nazajn
   
      break;
    case 7: // Spusti paleto
      forksHeight = 0;
      if(CheckIfForksHeightsMatch()){
        step++;
      }
      break;
    case 8: // Odmakni se od palete (malo vzratno, da je dovol prostora za zasuk)
      
      break;
    case 9: // Zavrti na mestu, če je avtomatski način potem se cikelj ponovi
      if(!doOnce){
        TurnAround();
        delayCount = 0;
        doOnce = true;
      }
      if(CheckIfDelayEnded(200)){
        step++;
        doOnce = false;
      }
      break;
  }

}

void FollowTrail(){

}

void HandleApproach(bool approach){
  if(approach){

  }
  else{

  }
}

void HandleForks(int height){
  
}

void TurnAround(){

}

void CheckIfForksHeightsMatch(){
  return (forksActualHeight == forksHeight);
}

void CheckIfDelayEnded(int delayTime){
  return (delayCount >= delayTime);
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
  WriteToMotors(LMPI, DMPI);
}

void WriteToMotors(int LMP, int DMP){
  analogWrite(motorPinL,LMP);
  analogWrite(motorPinD,DMP);
}

void BatteryIndicator(){ // Prikaz Led barve glede na napetost, tu ne rabiš nič dodat, izvede se vsakih 10s.
  voltageSensorVal = analogRead(voltageSensorPin);             
  vIn = ((voltageSensorVal / 1024) * 3) * 5.05;
  if(vIn > 13) Blynk.setProperty(V0, "color", "#33ff6b");
  else if(vIn < 13 && vIn > 12.3 ) Blynk.setProperty(V0, "color", "#ffa533");
  else if (vIn < 12.3 && vIn > 11.3) Blynk.setProperty(V0, "color", "#ff3333");
  else if (vIn < 11.3) Blynk.setProperty(V0, "color", "#360f0f");
}

void Debug(){ // To je za debug v Blynk terminal, lahko dodaš kako spremenljivko,
  if(debug){
    OutputTerminal.clear();

    OutputTerminal.println(
      "Xaxis: "+ String(Xaxis) + 
      " Yaxis: "+ String(Yaxis) +
      " autoMode: "+ String(autoMode)
    );

    OutputTerminal.println(
      " lIrSensor: "+ String(lIrSensor) + 
      " dIrSensor: "+ String(dIrSensor) +
      " frontDistance: "+ String(frontDistance)
    );

  }
}

BLYNK_WRITE(V1){ // Mode, 0-ročni način, 1-avtomatski način
  autoMode = param[0].asInt();
} 

BLYNK_WRITE(V3){ //TERMINAL, tu ne diraj nič, razen če hočeš kaj dodati
  if(String("/ping") == param.asStr()){
    OutputTerminal.println("Forklift ping");
  }
  else if(String("/c") == param.asStr()){
    OutputTerminal.clear();
  }
  else if(String("/status") == param.asStr()){
    OutputTerminal.println(status);
  }
  else if(String("/debug") == param.asStr()){
    isDebug = !isDebug;
    OutputTerminal.println("Debug: "+ String(isDebug));
  }
  OutputTerminal.flush();
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


