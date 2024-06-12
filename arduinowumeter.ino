/*
 * Arduino Wu Meter
 *
 * Created: 13.07.2024
 * Author: Giuseppe Tamanini
 */ 

#include <WS2812.h>

#define outputPin 8  // Digital output pin (default: 7)
#define LEDCount 90   // Number of LEDs to drive (default: 9)
WS2812 LED(LEDCount); 
cRGB value;
byte intensity;
byte sign;

int n;
int m;
int LedR = 0;
int LedG = 0;
int LedB = 0;
int barL;
int barR;

byte color[45] [3] = { {255,   0, 255},
                       {232,   3, 255},
                       {209,  46, 255},
                       {185,  70, 255},
                       {162,  93, 255},
                       {139, 116, 255},
                       {116, 139, 255},
                       { 93, 162, 255},
                       { 69, 186, 255},
                       { 46, 209, 255},
                       { 23, 232, 255},
                       {  0, 255, 255},
                       {  0, 255, 232},
                       {  0, 255, 209},
                       {  0, 255, 185},
                       {  0, 255, 162},
                       {  0, 255, 139},
                       {  0, 255, 116},
                       {  0, 255,  93},
                       {  0, 255,  69},
                       {  0, 255,  46},
                       {  0, 255,  23},
                       {  0, 255,   0},
                       { 23, 255,   0},
                       { 46, 255,   0},
                       { 69, 255,   0},
                       { 93, 255,   0},
                       {116, 255,   0},
                       {139, 255,   0},
                       {162, 255,   0},
                       {185, 255,   0},
                       {209, 255,   0},
                       {232, 255,   0},
                       {255, 255,   0},
                       {255, 232,   0},
                       {255, 209,   0},
                       {255, 185,   0},
                       {255, 162,   0},
                       {255, 139,   0},
                       {255, 116,   0},
                       {255,  93,   0},
                       {255,  69,   0},
                       {255,  46,   0},
                       {255,  23,   0},
                       {255,   0,   0} };

int sensorPinL = A0;   // select the input pin for the potentiometer
int sensorPinR = A1;   // select the input pin for the potentiometer
int audioLevelL = 0;  // variable to store the value coming from the sensor
int audioLevelR = 0;  // variable to store the value coming from the sensor

// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 9;     // the number of the pushbutton pin
const int buttonPin2 = 10;     // the number of the pushbutton pin
const int ledPin1 =  11;      // the number of the LED pin
const int ledPin2 =  12;      // the number of the LED pin
const int ledPin3 =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int buttonState1 = 0;         // variable for reading the pushbutton status
int functionState = 1; // variabile che memorizza lo stato della funzione
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
int volumeM = 50;
boolean up = false;

void setup() {
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

	LED.setOutput(outputPin);
  for (n = 0; n < 90; n++) {
    value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
    LED.set_crgb_at(n, value); // Set value at LED found at index 0
  }
  LED.sync(); // Sends the value to the LED
}

void loop() {
  buttonRead();
  volumeMaster();
  vuMeter();
  barL = audioLevelL/11;
  /*Serial.print("L");
  Serial.print(barL);
  Serial.print("R");
  Serial.println(barR); */
  barR = audioLevelR/11;
  switch (functionState) {
  case 1:
    for (m = 0; m < 45; m++) {
      if (m < barL) {
        LedR = color[m] [0] * volumeM / 100;
        LedG = color[m] [1] * volumeM / 100;
        LedB = color[m] [2] * volumeM / 100;
        value.r = LedR; value.g = LedG; value.b = LedB; // RGB Value -> Red
      } else {
       value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
      }
      LED.set_crgb_at(m, value); // Set value at LED found at index 0
    }
    for (m = 45; m < 90; m++) {
      if (m < barR + 45) {
        LedR = color[m - 45] [0] * volumeM / 100;
        LedG = color[m - 45] [1] * volumeM / 100;
        LedB = color[m - 45] [2] * volumeM / 100;
        value.r = LedR; value.g = LedG; value.b = LedB; // RGB Value -> Red
      } else {
       value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
      }
      LED.set_crgb_at(m, value); // Set value at LED found at index 0
    }
    break;
  case 2:
    for (m = 0; m < 45; m++) {
      if (m < barL) {
        LedR = color[m] [0] * volumeM / 100;
        LedG = color[m] [1] * volumeM / 100;
        LedB = color[m] [2] * volumeM / 100;
        value.r = LedR; value.g = LedG; value.b = LedB; // RGB Value -> Red
      } else {
       value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
      }
      LED.set_crgb_at(44 - m, value); // Set value at LED found at index 0
    }
    for (m = 45; m < 90; m++) {
      if (m < barR + 45) {
        LedR = color[m - 45] [0] * volumeM / 100;
        LedG = color[m - 45] [1] * volumeM / 100;
        LedB = color[m - 45] [2] * volumeM / 100;
        value.r = LedR; value.g = LedG; value.b = LedB; // RGB Value -> Red
      } else {
       value.b = 0; value.g = 0; value.r = 0; // RGB Value -> Red
      }
      LED.set_crgb_at(m, value); // Set value at LED found at index 0
    }
    break;
  }
  LED.sync(); // Sends the value to the LED
}

void vuMeter() {
  audioLevelL = analogRead(sensorPinL);   //Read audio input
  if(audioLevelL < 500) {                  //Audio is biased on 2.5 volts (half Vcc) by voltage divider.
    audioLevelL = map(audioLevelL,0,512,512,0);  //Convert downgoing signal to positive values.
  } else if(audioLevelL >= 524) {          //Convert upgoing signal to the same range as the downgoing.
    audioLevelL = map(audioLevelL,512,1023,0,512);
  } else {                                //for detection the highest value within the detection time.
    audioLevelL = 0;
  }
  audioLevelR = analogRead(sensorPinR);   //Read audio input
  if(audioLevelR < 500) {                  //Audio is biased on 2.5 volts (half Vcc) by voltage divider.
    audioLevelR = map(audioLevelR,0,512,512,0);  //Convert downgoing signal to positive values.
  } else if(audioLevelR >= 524) {          //Convert upgoing signal to the same range as the downgoing.
    audioLevelR = map(audioLevelR,512,1023,0,512);
  } else {                                //for detection the highest value within the detection time.
    audioLevelR = 0;
  }
}


void buttonRead() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin1); // legge e memorizza lo stato del pulsante
  // se il pulsante  premuto ed è passato più di 1 sec
  if (buttonState & millis() - previousMillis >= 250) {
    previousMillis = millis(); // salva il tempo in cui è cambiato lo stato del led
    functionState = functionState + 1;
    if (functionState == 3) functionState = 1;
    //Serial.println(functionState);
  }
  switch (functionState) {
  case 1:
    digitalWrite(ledPin1, true);
    digitalWrite(ledPin2, false);
    digitalWrite(ledPin3, false);
    break;
  case 2:
    digitalWrite(ledPin1, false);
    digitalWrite(ledPin2, true);
    digitalWrite(ledPin3, false);
    break;
  case 3:
    digitalWrite(ledPin1, true);
    digitalWrite(ledPin2, true);
    digitalWrite(ledPin3, false);
    break;
  case 4:
    digitalWrite(ledPin1, false);
    digitalWrite(ledPin2, false);
    digitalWrite(ledPin3, true);
    break;
  case 5:
    digitalWrite(ledPin1, true);
    digitalWrite(ledPin2, false);
    digitalWrite(ledPin3, true);
    break;
  case 6:
    digitalWrite(ledPin1, false);
    digitalWrite(ledPin2, true);
    digitalWrite(ledPin3, true);
    break;
  case 7:
    digitalWrite(ledPin1, true);
    digitalWrite(ledPin2, true);
    digitalWrite(ledPin3, true);
    break;
  }
}

void volumeMaster() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin2); // legge e memorizza lo stato del pulsante
  // se il pulsante  premuto ed è passato più di 1 sec
  if (buttonState1 & millis() - previousMillis1 >= 50) {
    previousMillis1 = millis(); // salva il tempo in cui è cambiato lo stato del led
    if (up) {
      volumeM = volumeM + 1;
      if (volumeM == 101) {
        volumeM = 99;
        up = false;
      }
    } else {
      volumeM = volumeM - 1;
      if (volumeM < 0) {
        volumeM = 0;
        up = true;
      }
    }
  }
}
