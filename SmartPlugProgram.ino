#include <PZEM004Tv30.h>
#include <FirebaseESP8266.h>
#include <WiFiManager.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Firebase autentikasi 
 #define FIREBASE_HOST "https://sosmart-technopark-default-rtdb.firebaseio.com/" //Sesuaikan dengan link firebase database kalian, tanpa menggunakan "http:" , "\" and "/"
 #define FIREBASE_AUTH "afL12P1vXA0goN4ia42rsC4DUYLnNMqI19XRIye6" //Sesuaikan dengan firebase database secret code kalian

//Nama hotspot jika tak tersambung
 #define NamaHotspot "ControlLampFirebase" // Nama hotspot di esp8266

//Pzem sensor
 PZEM004Tv30 pzem(12,13);// D6 dan D7
 float Power,Energy,Voltase,Current;
 
String Second;
String Minute;
String Hour;
String shh;

// Firebase
 FirebaseData firebaseData;
 #define Lamp_saklar1 5 //D1
 #define Lamp_saklar2 4 //D2
 #define Saklar1 0 //D3
 #define Saklar2 2 //D4
 String val1;
 String val2;
 String val3;
 String val4;
 String val5;
 String val6;
 String zero = "0" , one = "1";

//Internet CLock
WiFiUDP ntpUDP;
int timezone = 7 * 3600;
int dst = 0;

void setup(){
    
Serial.begin(115200);
   
   //WifiManager:
    WiFiManager wm;
    bool res;
    res = wm.autoConnect(NamaHotspot);
    if(!res) {
    Serial.println("Failed to connect");
    } 
    else {
     Serial.println("connected...yeey :)");
    }
    WiFi.mode(WIFI_STA);
    
   //Firebase
    Serial.print('\n');
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to firebase
    Firebase.reconnectWiFi(true);
    
   //PinMode
    pinMode(Saklar1,OUTPUT);
    pinMode(Saklar2,OUTPUT);
    pinMode(Lamp_saklar1,OUTPUT);
    pinMode(Lamp_saklar2,OUTPUT);
   
   //Internet clock begin
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);     
    Second =  p_tm->tm_sec;
    Minute = p_tm->tm_min;
    Hour = p_tm->tm_hour;
    Serial.print(Second);  
}

void loop(){
    shh=8;
   //Update time    
    Serial.print(Second);
    Serial.print(Minute);
    Serial.println(Hour);    
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);     
    Second =  p_tm->tm_sec;
    Minute = p_tm->tm_min;
    Hour = p_tm->tm_hour;

            
   //Read power
    Power = pzem.power();
    
    if(isnan(Power)){
        Serial.print("Mati");
    }
    else{
        Serial.print("Power : ");
        Serial.print(Power);
        Serial.println("kW");
    }
    
   //Read Energy
    Energy = pzem.energy();
    
    if(isnan(Energy)){
        Serial.print("Mati");
    }
    else{
        Serial.print("Energy : ");
        Serial.print(Energy);
        Serial.println("kWh");
    }
    
   //Read Voltase
    Voltase = pzem.voltage();
    
    if(isnan(Voltase)){
        Serial.print("Mati");
    }
    else{
        Serial.print("Voltase : ");
        Serial.print(Voltase);
        Serial.println("V");
    }
    
    //Read Current
     Current = pzem.current();
    
     if(isnan(Current)){
        Serial.print("Mati");
     }
     else{
        Serial.print("Current : ");
        Serial.print(Current);
        Serial.println("A");
     }
   
    //Firebase Saklar1
     Firebase.getString(firebaseData, "Product/1BFOAB5PL482/Saklar1");
     val1 = firebaseData.stringData(); Serial.print('\n');

     Serial.print("Value 1 ");
     Serial.println(val1);

     if(val1==one){ 
      digitalWrite(Saklar1,HIGH);
      digitalWrite(Lamp_saklar1,HIGH);
      Serial.println("Saklar1 ON");
      } 
     else if(val1==zero){ 
      digitalWrite(Saklar1,LOW);
      digitalWrite(Lamp_saklar1,LOW);
      Serial.println("Saklar 1 OFF");
      }
    
    //Firebase Saklar2
     Firebase.getString(firebaseData, "Product/1BFOAB5PL482/Saklar2");
     val2 = firebaseData.stringData(); Serial.print('\n');

     Serial.print("Value 2 ");
     Serial.println(val2);

     if(val2==one){ 
      digitalWrite(Saklar2,HIGH);
      digitalWrite(Lamp_saklar2,HIGH);
      Serial.println("Saklar2 ON");
      } 
     else if(val2==zero){ 
      digitalWrite(Saklar2,LOW);
      digitalWrite(Lamp_saklar2,LOW);
      Serial.println("Saklar 2 OFF");
    }

    //Firebase TimeSaklar1 On
     Firebase.getString(firebaseData, "Product/1BFOAB5PL482/Timeon1/Detik");
     val3 = firebaseData.stringData(); Serial.print('\n');
     Firebase.getString(firebaseData, "Product/1BFOAB5PL482/Timeon1/Jam");
     val4 = firebaseData.stringData(); Serial.print('\n');
     Firebase.getString(firebaseData, "Product/1BFOAB5PL482/Timeon1/Menit");
     val5 = firebaseData.stringData(); Serial.print('\n');
     val6 = val3+10;          

     if(Second>val3){ 
      digitalWrite(Saklar1,HIGH);
      digitalWrite(Lamp_saklar1,HIGH);
      Serial.println("Saklar1 ON");
      Firebase.setString(firebaseData, "Product/1BFOAB5PL482/Saklar1","1");      
    }             
}
