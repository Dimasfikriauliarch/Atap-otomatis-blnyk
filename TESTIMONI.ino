#define BLYNK_PRINT Serial
#include <Servo.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 13 // d7
// inialisasi servo
Servo servo1;

// inialisasi pin sensor
const int pinHujan = D0 ;
const int pinLdr = D1 ;

// inisialiasi button blynk
int button1;
int button2;

// inialisasi pembacaan sensor
int hujan; 
int cahaya;

// inialisasi derajat buka-tutup servo
int tutup = 180;
int buka = 10;

// Inisialisasi Token dari Blynk
char auth[] = "57pLMRUpCQiFlFFW__Yaa83VVGSO5XJL";

// Inisialisasi Internet/WiFi
char ssid[] = "DIMAS";
char pass[] = "09januari2000";
WidgetLCD lcd1(V5);


DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor() // mengirim nilai dht ke blynk
{ 
  float t= dht.readTemperature();
  Blynk.virtualWrite(V6, t);
}

void setup() {
// setting baud rate serial monitor
Serial.begin(9600);

Blynk.begin(auth, ssid, pass);
dht.begin();
 timer.setInterval(1000L, sendSensor);
// inialisasi pin servo
servo1.attach(D4);


// inialisasi status input/output pin
pinMode(pinHujan, INPUT);
pinMode(pinLdr, INPUT);

lcd1.clear();
lcd1.print(0, 0, "Kondisi : ");

}

void loop() {
Blynk.run();
timer.run();
lcd1.clear();  
lcd1.print(0, 0, "Cuaca : ");

  // pembacaan sensor
hujan = digitalRead(pinHujan);
cahaya = digitalRead(pinLdr);

// menulis pada serial monitor pembacaan sensor hujan dan cahaya
// hasil output data sensor bisa dilihat di serial monitor arduino
Serial.print(hujan);
Serial.print(" ");
Serial.println(cahaya);
Serial.println(" ");


// sensor hujan akan bernilai = 0 saat ada air
// sensor cahaya akan bernilai = 1 saat gelap
// dan berlaku sebaliknya

// jika kondisi Hujan dan Mendung/Gelap
if (hujan == 0 && cahaya == 1)
{
// servo menutup
if (button1 == 1 )
servo1.write(tutup);

// // Menampilkan teks pada LCD Blynk
lcd1.print(0,0, "Cuaca: Hujan");
lcd1.print(0,1, "Kondisi: malam");

}

//=================================================

// jika kondisi Hujan dan Terang
else if (hujan == 0 && cahaya == 0)
{
// servo menutup
if (button1 == 1 )
servo1.write(tutup);

// // Menampilkan teks pada LCD Blynk
lcd1.print(0,0, "Cuaca: Hujan");
lcd1.print(0,1, "Kondisi: Siang");
Blynk.notify("LAPORAN TERKINI, CUACA HUJAN KONDISI SIANG MOHON TUTUP ATAP!");
}

//==================================================

// jika kondisi Cerah/Tidak Hujan dan Gelap
else if (hujan == 1 && cahaya == 1)
{
// servo menutup
if (button1 == 1 )
servo1.write(tutup);

// // Menampilkan teks pada LCD Blynk
lcd1.print(0,0, "Cuaca: Cerah");
lcd1.print(0,1, "Kondisi: Malam");
}


//==================================================

// jika kondisi Cerah/Tidak Hujan dan Terang
else if (hujan == 1 && cahaya == 0)
{
// servo membuka
 if (button2 == 1 && button1 == 1)
servo1.write(buka); 

// Menampilkan teks pada LCD Blynk
lcd1.print(0,0, "Cuaca: Cerah");
lcd1.print(0,1, "Kondisi: Siang");
Blynk.notify("LAPORAN TERKINI, CUACA CERAH KONDISI SIANG MOHON BUKA ATAP!");
}

// delay jalannya program
delay(300);
  // put your main code here, to run repeatedly:

}

// Button Buka pada Blynk ditekan
BLYNK_WRITE(V1){
  button1 = param.asInt();
servo1.write(buka);
}

// Button Tutup pada Blynk ditekan
BLYNK_WRITE(V2){
  button2 = param.asInt();
servo1.write(tutup);
}
