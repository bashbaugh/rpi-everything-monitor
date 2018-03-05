#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define DHT11_PIN 7
#define led 13
int num = false;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
dht DHT;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  lcd.begin(16,2); 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   everything   ");
  lcd.setCursor(0,1); 
  lcd.print("    monitor    ");
  delay(3000);
  lcd.clear();
  delay(500);
}

void flushSerial() {
  while(Serial.available()) {Serial.read();}
}

int response(int timeout) {
  int timepassed = 0;
  int val = false;
  while(!Serial.available()) {
    delay(10);
    timepassed += 10;
    if (timepassed > timeout) {return(false);}
  }
  val = Serial.read();
  if (timepassed < timeout) {delay(timeout - timepassed);}
  //Serial.println(val);
  //Serial.flush();
  return(val);
}

void loop(){
  num = false;
  int chk = DHT.read11(DHT11_PIN);
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print((int) DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print((int) DHT.humidity);
  lcd.print("%");
  flushSerial();
  Serial.println("bd");
  num = response(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Boulder traffic:");
  lcd.setCursor(0,1);
  if (!num == false) {
    lcd.print(num);
  } else {
    lcd.print("failed to find");
  }
  delay(3000);
  flushSerial();
  Serial.println("r");
  num = response(110);
  if (num == 1) {digitalWrite(led, HIGH);} else {digitalWrite(led, LOW);}
}
