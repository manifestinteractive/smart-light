#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_NeoPixel.h>
#include <RunningAverage.h>
#include <SPI.h>
#include <Adafruit_BLE_UART.h>

/* --- General Settings --- */

boolean lightIsOn = false;
boolean roomIsDark = false;
boolean inMotion = false;

/* --- Motion Sensor Settings --- */

boolean motionStart;
boolean motionLock = true;
int calibrationTime = 15;
unsigned long int pause = 5000;
unsigned long int startTime;
unsigned long int endTime;
unsigned long int totalTime;
unsigned long int motionStoppedTime;

/* --- NeoPixel Settings --- */

int lightBrightness = 50;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);

/* --- Light Sensor Settings --- */

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
RunningAverage lightReadingAverage(10);

/* --- Bluetooth Settings --- */

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(10, 2, 9);
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

/* --- Main Arduino Functions --- */

void setup(){
  Serial.begin(9600);
  setupMotion();
  setupNeoPixel();
  setupLightSensor();
  //setupBluetooth();
}

void loop(){
  checkForMotion();
  checkForLight();
  //checkForBluetooth();
  delay(100);
}

/* --- Motion Sensor Functions --- */

void setupMotion(){
  pinMode(3, INPUT);
  digitalWrite(3, LOW);

  Serial.println("Calibrating Motion Sensor. Do not move for 15 Seconds.");
  for(int i = 0; i < calibrationTime; i++){
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("Calibration Complete.");
}

void checkForMotion(){
  if(digitalRead(3) == HIGH){
    if(motionLock){
      motionStarted();
    }
    motionStart = true;
  }

  if(digitalRead(3) == LOW){
    if(motionStart){
      motionStoppedTime = millis();
      motionStart = false;
    }
    if( !motionLock && millis() - motionStoppedTime > pause){
      motionEnded();
    }
  }
}

void motionStarted(){
  Serial.println("Motion Started");
  motionLock = false;
  inMotion = true;
  startTime = millis()/1000;
  endTime = 0;
  totalTime = 0;

  if(roomIsDark == true)
  {
    turnLightOn();
  }
}

void motionEnded(){
  Serial.println("Motion Ended");
  motionLock = true;	
  inMotion = false;
  endTime = (millis() - pause)/1000;
  totalTime = (endTime - startTime);

  turnLightOff();
}

bool getInMotion()
{
  return inMotion;
}

/* --- NeoPixel Functions --- */

void setupNeoPixel(){
  pixels.begin();
  pixels.setBrightness(lightBrightness);
  pixels.show();
}

void turnLightOn(){
  if( !lightIsOn)
  {
    Serial.println("Turning Light On");
    lightIsOn = true;
    for(int i=0;i<12;i++){
      lightBrightness += 10;
      pixels.setPixelColor(i, pixels.Color(150,150,150));
      pixels.setBrightness(lightBrightness);
      pixels.show();
      delay(25);
    }
  } 
}

void turnLightOff(){
  if(lightIsOn)
  {
    Serial.println("Turning Light Off");
    lightIsOn = false;
    for(int i=0;i<12;i++){
      lightBrightness -= 10;
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.setBrightness(lightBrightness);
      pixels.show();
      delay(25);
    }
  }
}

/* --- Light Sensor Functions --- */

void setupLightSensor(){
  tsl.begin();
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}

void checkForLight(){
  sensors_event_t event;
  tsl.getEvent(&event);
 
  if (event.light >= 0)
  {
    lightReadingAverage.addValue(event.light);
  }
  
  if(lightReadingAverage.getAverage() <= 10 && !roomIsDark)
  {
    Serial.println("Room is now dark");
    roomIsDark = true;
    
    if(getInMotion())
    {
      turnLightOn();
    }
  }
  else if(lightReadingAverage.getAverage() > 10 && roomIsDark)
  {
    Serial.println("Room is no longer dark");
    roomIsDark = false;
  }
}

/* --- Bluetooth Functions --- */

void setupBluetooth(){
  BTLEserial.setDeviceName("MyLight");
  BTLEserial.begin();
}

void checkForBluetooth(){
// Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();
  
  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Waiting for Bluetooth Device Connection"));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Bluetooth Device Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or Connection timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }
  
  if (status == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us!
    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }
    
    // Next up, see if we have any data to get from the Serial console
 
    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();
 
      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());
      
      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");
      
      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);
    }
  }

}

void receivedMessage(String message)
{
}

void sendMessage(String message)
{
}
