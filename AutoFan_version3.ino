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
bool mode = true; // true: LIGHT_DEPENDENT, false: LIGHT_INDEPENDENT

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
      if (tempThreshold == 20)
        break;
      tempThreshold -= 1;
      break;
    case 0xFFE21D: 
      Serial.println("CH+");
      if (tempThreshold == 40)
        break;
      tempThreshold += 1;
      break;
    case 0xFF629D: 
      Serial.println("CH");
      mode = !mode;
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
}

void colour(int colour) {
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(colour, HIGH);
}

void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

  // Sets the cursor to column 0, line 1
  lcd.setCursor(0, 1);

  // Reads the temperature
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  int temperature = tempC;

  int reading = analogRead(lightpin);
  int dark = reading / 114;
  if (dark > 1) dark = 1;

  // Prints it
  lcd.print((int)temperature);
  lcd.setCursor(3, 1);
  lcd.print("*C; dark:");
  lcd.setCursor(13, 1);
  lcd.print(dark);

  // Display mode and temperature threshold on LCD
  lcd.setCursor(0, 0);
  lcd.print("Mode:");
  lcd.setCursor(6, 0);
  lcd.print(mode ? "LD" : "LI");
  lcd.setCursor(11, 0);
  lcd.print("Th:");
  lcd.print(tempThreshold);

  // Turns the fan on
  if ((int)temperature >= tempThreshold && (mode ? dark >= 1 : true)) {
    analogWrite(ENABLE,255); // enable on
    digitalWrite(DIRA,HIGH); // one way
    digitalWrite(DIRB,LOW);

    colour(BLUE);
  } else {
    // Turns the fan off
    digitalWrite(ENABLE,LOW); // slow stop
    colour(RED);
  }

  delay(1000);
}
