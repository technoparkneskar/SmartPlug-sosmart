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
 #define NamaHotspot "SOSMART-1BFOAB5PL482" // Nama hotspot di esp8266

//Pzem sensor
 PZEM004Tv30 pzem(12,13);// D6 dan D7 rx tx
float Power,Energy,Voltase,Current;
 
float Second;
float Minute;
float Hour;
float shh;

int Tombol;
float fungsi;
float S1;
float S2;

// Firebase
 FirebaseData firebaseData;
 #define Lamp_saklar1 5 //D1
 #define Lamp_saklar2 4 //D2
 #define Saklar1 0 //D3
 #define Saklar2 2 //D4
 bool val1;
 bool val2;

//Internet CLock
WiFiUDP ntpUDP;
int timezone = 7 * 3600;
int dst = 0;

void setup(){
    
Serial.begin(115200);
   
   //Tombol
   pinMode(16, INPUT);

   //WifiManager:
    WiFiManager wm;
    bool res;
    res = wm.autoConnect(NamaHotspot);
    if(!res) {
    Serial.println("Failed to connect");
    wm.resetSettings();
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

   //tombol
   Tombol=digitalRead(16); 

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
        Firebase.setFloat(firebaseData, "Product/1BFOAB5PL482/Power",Power);
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
        Firebase.setFloat(firebaseData, "Product/1BFOAB5PL482/Energy",Energy);
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
        Firebase.setFloat(firebaseData, "Product/1BFOAB5PL482/Voltase",Voltase);
    }
    
   //Read Current
     Current = pzem.current();
    
     if(isnan(Current)){
        Serial.print("0");
     }
     else{
        Serial.print("Current : ");
        Serial.print(Current);
        Serial.println("A");
        Firebase.setFloat(firebaseData, "Product/1BFOAB5PL482/Current",Current);
     }
   
   //Firebase Saklar1
     Firebase.getBool(firebaseData, "Product/1BFOAB5PL482/Switch/1");
     val1 = firebaseData.boolData(); Serial.print('\n');

     if(val1==true){ 
      digitalWrite(Saklar1,HIGH);
      digitalWrite(Lamp_saklar1,HIGH);
      Serial.println("Saklar1 ON");
      S1=1;
      } 
     else if(val1==false){ 
      digitalWrite(Saklar1,LOW);
      digitalWrite(Lamp_saklar1,LOW);
      Serial.println("Saklar 1 OFF");
      S1=0;      
      }
    
   //Firebase Saklar2
     Firebase.getBool(firebaseData, "Product/1BFOAB5PL482/Switch/2");
     val2 = firebaseData.boolData(); Serial.print('\n');

     if(val2==true){ 
      digitalWrite(Saklar2,HIGH);
      digitalWrite(Lamp_saklar2,HIGH);
      Serial.println("Saklar2 ON");
      S2=1;
      } 
     else if(val2==false){ 
      digitalWrite(Saklar2,LOW);
      digitalWrite(Lamp_saklar2,LOW);
      Serial.println("Saklar 2 OFF");
      S2=0;
    }
   //Tombol Emergency
     if(Tombol==1) {
      if(fungsi==1){
      Firebase.setBool(firebaseData, "Product/1BFOAB5PL482/Switch/1",false);
      Firebase.setBool(firebaseData, "Product/1BFOAB5PL482/Switch/2",false);      
      digitalWrite(Saklar1,LOW);
      digitalWrite(Lamp_saklar1,LOW);
      digitalWrite(Saklar2,LOW);
      digitalWrite(Lamp_saklar2,LOW);
      delay(500);      
      fungsi=0;
      }
     else {
      Firebase.setBool(firebaseData, "Product/1BFOAB5PL482/Switch/1",true);
      Firebase.setBool(firebaseData, "Product/1BFOAB5PL482/Switch/2",true);
      digitalWrite(Saklar1,HIGH);
      digitalWrite(Lamp_saklar1,HIGH);
      digitalWrite(Saklar2,HIGH);
      digitalWrite(Lamp_saklar2,HIGH);
      delay(500);      
      fungsi=1;
      }
    }
    
   // FUngsi Emergency
     if(S1==1 && S2==1){
     fungsi=1;       
     }
     else if(S1==0 && S2==0){
     fungsi=0;       
     }
     else if(S1==1 || S2==1){
     fungsi=1;
     }
  delay(100);                      
}
