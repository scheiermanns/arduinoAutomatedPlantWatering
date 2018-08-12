/** defines
*/
typedef enum pumptStateType{
  pumpTurnOn=1,
  pumpOn,
  pumpTurnOff,
  pumpOff
}pumpState_te;

// constants won't change. Used here to set a pin number :
const int ledPin =  LED_BUILTIN;// the number of the LED pin
const int pumpRelaisPin =  12;// the number of the LED pin

/** APPLICATION CONFIGURATION*/
const unsigned long LED_INTERVAL = 500;           // interval at which to blink (milliseconds)
const unsigned long WATERING_INTERVAL = 60000;           // interval at which to blink (milliseconds)
const unsigned long WATERING_TIME = 60000;           // interval at which to blink (milliseconds)


// Variables will change :
int ledState = LOW;             // ledState used to set the LED
pumpState_te pumpState = pumpOff;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillisLed = 0;                // will store last time LED was updated
unsigned long previousMillisPumpRelais = 0;        // will store last time LED was updated


/** ----------- SETUP -----------------*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2500);


  Serial.println("---------- Watering Application -------------");  
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(pumpRelaisPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  digitalWrite(pumpRelaisPin, LOW);
  Serial.println("[INFO] Initialization done ...");

  pumpState = pumpTurnOn;
  pumpControl(pumpState);
  delay(10000);
  pumpState = pumpTurnOff;
  pumpControl(pumpState);
  previousMillisPumpRelais = millis();
}


/** ----------- LOOP -----------------*/
void loop() {
  // check time
  unsigned long currentMillis = millis();

  // check time for blinking LED
  if (currentMillis - previousMillisLed >= LED_INTERVAL){
    previousMillisLed = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set LED
    digitalWrite(ledPin, ledState);
  }

  // check to turn on RELAIS
  if (currentMillis - previousMillisPumpRelais >= WATERING_INTERVAL) {

    if (pumpState == pumpOff){
      // turn on
      pumpState = pumpTurnOn;
    }    
    
    if ((currentMillis - (previousMillisPumpRelais + WATERING_INTERVAL))>=WATERING_TIME){
      // turn off pump  
      previousMillisPumpRelais = currentMillis;
      pumpState = pumpTurnOff;
    }
    // set pump state
    pumpControl(pumpState);
  }

}


void pumpControl(pumpState_te pumpStateIn){
  long hours;
  long minutes;
  long seconds;
  // check
    switch(pumpStateIn){
        case pumpTurnOn:
          pumpState = pumpOn;
          Serial.print("[INFO]: start watering, turn on pump, at time = ");
          Serial.print(" Days: " + String(millis()%86400000));
          Serial.print(" Hours: " + String(millis()%3600000));
          Serial.print(" Minutes: " + String(millis()%60000));
          Serial.print(" Seconds: " + String(millis()%1000));
          Serial.print("\n");
        case pumpOn:          
          digitalWrite(pumpRelaisPin, HIGH);
          pumpState = pumpOn;
          break;
        
        case pumpTurnOff:
          pumpState = pumpOff;
        case pumpOff:
          digitalWrite(pumpRelaisPin, LOW);
          Serial.print("[INFO]: stop watering, turn off pump, at time = ");
          Serial.print(" Days: " + String(millis()%86400000));
          Serial.print(" Hours: " + String(millis()%86400000%3600000));
          Serial.print(" Minutes: " + String(millis()%86400000%3600000%60000));
          Serial.print(" Seconds: " + String(millis()%86400000%3600000%60000%1000));
          Serial.print("\n");
          pumpState = pumpOff;
          break;
        
        default:
          pumpState = pumpOff;
          Serial.println("[ERROR]:pump unknown state...");
          break;   
      }
}

