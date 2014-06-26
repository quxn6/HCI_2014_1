
/*
  WiFi UDP Send and Receive String
 
 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort
 
 Circuit:
 * WiFi shield attached
 
 created 30 December 2012
 by dlf (Metodo2 srl)

 */

//#include <MsTimer2.h>
#include <Timer.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "HCI_ARDUINO_HW"; //  your network SSID (name) 
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;
Timer t;

const int leftPunch=8;
const int rightPunch=9;
const int ledOut = 2;
bool      ledOn = false;

void StopLeftPunchVib()
{
  digitalWrite(leftPunch, LOW);
}

void StopRightPunchVib()
{
  digitalWrite(rightPunch, LOW);
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);  
  
  pinMode(leftPunch,OUTPUT);
  pinMode(rightPunch,OUTPUT);
  pinMode(ledOut, OUTPUT);
}

void loop() {
  t.update();
    
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {   
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer,255);
    if (len >0) packetBuffer[len]=0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    if (strcmp(packetBuffer, "lp") == 0 )
    {
      Serial.println("lp receive");
      digitalWrite(leftPunch, HIGH);
      t.after(500, StopLeftPunchVib);
    }
    if (strcmp(packetBuffer, "rp") == 0 )
    {
      Serial.println("rp receive");
      digitalWrite(rightPunch, HIGH);
      t.after(500, StopRightPunchVib);
    }
    if (strcmp(packetBuffer, "l") == 0 )
    {
      Serial.println("led signal");
      if ( ledOn )
      {
        ledOn = false;
        digitalWrite(ledOut, LOW);
        Serial.println("led OFF");
      }
      else 
      {
        ledOn = true;
        digitalWrite(ledOut, HIGH);
        Serial.println("led ON");        
      }
      
//      ledOn = (ledOn)? false : true;
//      (ledOn)? digitalWrite(ledOut, HIGH) : digitalWrite(ledOut, LOW);
    }
    
    // send a reply, to the IP address and port that sent us the packet we received
//    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.write(ReplyBuffer);
//    Udp.endPacket();
//    Serial.println("Send Packet");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}




