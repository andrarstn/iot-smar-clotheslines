#define BLYNK_PRINT Serial
#include <LiquidCrystal.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

//Motor
int IN1=9;
int IN2=10;

//Sensor
int sensor_hujan = A0;
int ldr = 7;

//LED
int led1 = 11;
int led2 = 12;

//Helpers
int status = 0;
int tombol_auto = 0;
int notif = 0;

char auth[] = "-BxbVBm0aHLFTaGfrsK-0PkBUQFpijQs";
char ssid[] = "Private";
char pass[] = "AndrA0127";

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(5,6); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

WidgetLCD lcd1(V5);

void auto_jemuran(){
  lcd1.clear();
  int nilai_sensor_hujan = analogRead(sensor_hujan);
    int nilai_sensor_cahaya = digitalRead(ldr);
    if(nilai_sensor_hujan <= 400 && status!=1){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      lcd1.print(0,0, "AUTOMATIC");
      lcd1.print(0,1, "Hujan");
      Blynk.notify("Hujan Auto");
      digitalWrite(led1, HIGH); 
      delay(2000);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(led1, LOW);
      status =1;
    }else if(nilai_sensor_hujan > 400 && nilai_sensor_cahaya == 0 && status!=2){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      lcd1.print(0,0, "AUTOMATIC");
      lcd1.print(0,1, "Panas");
      Blynk.notify("Panas Auto"); 
      digitalWrite(led2, HIGH);
      delay(2000);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(led2, LOW);
      status = 2;
    }else if(nilai_sensor_hujan > 400 && nilai_sensor_cahaya == 1  && status!=3){     
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      lcd1.print(0,0, "AUTOMATIC");
      lcd1.print(0,1, "Mendung");  
      Blynk.notify("Mendung Auto");
      digitalWrite(led1, HIGH); 
      delay(2000);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(led1, LOW);
      status=3;
    }else{
      Serial.println("6 else");      
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);    
    }
}

void setup () {
  Serial.begin(9600);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  
  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8080);

  lcd1.clear();
  
  pinMode(sensor_hujan, INPUT);
  pinMode(ldr, INPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
}
 
void loop() {
  Blynk.run();
  Serial.println(analogRead(sensor_hujan));
  if(tombol_auto == 1){
    auto_jemuran();
  }else{
    lcd1.clear();  
    lcd1.print(0, 0, "Cuaca : "); 
    Serial.println(digitalRead(ldr));
    
    int nilai_sensor_hujan= analogRead(sensor_hujan);
    if (nilai_sensor_hujan <= 400){
      lcd1.print(8,0, "Hujan");
      if(notif!=1){
        Blynk.notify("Hujan");
      }
      notif = 1; 
    }else {
      lcd1.print(8,0, "Tidak Hujan");
    }
    
    int nilai_sensor_cahaya= digitalRead(ldr); 
    if(nilai_sensor_cahaya == 0){
      lcd1.print(0,1,"Kondisi : Terang");
    }
    else{
      lcd1.print(0,1,"Kondisi : Mendung");
      if(notif!=2){
        Blynk.notify("Mendung");
      }
      notif = 2;
    }
  }
}

//Keluar
BLYNK_WRITE(V1){
  int y = param.asInt();
  if(y == 1){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(led2, HIGH);
    delay(2000);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(led2, LOW);
    Blynk.virtualWrite(V1, LOW);
  }
  else{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
  }
}


//Masuk
BLYNK_WRITE(V2){
  int y = param.asInt();
  if(y == 1){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(led1, HIGH);
    delay(2000);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(led1, LOW);
    Blynk.virtualWrite(V2, LOW);
  }
  else{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
  }
}

//AUTO
BLYNK_WRITE(V3){
  int y = param.asInt();
  if(y == 1){
    tombol_auto = 1;
  }else{
    tombol_auto = 0;
  }
}
