#include <FB_Const.h>
#include <FB_Error.h>
#include <FB_Network.h>
#include <FB_Utils.h>
#include <Firebase.h>
#include <FirebaseFS.h>
#include <Firebase_ESP_Client.h>

#include<WiFi.h>
#include <SparkFunTSL2561.h>
#include <Wire.h>
#include "DHT.h"
#include "secrets.h"
#include "addons/TokenHelper.h"

#define LCDDISPLAY

#ifdef LCDDISPLAY
#include <LiquidCrystal_I2C.h>
#endif

#define DHT11PIN 5
#define AOUT_PIN 34
#define RELAYPIN 25
#define WIFI_TIMEOUT_MS 2000

// Objects
WiFiClient client;
SFE_TSL2561 light;
DHT dht(DHT11PIN, DHT11);
#ifdef LCDDISPLAY
LiquidCrystal_I2C lcd_i2c(0x27, 16, 2);
#endif
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned char ID;
boolean gain;
unsigned int ms;

double lux = 0; 
float tempr = 0.0;
float humdty = 0.0;
int moist = 0;

void readDHT(float *temp, float *humd);

void setup()
{
  
  Serial.begin(9600);  
  initNetwork();
  if(!initLight())
  {
    Serial.println("TSL2561 Initialization issue");
  }
  initDHTsensor();
  initRelay(RELAYPIN);

  initFirebase();

#ifdef LCDDISPLAY
//LCD Initialize
  lcd_i2c.init();
  lcd_i2c.backlight();
#endif
}

void loop()
{
  //readDHT();
  readDHT(&tempr, &humdty);
  readMoisture(&moist);
  lux = readLight();

  //Send serial monitor
  sendtoSerialmonitor("Temperatre",tempr);
  sendtoSerialmonitor("Humidity",humdty);
  sendtoSerialmonitor("Moisture",moist);
  sendtoSerialmonitor("Lux",lux);

#ifdef LCDDISPLAY
  //Send to LCD
  sendKeyvaluepairtoLCD(lux,tempr,humdty,moist);
#endif

  if(moist<1500)
  {
    relayDriver(0,1000);
  }
  else
  {
    relayDriver(1,10);
  }

}

//Init Relay
void initRelay(int relaypin)
{
  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin,HIGH);
}

// Drive the Relay
void relayDriver(bool state, int duration)
{
  digitalWrite(RELAYPIN,state);
  delay(duration);
}

// Initiate WiFi
bool initNetwork()
{
  bool status = false;
  Serial.print("connecting to wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttempTime = millis();
  while (WiFi.status()!=WL_CONNECTED &&
              millis() - startAttempTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);          
  }

  if(WiFi.status() != WL_CONNECTED ){
    Serial.print("failed");
    status = false;
  }
  else{
    Serial.print("Connected");
    Serial.println(WiFi.localIP());
    status = true;
  }

  return status;
}

//Initiate FireBase
bool initCommunication()
{
  bool status = false;
  return status;
}

//Initiate DHT sensor
bool initDHTsensor()
{
  bool status = false;
  dht.begin();
  return true;
}

//Read DHT sensor
void readDHT(float *temp, float *humd)
{
  *humd = dht.readHumidity();
  *temp = dht.readTemperature();
}

//Read Moisture values
void readMoisture(int *mst)
{
  *mst = analogRead(AOUT_PIN);
}

//Send values to Serial print
void sendtoSerialmonitor(String key, int value)
{
  Serial.print(key);
  Serial.print(" : ");
  Serial.println(value);
}

//Send values to LCD display
#ifdef LCDDISPLAY
void sendKeyvaluepairtoLCD(double lux,float temp,float humd, int moist)
{
  lcd_i2c.clear();                  // clear display
  lcd_i2c.setCursor(0, 0);          // move cursor to   (3, 0)
  lcd_i2c.print("Temperature");        // print message at (3, 0)
  lcd_i2c.setCursor(12, 0);          // move cursor to   (0, 1)
  lcd_i2c.print(temp);
  lcd_i2c.setCursor(3, 1);      // move cursor to   (0, 0)
  lcd_i2c.print("Humidity");       // print message at (0, 0)
  lcd_i2c.setCursor(12, 1);      // move cursor to   (2, 1)
  lcd_i2c.print(humd);
  delay(500);
  lcd_i2c.clear();                  // clear display
  lcd_i2c.setCursor(8, 0);          // move cursor to   (3, 0)
  lcd_i2c.print("Lux");        // print message at (3, 0)
  lcd_i2c.setCursor(12, 0);          // move cursor to   (0, 1)
  lcd_i2c.print(lux);
  lcd_i2c.setCursor(3, 1);      // move cursor to   (0, 0)
  lcd_i2c.print("Moisture");       // print message at (0, 0)
  lcd_i2c.setCursor(12, 1);      // move cursor to   (2, 1)
  lcd_i2c.print(moist);
  delay(500);
}

void sendTextlinetoLCD(String text, int row)
{

}
#endif