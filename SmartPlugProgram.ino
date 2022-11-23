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
 #define lamp_saklar1 5 //D1
 #define lamp_saklar2 4 //D2
 #define saklar1 0 //D3
 #define saklar1 2 //D4
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

     // ESP.restart();

    } 

    else {

        //if you get here you have connected to the WiFi    

        Serial.println("connected...yeey :)");

    }
    WiFi.mode(WIFI_STA);


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
}
