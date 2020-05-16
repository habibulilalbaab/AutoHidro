/*
 Repository: https://github.com/intprism/AutoHidro
 created 12 May 2020
 by Muhammad Habib Ulil Albaab
*/
// Load DHT Library & init
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
//Init Ultrasonic
#define sensorIR A1

DHT dht(DHTPIN, DHTTYPE);
float temp, waterLevel, waterLevelCm;
int LDRSensor, currentDay = 1, uptime = millis();
unsigned long dayInMillis = 86400000, currentDayIntMillis;
const int RELAYFAN = 6;
const int RELAYLED = 4;
const int WATERLEVEL = 3;
const int LDRSENSOR = A0;
const int NUTRITIONPUMP = 5;

void setup() {
  Serial.begin(9600);
  pinMode(RELAYFAN, OUTPUT);
  pinMode(RELAYLED, OUTPUT);
  pinMode(WATERLEVEL, OUTPUT);
  pinMode(LDRSENSOR, INPUT);
  pinMode(NUTRITIONPUMP, OUTPUT);
}

void loop() {
  // Cek Suhu
  temp = dht.readTemperature();
  if(isnan(temp)){
    Serial.println("DHT11 Tidak terdeteksi !");
  }else{
    if(temp >= 35.00){
      // Relay Fan On
      Serial.print("Temperatur = ");
      Serial.print(temp);
      Serial.println("C - Fan On");
      digitalWrite(RELAYFAN, HIGH);
    }else{
      // Relay Fan Off
      Serial.print("Temperatur = ");
      Serial.print(temp);
      Serial.println("C - Fan Off");
      digitalWrite(RELAYFAN, LOW);
    }
  }
  // Cek Level Air
  waterLevel = analogRead(sensorIR);
  waterLevelCm = 10650.08*pow(waterLevel,-0.935)-10;
  if(waterLevelCm >= 2.00){
    // LED notifikasi level air nyala
    Serial.print("Level air dari sensor = ");
    Serial.print(waterLevelCm);
    Serial.println("CM - Level Air Rendah !");
    digitalWrite(WATERLEVEL, HIGH);
  }else{
    // LED notifikasi level air mati
    Serial.print("Level air dari sensor = ");
    Serial.print(waterLevelCm);
    Serial.println("CM - Level Air Normal !");
    digitalWrite(WATERLEVEL, LOW);
  }
  // Cek Intensitas
  LDRSensor = analogRead(LDRSENSOR);
  if(LDRSensor <= 200){
    // Relay led on
    Serial.println("Intensitas rendah, LED Menyala !");
    digitalWrite(RELAYLED, HIGH);
  }else{
    // Relay led off
    Serial.println("Intensitas normal, LED Mati !");
    digitalWrite(RELAYLED, LOW);
  }
  // Pompa Nutrisi
  uptime = millis();
  currentDayIntMillis = dayInMillis*currentDay;
  if(uptime/currentDayIntMillis > currentDay){
    // Pompa nutrisi on 2 second
    digitalWrite(NUTRITIONPUMP, HIGH);
    delay(2000);
    digitalWrite(NUTRITIONPUMP, LOW);
    currentDay = currentDay+1;
  }else{
    digitalWrite(NUTRITIONPUMP, LOW);
  }
}
