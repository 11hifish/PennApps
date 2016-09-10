/*
MODIFIED AND ADAPTED FROM ARDUINO EXAMPLE.
circuit:
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
 by Sihao Yu for PennApps XIV

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledEmpty = 2;

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int ledCount = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("RGB led debo");
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledEmpty + 1, OUTPUT);
  pinMode(ledEmpty + 2, OUTPUT);
  pinMode(ledEmpty + 3, OUTPUT);

  // set initial LED state
  digitalWrite(ledEmpty + 1, ledState);
  digitalWrite(ledEmpty + 2, ledState);
  digitalWrite(ledEmpty + 3, ledState);
}
void onButtonPress(){
  ledCount += 1;
  ledCount %= 4;
}
void ledDisplay(){
//  Serial.println(ledCount);
  for(int i = 1; i < 4; i++){
    if( i == ledCount){
      digitalWrite(ledEmpty + i, HIGH);
    }else{
      digitalWrite(ledEmpty + i, LOW);
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
        onButtonPress();
      }
    }
  }
  ledDisplay();
  lastButtonState = curButton;
}

