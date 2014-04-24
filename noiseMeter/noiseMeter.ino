#include <MsTimer2.h>
#include <WiFi.h>

// init Sensor Param
int timeCounter = 0;
const int ledPinLoudness=8;
const int ledPinVibration=9;
int vibCounter = 0;
int vibSensorValue = 0;
float loudnessCounter = 0;
float loudnessSensorValue = 0;

// init Wifi
char ssid[] = "HCI_ARDUINO_HW";     // the name of your network
char server[] = "192.168.0.2";
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiClient client;

void HttpRequest(String PostData) {
  client.stop();
  if (client.connect(server,8888)) {
    Serial.print("  Connect  ");
    client.println("POST /upload HTTP/1.1");
//    client.println("Host: 192.168.0.2");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);   
  } else {
    Serial.println("  Connect Failed  ");
  }
}

void SendData()
{
  // send data to server
  String PostData = String("[")+String(vibCounter)+String(",")+String(int(loudnessCounter))+String("]");
  vibCounter = 0;
  loudnessCounter = 0;  
  Serial.println(PostData);
  HttpRequest(PostData);
  
  // Print data to serial
//  Serial.print("vibration : ");
//  Serial.print(vibCounter);
//  Serial.print("  loudness : ");
//  Serial.println(loudnessCounter);
}

void setup() {
  Serial.begin(9600);

  // Set Wifi Connection
  printMacAddress();
  Serial.println("Attempting to connect to open network...");
  status = WiFi.begin(ssid);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } else { // if you are connected :
    Serial.println("Connected to the network");
  }
  
  // Set Pin mode
  pinMode(ledPinLoudness,OUTPUT);
  pinMode(ledPinVibration,OUTPUT);

  //Set Timer for callback function
  MsTimer2::set(3000,SendData);
  MsTimer2::start();
} 
 
void loop() {
//    timeCounter++;
//    if (timeCounter == 100) {
//      SendData();
//      timeCounter = 0;
//    }
    while(true) {
      vibSensorValue = analogRead(A0);
      CheckVibration();  
      delay(10);
  
      loudnessSensorValue = analogRead(A5);  
      CheckLoudness();
      delay(10);      
    }

}

void CheckLoudness () {
  loudnessCounter += loudnessSensorValue/1000;
  if(loudnessSensorValue > 600) {
    digitalWrite(ledPinLoudness,HIGH);
  } else {
    digitalWrite(ledPinLoudness,LOW);
  } 
}

void CheckVibration () {
  if(vibSensorValue > 100) {
    Serial.println(vibSensorValue);
    vibCounter++;
    digitalWrite(ledPinVibration,HIGH);
  } else {
    digitalWrite(ledPinVibration,LOW);
  }
}


void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];                     

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}
