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
 
float Second;
float Minute;
float Hour;
float shh;

// Firebase
 FirebaseData firebaseData;
 #define Lamp_saklar1 5 //D1
 #define Lamp_saklar2 4 //D2
 #define Saklar1 0 //D3
 #define Saklar2 2 //D4
 String val1;
 String val2;
 float onDetik1,onJam1,onMenit1,onDetik2,onJam2,onMenit2;
 float offDetik1,offJam1,offMenit1,offDetik2,offJam2,offMenit2;
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
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now); 
    Second =  p_tm->tm_sec;
    Minute = p_tm->tm_min;
    Hour = p_tm->tm_hour;    
    Serial.println(Second);
    Serial.println(Minute);
    Serial.println(Hour);    
    
            
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
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon1/Menit");
    onMenit1 = firebaseData.floatData(); Serial.print('\n');
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon1/Jam");
    onJam1 = firebaseData.floatData(); Serial.print('\n');  
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon1/Detik");
    onDetik1 = firebaseData.floatData(); Serial.print('\n');           

     if((Minute==onMenit1) && (Second>onDetik1) && (Hour==onJam1)){ 
      digitalWrite(Saklar1,HIGH);
      digitalWrite(Lamp_saklar1,HIGH);
      Firebase.setString(firebaseData, "Product/1BFOAB5PL482/Saklar1","1");      
    }

  //Firebase TimeSaklar2 On
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon2/Menit");
    onMenit2 = firebaseData.floatData(); Serial.print('\n');
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon2/Jam");
    onJam2 = firebaseData.floatData(); Serial.print('\n');  
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeon2/Detik");
    onDetik2 = firebaseData.floatData(); Serial.print('\n');           

     if((Minute==onMenit2) && (Second>onDetik2) && (Hour==onJam2)){ 
      digitalWrite(Saklar2,HIGH);
      digitalWrite(Lamp_saklar2,HIGH);
      Firebase.setString(firebaseData, "Product/1BFOAB5PL482/Saklar2","1");      
    }               
  
  //Firebase TimeSaklar1 Off    
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff1/Menit");
    offMenit1 = firebaseData.floatData(); Serial.print('\n');
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff1/Jam");
    offJam1 = firebaseData.floatData(); Serial.print('\n');  
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff1/Detik");
    offDetik1 = firebaseData.floatData(); Serial.print('\n');           

     if((Minute==offMenit1) && (Second>offDetik1) && (Hour==offJam1)){ 
      digitalWrite(Saklar1,LOW);
      digitalWrite(Lamp_saklar1,LOW);
      Firebase.setString(firebaseData, "Product/1BFOAB5PL482/Saklar1","0");      
    }
    
  //Firebase TimeSaklar1 Off    
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff2/Menit");
    offMenit2 = firebaseData.floatData(); Serial.print('\n');
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff2/Jam");
    offJam2 = firebaseData.floatData(); Serial.print('\n');  
    Firebase.getFloat(firebaseData, "Product/1BFOAB5PL482/Timeoff2/Detik");
    offDetik2 = firebaseData.floatData(); Serial.print('\n');           

    if((Minute==offMenit2) && (Second>offDetik2) && (Hour==offJam2)){ 
    digitalWrite(Saklar2,LOW);
    digitalWrite(Lamp_saklar2,LOW);
    Firebase.setString(firebaseData, "Product/1BFOAB5PL482/Saklar2","0");      
    }                       
}
