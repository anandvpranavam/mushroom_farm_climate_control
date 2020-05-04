#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include "DHT.h"

#define temLowTrigger 9  //Setting the trigger value for the temperture, once the temperture lower than this trigger value, the heater band will start heating
#define humLowTrigger 45 //Setting the trigger value for the humidity, once the humidity lower than this value, start humidification


#define DHTPIN 15     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

const int relay1 =  18;      // the number of the relay 1 pin
const int relay2 =  19;      // the number of the relay 2 pin



DHT dht(DHTPIN, DHTTYPE);


//Pin assignments for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
String keyString = "";
                 
void setup() 
{ 
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("initializing...");
  lcd.setCursor(0, 1);
  delay(1000);
  
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  lcd.clear();
  
  /*
  OPTIONAL
  keypad.setRate(x);
  Sets the sample rate at once every x milliseconds.
  Default: 10ms
  */
  keypad.setRate(10);
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

}

void loop() 
{ 

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    
    //Display 
    lcd.setCursor(0, 0);
    lcd.print("Hum: ");
    lcd.print(h);
    lcd.print(" %");
    
    lcd.setCursor(0, 1);
    lcd.print("Tem: ");
    lcd.print(t);
    lcd.print(" *C");
    
    if(h < humLowTrigger) //if the humidity lower than the trigger value
      digitalWrite(relay1, LOW); //start humidification
    else
      digitalWrite(relay1, HIGH);
      
     if(t < temLowTrigger)
      digitalWrite(relay2, LOW);//start heating
    else
      digitalWrite(relay2, HIGH);
  }
}

