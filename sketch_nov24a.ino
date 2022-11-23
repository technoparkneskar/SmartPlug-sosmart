#include <PZEM004Tv30.h>
#include <FirebaseESP8266.h>
#include <WiFiManager.h>

//Firebase autentikasi 
 #define FIREBASE_HOST "https://areauji-default-rtdb.firebaseio.com" //Sesuaikan dengan link firebase database kalian, tanpa menggunakan "http:" , "\" and "/"
 #define FIREBASE_AUTH "Bn7LhCTiP8VphRn53K2WmcqlHvV6WnexeRj7vKLa3ns" //Sesuaikan dengan firebase database secret code kalian

//Nama hotspot jika tak tersambung
 #define NamaHotspot "ControlLampFirebase" // Nama hotspot di esp8266

//Pzem sensor
 PZEM004Tv30 pzem(12,13);// D6 dan D7
 float Power,Energy,Voltase,Current;

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
 String zero = "0" , one = "1";

void setup(){
    
Serial.begin(115200);
   
   //WifiManager:
    WiFiManager wm;
    wm.resetSettings();
    bool res;
    res = wm.autoConnect("NamaHotspot");
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
    Firebase.setString(firebaseData, "S2","0");
    
   //PinMode
    pinMode(Saklar1,OUTPUT);
    pinMode(Saklar2,OUTPUT);
    pinMode(Lamp_saklar1,OUTPUT);
    pinMode(Lamp_saklar2,OUTPUT);
}

void loop(){
    
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
   
   //Firebase
    Firebase.getString(firebaseData, "product/1BFOAB5PL482/Saklar1");
    val1 = firebaseData.stringData(); Serial.print('\n');

    Serial.print("Value 1 ");
    Serial.println(val1);

    if(val1==one){ 
     digitalWrite(Saklar1,LOW);
     Serial.println("Saklar1 OFF");
     } 
    else if(val1==zero){ 
     digitalWrite(Saklar2,HIGH);
     Serial.println("Saklar 2 ON");
     }
}