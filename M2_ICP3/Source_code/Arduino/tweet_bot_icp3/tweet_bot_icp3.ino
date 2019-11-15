#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial wifi(9,10); 
//#define ID "a" //"SSID-WiFiname" 
//#define PASS "11111111" // "password"
//#define ID "ERROR 404" //"SSID-WiFiname" 
//#define PASS "password1234#" // "password"
#define ID "ApnaTimeAyega"     // "SSID-WiFiname" 
#define PASS "GullyGirls@112"       // "password"

String readtext = "";
String tweet = String("Tweet");
int switchState = 0;

void setup() {
  Serial.begin(115200);
  connectwifi(ID,PASS);
  close_tcp();
  init_tcp("5005");

  pinMode(3, OUTPUT);
  pinMode(5, INPUT);
}

void loop() {
  
  //init_tcp("5005");
  switchState = digitalRead(5);
  if(switchState == HIGH){
  // the button is not pressed
    delay(100);
    send_data_tcp("Tweet:from arduino via Raspberry pi\n");
    delay(50);
  }
  
  else{
    if (wifi.available()) 
      {  
        String readtext = (wifi.readString());
        readtext.remove(1,8);
        readtext.trim();
        Serial.println(readtext);
        if(wifi.find("+IPD,10:Tweet_Sent"))
          {
           digitalWrite(3, HIGH);
           delay(5000);
           digitalWrite(3, LOW); 
          }
        
//        
//          if(readtext == tweet){
//            Serial.println("Yaay received Tweet from rapberry Pi\n");
//            digitalWrite(3, HIGH);
//            delay(5000);
//            digitalWrite(3, LOW);
         else 
          {
    //      Serial.println("arduino: not posted");
            digitalWrite(3, LOW);
          }
       }
      else {digitalWrite(3, LOW);}
    }
}

///////////////////////////////////////////// USE: connectwifi(ssid,password)
boolean connectwifi(String s, String p){
  wifi.begin(115200);
  wifi.write("AT\r\n");
  if (wifi.find("OK")) {
    Serial.println("wifi hardware found");
    wifi.println("AT+CWMODE=1");
    delay(500);
    wifi_try:
    wifi.println("AT+CWJAP?");
    delay(500);
    if (wifi.find("No AP")){
      Serial.println("No wifi connected.. trying to connect");
      String cmd="AT+CWJAP=\"";cmd+=s;cmd+="\",\"";cmd+=p;cmd+="\"";
      wifi.println(cmd);delay(5000);
      if (wifi.find("CONNECTED")){Serial.println("wifi Connected");}
    }
    else {
      Serial.println("Wifi Already Connected");
      }
  }
  else {Serial.println("Check connection. wifi device not found.");}
  }
////////////////////////////////////////

boolean close_tcp(){
  String cmd = "AT+CIPCLOSE";
  wifi.println(cmd);
}

boolean init_tcp(String port){
  String cmd = "AT+CIPSTART=\"TCP\",\"192.168.1.205\","+port;  //put IP address of Raspberry Pi
  wifi.println(cmd);
}

boolean send_data_tcp(String Data){
  String cmd = "AT+CIPSTART=\"UDP\",\"192.168.1.205\",5005";
  //wifi.println(cmd);
  wifi.println("AT+CIPSEND="+String(Data.length()));
  Serial.println(Data);
  wifi.println(Data);
  //close_tcp();
  }

  



    











  
