#include <SoftwareSerial.h>
#include <LiquidCrystal_PCF8574.h>
#include <DHT.h>
#include <LedFlasher.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int blinkRed = 13; 
int blinkGreen = 12;

int light;

float Vsig;

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

String AP = "";      
String PASS = ""; 
String API = "";   
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

SoftwareSerial esp8266(9,10); 
 
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float pressure = 0;

void setup() {
    pinMode(8,INPUT);
  starttime = millis();
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  lcd.setBacklight(255);
  lcd.begin(16, 2);
  lcd.print("Group 5");
  //delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");
    lcd.clear();
  dht.begin();

  pinMode(A0,INPUT);


}
void loop() {

    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    light=analogRead(A0);
    Serial.println("Light = " + String(light));

    duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    Serial.print("Concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    lowpulseoccupancy = 0;
    starttime = millis();
  }

      int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A5);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 Serial.print("The voltage value: ");
 Serial.print(Vsig);
 Serial.print(" mV    --   ");
    
    Serial.println();

            lcd.display();
              lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Temperature=");
            lcd.print(temp);
            lcd.setCursor(0, 1); // set the cursor to column 0, line 2
            lcd.print("Humidity=");
            lcd.print(hum);
            delay(1000);
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Light=");
            lcd.print(light);
            delay(1000);

            lcd.display();
            lcd.setCursor(0, 0);
            lcd.print("UV= ");
            lcd.print(Vsig);
            delay(1000);

            lcd.display();
            lcd.setCursor(0, 0);
            lcd.print("Concentration = ");
            lcd.setCursor(0, 1); // set the cursor to column 0, line 2
            lcd.print(concentration );

            
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+temp+"&"+ field2 +"="+hum +"&"+ field3 +"="+light +"&"+ field4 +"="+Vsig +"&"+ field5 +"="+concentration;
sendCommand("AT+CIPMUX=1",5,"OK");
sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
esp8266.println(getData);
countTrueCommand++;
sendCommand("AT+CIPCLOSE=0",5,"OK");
    
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");

  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
