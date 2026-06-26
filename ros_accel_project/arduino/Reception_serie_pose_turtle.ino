#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Adresse LCD (souvent 0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

String inputString = "";
bool stringComplete = false;

// données
float x = 0;
float y = 0;
float theta = 0;
String dir = "";

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  delay(1000);
  lcd.clear();
}

void loop() {

  // lecture série
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      stringComplete = true;
      break;
    } else {
      inputString += inChar;
    }
  }

  if (stringComplete) {

    parseData(inputString);
    inputString = "";
    stringComplete = false;

    displayData();
  }
}

void parseData(String data) {

  // format attendu : x,y,theta

  int firstComma = data.indexOf(',');
  int secondComma = data.lastIndexOf(',');

  if (firstComma == -1 || secondComma == -1) return;

  x = data.substring(0, firstComma).toFloat();
  y = data.substring(firstComma + 1, secondComma).toFloat();
  theta = data.substring(secondComma + 1).toFloat();

  // conversion direction
  dir = getDirection(theta);
}

String getDirection(float t) {

  // normalisation simple
  if (t > -0.78 && t <= 0.78) {
    return "E";   // droite
  }
  else if (t > 0.78 && t <= 2.35) {
    return "N";   // haut
  }
  else if (t <= -0.78 && t > -2.35) {
    return "S";   // bas
  }
  else {
    return "W";   // gauche
  }
}

void displayData() {

  lcd.clear();

  // ligne 1
  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.print(x, 1);

  lcd.print(" Y:");
  lcd.print(y, 1);

  // ligne 2
  lcd.setCursor(0, 1);
  lcd.print("DIR:");
  lcd.print(dir);
}