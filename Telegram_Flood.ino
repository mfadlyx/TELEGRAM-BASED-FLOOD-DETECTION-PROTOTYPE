//Library function I2C dan set alamat LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Library ESP8266 dan Bot Telegram
#include <ESP8266WiFi.h>
#include "CTBot.h"
CTBot my_scurity;

//Library servo dan posisi awal
#include <Servo.h>
Servo myservo;
int pos = 0;

//Definisi sensor dan perangkat lain
int buzz=13; //Buzzer terpasang pada pin D7
const int trigP= 14; //Trigger pada sensor ultrasonik terpasang pada pin D5
const int echoP= 12; //Echo pada sensor ultrasonik terpasang pada pin D6

//SCL pada D1 dan SDA pada D2

//Definisi variabel
long duration; //Membuat variabel duration untuk durasi
int distance; //Membuat variabel distance untuk jarak
String balas; //Membuat variabel balas untuk pemberitahuan bot telegram
String balas1; //Membuat variabel balas1 untuk pemberitahuan bot telegram
String balas2; // Membuat variabel balas2 untuk pemberitahuan bot telegram

//Memasang SSID dan Pass untuk koneksi ESP8266 dan token telegram
const char *ssid = "INPUT SSID"; 
const char *pass = "INPUT PASSWORD ID";
String token = "INPUT TOKEN TELEGRAM";

WiFiClient Client;

void setup(){
  lcd.begin(); //Memanggil LCD
  pinMode(trigP, OUTPUT);     // Set Trigger Pin sebagai Output
  pinMode(echoP, INPUT);      // Set Echo Pin sebagai Input
  pinMode(buzz, OUTPUT);      // Set Buzzer sebagai output 
  myservo.attach(15);          // Set Servo pada pin D8
  Serial.begin(9600);

 //Menghubungkan Jaringan WiFi dengan ssid dan password
  my_scurity.wifiConnect(ssid, pass);
  my_scurity.setTelegramToken(token);
  if(my_scurity.testConnection())
  Serial.print(" Menghubungkan ke : ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("ů.");
  }
  Serial.print("\n");
  Serial.print("IP address : ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
  Serial.println("");
  Serial.print("Terhubung dengan : ");
  Serial.println(ssid);
}

void loop(){ 
  TBMessage pesan;
    if(my_scurity.getNewMessage(pesan))
    {
    Serial.println("INBOX: ");
    Serial.println(pesan.text);
    if(pesan.text.equalsIgnoreCase("cek")){
      my_scurity.sendMessage(pesan.sender.id,"OK,Siap...");
    }
      else
      {
        String balas;
        balas="Info...Sistem Peringatan Dini Bahaya Banjir telah siap";
        my_scurity.sendMessage(pesan.sender.id,balas);
      }
    }
  {
    digitalWrite(trigP, LOW);            // Set Trig Pin menjadi Low 
    delayMicroseconds(2);                // Delay 2Ms
    digitalWrite(trigP, HIGH);           // Set Trig Pin menjadi High
    delayMicroseconds(10);               // Delay 10Ms
    digitalWrite(trigP, LOW);            // Set Trig Pin menjadi Low kembali 
    duration = pulseIn(echoP, HIGH);     // Membaca Echo Pin
    distance= duration*0.034/2;          // Membuat rumus untuk jarak
    Serial.print("Distance = ");         // Memprint Jarak pada serial monitor 
    Serial.println(distance);
    delay(1000);
    
  if (distance <= 10 && distance >= 0) // kondisi BANJIR
    {
      lcd.setCursor(0,0);
      lcd.print("STATUS BANJIR");
      lcd.setCursor(0,1);
      lcd.print("PINTU TERBUKA!");
      myservo.write(180);
      digitalWrite(buzz,HIGH);
      balas1="INFO...!!! Ketinggian air =  " + String (distance) + " cm. Kondisi sudah memasuki status BANJIR dan pintu air sudah terbuka";
      my_scurity.sendMessage(pesan.sender.id,balas1);
    }
  else if (distance >= 11 && distance <= 20) // kondisi SIAGA
    {
      lcd.setCursor(0,0);
      lcd.print("STATUS SIAGA!");
      lcd.setCursor(0,1);
      lcd.print("PINTU TERBUKA!");
      myservo.write(180);
      digitalWrite(buzz,LOW);
      balas2="INFO...!!! Ketinggian air = " + String (distance) + " cm. Kondisi sudah memasuki status SIAGA dan pintu air sudah terbuka";
      my_scurity.sendMessage(pesan.sender.id,balas2);
    }
  else // Kondisi AMAN
    {
      lcd.setCursor(0,0);
      lcd.print("STATUS AMAN!!");
      lcd.setCursor(0,1);
      lcd.print("PINTU TERTUTUP");
      myservo.write(0);
      digitalWrite(buzz,LOW);
     } 
   }
   delay(500); 
}
