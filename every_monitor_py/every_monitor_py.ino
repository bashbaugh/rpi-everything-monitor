#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <avr/sleep.h>

#define DHT11_PIN 7
#define led 13
#define powerbutton 2
#define powerpin 8
int num = false;
volatile boolean poweroff = false;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
dht DHT;


void sleep()
    {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();  // Disable interrupts
    sleep_enable();
    sleep_mode();
    }
 
void powerdown() {
  digitalWrite(led, LOW);
  pinMode(powerpin, OUTPUT);
  digitalWrite(powerpin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shutting down...");
  delay(5000);
  pinMode(powerpin, INPUT);
  delay(10000);
  lcd.setCursor(0,1);
  lcd.print("Unplug Power");
  delay(60000);
  lcd.noBacklight();
  sleep();
}

void power() {
  digitalWrite(led, LOW);
  poweroff = true;
}

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(powerbutton, INPUT);
  attachInterrupt(0, power, HIGH);
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

void checkConnection() {
  delay(200);
  flushSerial();
  Serial.println("r");
  num = response(110);
  if (num == 1) {digitalWrite(led, HIGH);} else {digitalWrite(led, LOW);}
  delay(200);
}

void loop(){
  if(poweroff){powerdown();}
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
  Serial.println("lt");
  num = response(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LC traffic:");
  lcd.setCursor(0,1);
  if (!num == false) {
    lcd.print(num);
  } else {
    lcd.print("failed to find");
  }
  flushSerial();
  Serial.println("at");
  num = response(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Apex traffic:");
  lcd.setCursor(0,1);
  if (!num == false) {
    lcd.print(num);
  } else {
    lcd.print("failed to find");
  }
  flushSerial();
  Serial.println("dt");
  num = response(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ballet traffic:");
  lcd.setCursor(0,1);
  if (!num == false) {
    lcd.print(num);
  } else {
    lcd.print("failed to find");
  }
  delay(3000);
  checkConnection();
}
