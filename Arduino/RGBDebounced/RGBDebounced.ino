/*
By Sihao Yu, Haohan Shi for PennApps XIV

MODIFIED AND ADAPTED FROM ARDUINO EXAMPLE and WIFI101 EXAMPLE.

 This example connects to an unencrypted Wifi network.
 Then it prints the  MAC address of the Wifi shield,
 the IP address obtained, and other network details.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe

 Circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground
 * 
 *  created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters
 modified 11 Sep 2016

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 and
 https://www.arduino.cc/en/Reference/WiFi101
 */

#include <SPI.h>
#include <WiFi101.h>
#define NOTHING 0xCC
#define WAITING 0xDD
#define DENY 0xEE
#define ACCEPT 0xFF
#define DEVICE_ID "0_65535"
typedef uint8_t state;
// constants won't change. They'reused here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledWait = 4;
const int ledYes = 5;
const int ledNo = 6;

state s = WAITING;
//set Network information
char ssid[] = "Haohan iPhone";     //  your network SSID (name)
char pass[] = "shihaohan";  // your network password

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "yusihao51.pythonanywhere.com";    // name address for Google (using DNS)
IPAddress server(172,20,10,9);
int port = 80;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  // initialize the LED and button pin:
  pinMode(buttonPin, INPUT);
  pinMode(ledWait, OUTPUT);
  pinMode(ledYes, OUTPUT);
  pinMode(ledNo, OUTPUT);

  // set initial LED state
  digitalWrite(ledWait, ledState);
  digitalWrite(ledYes, ledState);
  digitalWrite(ledNo, ledState);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not found");
    // don't continue:
    while (true);
  }
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for retry:
    delay(8000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  sendContent("Initialize Server");
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void ledDisplay(){
//  Serial.println(ledCount);
  if (s == NOTHING) {
    digitalWrite(ledNo, LOW);
    digitalWrite(ledYes, LOW);
    digitalWrite(ledWait, LOW);
    }
  else if (s == WAITING) {
    digitalWrite(ledWait, HIGH);
    digitalWrite(ledYes, LOW);
    digitalWrite(ledNo, LOW);
  }
  else if (s == DENY) {
    digitalWrite(ledWait, LOW);
    digitalWrite(ledYes, LOW);
    digitalWrite(ledNo, HIGH);
  }
  else{
    digitalWrite(ledWait, LOW);
    digitalWrite(ledYes, HIGH);
    digitalWrite(ledNo, LOW);    
  }
}

void changeState(state n) {
  s = n;
}
void sendContent(char* s){
  while(true){
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, port )) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.flush();
      client.write(s);
      Serial.println(s);
      return;
    }
    else{
      delay(3000);
    }
  }
}
void getStatus(char* &buffer, int size){
  //send server an getStatus request
  while(true){
    Serial.println("\nConnecting server for result...");
    // if you get a connection, report back via serial:
    if (client.connect(server, port )) {
      Serial.println("connected to server");
      // Make a HTTP request:
      char* str = sprintf("UPDATE_%s",DEVICE_ID);
      client.write(str);
      Serial.println(s);
      break;
    }
    else{
      delay(1000);
    }
  }
  //wait for server response
  //assume server retain connection
  while(!client.available()){
    delay(500);
  }
  int i = 0;
  while (client.available() && i < size) {
    char c = client.read();
    Serial.write(c);
    buffer[i] = c;
  }

}
void loop() {
  // read the state of the switch into a local variable:
  int curButton = digitalRead(buttonPin);

  if (curButton != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
    if (s == NOTHING) {
      if (curButton != buttonState) {
        buttonState = curButton;
        // only toggle the LED if the new button state is HIGH
        if (buttonState == HIGH) {
          changeState(WAITING);
          ledDisplay();
          char* str = sprintf("Pressed_%s",DEVICE_ID);
          sendContent(str);
          char* buffer = new char[100];
          getStatus(buffer,100);
          if (buffer == "ACCEPTED"){
            changeState(ACCEPT);
          }
          if (buffer == "DENY"){
            changeState(DENY);
          }
        }
        lastButtonState = curButton;
      }
    }
  }
  // '''code to send a normal request'''
  // '''listen to reply from server'''
  // '''call changeState(0x..) when applicable'''
  ledDisplay();
  delay(3000);
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
