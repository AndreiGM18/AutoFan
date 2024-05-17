// Copyright Mitran Andrei-Gabriel 2023
#include <LiquidCrystal.h>
#include <SimpleDHT.h>

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define RED 13
#define YELLOW 6
#define BLUE 1

int lightpin = 0;

// Initializes the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// For DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;

void setup() {
  // Sets up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Chill out bro");

  // Sets pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void colour(int colour) {
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(BLUE, LOW);

  digitalWrite(colour, HIGH);
}

void loop() {
  // Sets the cursor to column 0, line 1
  lcd.setCursor(0, 1);

  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};

  // Reads the temperature
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    return;
  }

  int reading = analogRead(lightpin);
  int dark = reading / 114;
  if (dark > 1) dark = 1;

  // Prints it
  lcd.print((int)temperature);
  lcd.setCursor(3, 1);
  lcd.print("*C; dark:");
  lcd.setCursor(13, 1);
  lcd.print(dark);

  // Turns the fan on
  if ((int)temperature >= 28 && dark >= 1) {
    analogWrite(ENABLE,255); // enable on
    digitalWrite(DIRA,HIGH); // one way
    digitalWrite(DIRB,LOW);

    colour(BLUE);
  }

  if ((int)temperature >= 28 && dark < 1) {
    analogWrite(ENABLE,128); // enable on
    digitalWrite(DIRA,HIGH); // one way
    digitalWrite(DIRB,LOW);

    colour(YELLOW);
  }

  // Turns the fan off
  if ((int)temperature < 28) {
    digitalWrite(ENABLE,LOW); // slow stop
    colour(RED);
  }

  delay(1000);
}

