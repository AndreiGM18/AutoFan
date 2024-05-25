// Copyright Mitran Andrei-Gabriel 2024
#include <LiquidCrystal.h>
#include <SimpleDHT.h>
#include <IRremote.h>

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define RED 6
#define BLUE 2

int tempPin = 1;

int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 11
int tempThreshold = 28; // Initial temperature threshold
int timeThreshold = 30; // Initial time threshold in seconds
bool mode = true; // true: LIGHT_DEPENDENT, false: LIGHT_INDEPENDENT
bool systemRunning = true; // System running state
bool autoOffEnabled = true; // Auto-off feature state

unsigned long startTime; // Variable to store the start time

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {
    case 0xFFA25D: 
      Serial.println("CH-");
      if (tempThreshold > 20)
        tempThreshold -= 1;
      break;
    case 0xFFE21D: 
      Serial.println("CH+");
      if (tempThreshold < 40)
        tempThreshold += 1;
      break;
    case 0xFF629D: 
      Serial.println("CH");
      mode = !mode;
      break;
    case 0xFF906F: 
      Serial.println("EQ");
      systemRunning = !systemRunning;
      if (systemRunning) {
        startTime = millis(); // Reset the start time when system is turned back on
      }
      break;
    case 0xFFE01F: 
      Serial.println("VOL-");
      if (timeThreshold > 10) {
        timeThreshold -= 10;
        startTime = millis(); // Reset the start time when time threshold is changed
      }
      break;
    case 0xFFA857: 
      Serial.println("VOL+");
      if (timeThreshold < 60) {
        timeThreshold += 10;
        startTime = millis(); // Reset the start time when time threshold is changed
      }
      break;
    case 0xFFC23D: 
      Serial.println("PAUSE");
      autoOffEnabled = !autoOffEnabled;
      startTime = millis(); // Reset the start time when toggling auto-off
      break;
    // other cases...
    default: 
      Serial.println(" other button   ");
  }// End Case
  delay(500); // Do not get immediate repeat
} //END translateIR

int lightpin = 0;

// Initializes the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 

  // Sets up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Sets pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  irrecv.enableIRIn(); // Start the receiver

  // Store the start time
  startTime = millis();
}

void colour(int colour) {
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(colour, HIGH);
}

void stopSystem() {
  systemRunning = false;
}

void loop() {
  // Check if the system is running and the time threshold has passed
  if (systemRunning && autoOffEnabled && millis() - startTime >= timeThreshold * 1000) {
    stopSystem();
  }

  if (!systemRunning) {
    // Stop the system and display OFF
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OFF");
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(ENABLE, LOW);
    while (!systemRunning) {
      // Stay in this loop until the system is turned back on
      if (irrecv.decode(&results)) {
        translateIR();
        irrecv.resume();
      }
    }
    startTime = millis(); // Reset the start time when the system is turned back on
  }

  if (irrecv.decode(&results)) // Have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // Receive the next value
  }  

  // Calculate the remaining time until shutdown
  unsigned long elapsedTime = millis() - startTime;
  int remainingTime = autoOffEnabled ? timeThreshold - (elapsedTime / 1000) : -1;

  // Reads the temperature
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celsius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celsius to Fahrenheit
  int temperature = tempC;

  int reading = analogRead(lightpin);
  int dark = reading / 114;
  if (dark > 1) dark = 1;

  // Display mode, temperature threshold, and time threshold on LCD
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(mode ? "LD " : "LI ");
  lcd.print("Th:");
  lcd.print(tempThreshold);
  lcd.setCursor(11, 0);
  lcd.print("T:");
  if (autoOffEnabled) {
    lcd.print(timeThreshold);
    lcd.print("   "); // Clear remaining characters
  } else {
    lcd.print("OFF");
    lcd.print("  "); // Clear remaining characters
  }

  // Prints the temperature and light/dark status with countdown timer
  lcd.setCursor(0, 1);
  lcd.print((int)temperature);
  lcd.print("*C ");
  lcd.print(dark ? "D " : "L ");
  if (autoOffEnabled) {
    lcd.print(remainingTime);
    lcd.print("   "); // Clear remaining characters
  } else {
    lcd.print("INF");
    lcd.print("  "); // Clear remaining characters
  }

  // Turns the fan on
  if ((int)temperature >= tempThreshold && (mode ? dark >= 1 : true)) {
    analogWrite(ENABLE, 255); // enable on
    digitalWrite(DIRA, HIGH); // one way
    digitalWrite(DIRB, LOW);

    colour(BLUE);
  } else {
    // Turns the fan off
    digitalWrite(ENABLE, LOW); // slow stop
    colour(RED);
  }

  delay(1000);
}
