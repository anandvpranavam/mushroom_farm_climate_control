// THIS PROGRAM IS CODED TO RUN IN AN ARDUINO MEGA //


#include <DS3231.h>
//#include <SD.h>
//#include <SPI.h>
#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1); // RX, TX

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(9,8,7,6,5,4); //RS,EN,D4,D5,D6,D7



#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
#define pwmPin 3
//int pinCS = 10; // Pin 10 on Arduino Uno
#define DHTPIN 2 // Digital pin connected to the DHT sensor
//int pin_errLED = 11; // SD card error LED
//#define relayPin 12 // Pin A4 for relay switching


byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[9]; 
unsigned long th,tpwm,p1,p2, tl,ppm, ppm2, ppm3 = 0;

DS3231  rtc(SDA, SCL);
DHT dht(DHTPIN, DHTTYPE);
//File myFile;



void setup() {
  Serial.begin(9600); 
  Serial.println(F("DHTxx test!"));
  mySerial.begin(9600); 
  dht.begin();
//  pinMode(pinCS, OUTPUT);
  pinMode(pwmPin, INPUT);
//  pinMode (pin_errLED, OUTPUT);
 // pinMode (relayPin, OUTPUT);

  lcd.begin(16,2);
  delay(1000);
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Welcome...");
  delay(2000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Intelligent");
  lcd.setCursor(0,1);
  lcd.print("Mushroom farm");
  delay(500);


  
  
  // SD Card Initialization
  
  
  th = pulseIn(pwmPin, HIGH, 30000); // use microseconds
  tl = pulseIn(pwmPin, LOW, 30000);
  tpwm = th + tl; // actual pulse width
  Serial.print("PWM-time: ");
  Serial.print(tpwm);
  Serial.println(" us");
  p1 = tpwm/502; // start pulse width
  p2 = tpwm/251; // start and end pulse width combined
  
  rtc.begin(); 

  //rtc.setDOW(SATURDAY);           // Set Day-of-Week
  //rtc.setTime(07, 13, 00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(04, 05, 2019);   // Set the date to January 1st, 2014
}




void loop() {


  
  //digitalWrite (relayPin, LOW);
  
  //digitalWrite (pin_errLED, LOW);
  //delay(500);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();                  // DHT22
  float f = dht.readTemperature(true);
  
   if (isnan(h) || isnan(t) || isnan(f)) 
   {
    Serial.println("Failed to read from DHT sensor!");
    return;
   }

  Serial.print(rtc.getTimeStr());
  Serial.print(",");
  Serial.println(int(rtc.getTemp()));
   // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  mySerial.write(cmd,9);
  mySerial.readBytes(response, 9);
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  ppm = (256*responseHigh)+responseLow;


  //CO2 via pwm
  do {
    th = pulseIn(pwmPin, HIGH, 1004000) / 1000;
    tl = 1004 - th;
   // ppm2 = 2000 * (th-2)/(th+tl-4);
    ppm3 = 5000 * (th-2)/(th+tl-4);
  }
  
  while (th == 0);

  int temp= t;
  int hum= h;
  int co2= ppm3;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Temp: Hum: CO2:");
lcd.setCursor(0,1);
lcd.print(temp);
lcd.print("C");
lcd.setCursor(6, 1);
lcd.print(hum);
lcd.print("%");
lcd.setCursor(10, 1);
lcd.print(co2);
lcd.print("ppm");

//Serial.println(ppm);
 // Serial.println(th);
 // Serial.println(ppm2);
 Serial.print("CO2 level: ");
  Serial.print(ppm3);
  Serial.println(" ppm");
  Serial.println("-----------");
  delay(50);
     
/*
  myFile = SD.open("TEST.txt", FILE_WRITE);
  if (myFile) {  
    
    // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  // Send time
  Serial.println(rtc.getTimeStr());
  delay(1000);
  
  myFile.println(rtc.getDOWStr());
  myFile.print(" ");
  myFile.println(rtc.getDateStr());
  myFile.print(" -- ");
  myFile.print(rtc.getTimeStr());
  myFile.print(":- ");   
  delay(500); 
   
    //myFile.println(int(rtc.getTemp()));
  
  myFile.print("CO2 level: ");
  myFile.print(ppm3);
  myFile.println(" ppm");
  
  
  myFile.print(F("Humidity: "));
  myFile.print(h);
  myFile.print(F("%  Temperature: "));
  myFile.print(t);
  myFile.println(F("°C "));
  myFile.println("-----------");
    
    myFile.close(); // close the file

    
 // Automation section
    
    
   
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening TEST.txt");
    digitalWrite (pin_errLED, HIGH);
    delay(1000);
  }*/
  delay(1000);
  
}
