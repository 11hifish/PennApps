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

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledWait = 4;
const int ledYes = 5;
const int ledNo = 6;

//set Network information
char ssid[] = "Haohan iPhone";     //  your network SSID (name)
char pass[] = "shihaohan";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char serverIP[] = "xxx.xxx.xxx.xxx";    //the IP to get request from
// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int ledCount = 3;

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

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to  WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for retry:
    delay(5000);
  }
  printWifiData();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  
void onButtonPressed(){
  ledCount += 1;
  ledCount %= 4;
}
void ledDisplay(){
//  Serial.println(ledCount);
  for(int i = 0; i < 3; i++){
    if( i == ledCount){
      digitalWrite(ledZero + i, HIGH);
    }else{
      digitalWrite(ledZero + i, LOW);
    }
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
    if (curButton != buttonState) {
      buttonState = curButton;
      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        onButtonPressed();
      }
    }
  }
  ledDisplay();
  lastButtonState = curButton;
}

