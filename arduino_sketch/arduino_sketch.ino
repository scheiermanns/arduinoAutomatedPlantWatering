/* pump state application enum */
typedef enum pumptStateType{
  pumpOn=1,
  pumpOff
}pumpState_te;

/* GPIO defintion for pump and led */
const int ledPin =  LED_BUILTIN;// the number of the LED pin
const int pumpRelaisPin =  12;// the number of the LED pin

/* APPLICATION CONFIGURATION*/
const unsigned long LED_INTERVAL = 500;           // interval at which to blink (milliseconds)
const unsigned long WATERING_INTERVAL = 24*60*60*1000; // interval at which to blink (milliseconds)
const unsigned long WATERING_TIME = 45000;        // interval at which to blink (milliseconds)

/* pump states */
int ledState = LOW;             // ledState used to set the LED
pumpState_te pumpState = pumpOff;
bool printMessage = true;

/* timing varaibles */
unsigned long previousMillisLed = 0;                // will store last time LED was updated
unsigned long previousMillisWateringInterval = 0;        // will store last time watering has been taken

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

  pumpState = pumpOn;
  pumpControl(pumpState);
  delay(WATERING_TIME);
  pumpState = pumpOff;
  pumpControl(pumpState);
  previousMillisWateringInterval = millis();
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
  if (currentMillis - previousMillisWateringInterval >= WATERING_INTERVAL) {

    pumpState = pumpOn;    
    if ((currentMillis - (previousMillisWateringInterval + WATERING_INTERVAL))>=WATERING_TIME){
      // turn off pump  
      previousMillisWateringInterval = currentMillis;
      pumpState = pumpOff;
    }
    // set pump state
    pumpControl(pumpState);
  }
}

// pumpControl - control the pump pin according to state
void pumpControl(pumpState_te pumpStateIn){
    // check
    switch(pumpStateIn){
        case pumpOn:          
          digitalWrite(pumpRelaisPin, HIGH);
          pumpState = pumpOn;
          if (printMessage==true){
            Serial.print("[INFO]: start watering, turn on pump, at time = ");
            Serial.print(" Days: " + String(millis()/86400000));
            Serial.print(" Hours: " + String(millis()/3600000));
            Serial.print(" Minutes: " + String(millis()/60000));
            Serial.print(" Seconds: " + String(millis()/1000));
            Serial.print("\n");
            printMessage=false;
          }        
          break;
        case pumpOff:
          digitalWrite(pumpRelaisPin, LOW);
          Serial.print("[INFO]: stop watering, turn off pump, at time = ");
          Serial.print(" Days: " + String(millis()/86400000));
          Serial.print(" Hours: " + String(millis()/3600000));
          Serial.print(" Minutes: " + String(millis()/60000));
          Serial.print(" Seconds: " + String(millis()/1000));
          Serial.print("\n");
          pumpState = pumpOff;
          printMessage = true;
          break;
        
        default:
          pumpState = pumpOff;
          printMessage = true;
          Serial.println("[ERROR]:pump unknown state...");
          break;   
      }
}

