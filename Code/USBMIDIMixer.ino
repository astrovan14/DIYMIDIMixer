#include <ChuckPlex.h>
#include "MIDIUSB.h"

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

//averaging setup for analog buttons
//const int buttonNumReadings = 10;
//
//int buttonReadings[buttonNumReadings][2];      // the readings from the analog input
//int buttonReadIndex[] = {0,0};              // the index of the current reading
//int buttonTotal[] = {0,0};                  // the running total
int buttonAverage[] = {0,0};                // the average
//int buttonSensorValue[] = {0,0};

int buttonInputPin[] = {A8, A9};

int digitalButtonInput[] = {0,0,0,0,0}; // this is a storage method for the digital buttons to after conversion 

//averaging setup for faders
const int numReadings = 30;

int readings[numReadings][5];      // the readings from the analog input
int readIndex[] = {0,0,0,0,0};              // the index of the current reading
int total[] = {0,0,0,0,0};                  // the running total
int average[] = {0,0,0,0,0};                // the average

const int buttonPin = 7;
int buttonState[5];
int lastButtonState[] = {0,0,0,0,0};
int prevOutputValue[] = {0,0,0,0,0};
unsigned long lastDebounceTime[] = {0,0,0,0,0};
unsigned long debounceDelay = 30;

const int analogInPin[] = {A10,A0,A1,A2,A3};
int sensorValue[] = {0,0,0,0,0};
int outputValue[] = {0,0,0,0,0};
bool muteCh[] = {true, true, true, true, true};
int pins[] = {4, 5, 6};
//int nodes = 6;
ChuckPlex muteLEDs = ChuckPlex(pins, 3);
void setup(){
 pinMode(buttonPin, INPUT);
//   print the connections to make
//   you should remove this section once you've done your wiring
//   Serial.begin(9600);
//  delay(6000);
//  muteLEDs.displayConnections(6);
 for(int i=0;i<=4;i++){
   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading][i] = 0;
    }
  }
//   for(int i=0;i<=1;i++){
//   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
//      buttonReadings[thisReading][i] = 0;
//    }
//  }
}


void loop(){
  
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  
if (rx.header != 0) {
//      Serial.print("Received: ");
//      Serial.print(rx.header, HEX);
//      Serial.print("-");
//      Serial.print(rx.byte1, HEX);
//      Serial.print("-");
//      Serial.print(rx.byte2, HEX);
//      Serial.print("-");
//      Serial.println(rx.byte3, HEX);
    for(int i=0;i<=4;i++){
        if (rx.byte2==i+1){
         if (rx.byte3==0){
          muteCh[i]=false;
          }else{
          muteCh[i]=true;
        } 
        } 
//        Serial.print(i);
//        Serial.print(": ");
//        Serial.print(muteCh[i]);
//        Serial.print(" "); 
      }
//      Serial.println();
    }
  
  
    
  for(int i=1; i<=4; i++){
      if(muteCh[i-1]){
        muteLEDs.enable(i);
//        Serial.print("printing: ");
//        Serial.println(i);
    delayMicroseconds(500);
    }
  }
  if(muteCh[4]){
    muteLEDs.enable(6);
//    Serial.println("printing: 5");
    delayMicroseconds(500);
  }
  muteLEDs.clear();

  for(int i=0;i<=4;i++){ 
      sensorValue[i] = analogRead(analogInPin[i]);
//      Serial.print(i);
//      Serial.print(": ");
//      Serial.print(sensorValue[i]);
//Serial.print(" ");
      total[i] = total[i] - readings[readIndex[i]][i];
      
      readings[readIndex[i]][i] = sensorValue[i];
      
      total[i] = total[i] + readings[readIndex[i]][i];
      
      readIndex[i] = readIndex[i] + 1;
    
      if (readIndex[i] >= numReadings) {
        
        readIndex[i] = 0;
      }
      // calculate the average:
      average[i] = total[i] / numReadings;
      
      outputValue[i] = map(average[i], 1023, 5, 0, 127);
      if(outputValue[i]<0){outputValue[i]=0;}
      if(outputValue[i]>127){outputValue[i]=127;}
      
    //  Serial.print(outputValue);
    //  Serial.print("  >=<  ");
    //  Serial.println(prevOutputValue);
      if(prevOutputValue[i] > outputValue[i] || prevOutputValue[i] < outputValue[i]  ){
    //    Serial.println("changed");
        controlChange(0,i,outputValue[i]);
        MidiUSB.flush();
        prevOutputValue[i]=outputValue[i];
    //        Serial.println(map(average,0,127,0,100));
      }
  }
//      Serial.println();
for(int i=0;i<=1;i++){
   buttonAverage[i]=analogRead(buttonInputPin[i]);//buttonSensorValue[i]
//   buttonSensorValue[i] = analogRead(buttonInputPin[i]);
//      buttonTotal[i] = buttonTotal[i] - buttonReadings[buttonReadIndex[i]][i];
//      
//      buttonReadings[buttonReadIndex[i]][i] = buttonSensorValue[i];
//      
//      buttonTotal[i] = buttonTotal[i] + buttonReadings[buttonReadIndex[i]][i];
//      
//      buttonReadIndex[i] = buttonReadIndex[i] + 1;
//    
//      if (buttonReadIndex[i] >= buttonNumReadings) {
//        
//        buttonReadIndex[i] = 0;
//      }
//      // calculate the average:
//      buttonAverage[i] = buttonTotal[i] / buttonNumReadings;
//      Serial.print(i);
//      Serial.print("a: ");
//      Serial.print(buttonAverage[i]);
//      Serial.print(" ");
}
digitalButtonInput[0]= digitalRead(buttonPin);
//Serial.println();
 // this is for the analog to digital for the A8 pin  
  if(buttonAverage[0]>=1002 && buttonAverage[0]<=1005){//both
    digitalButtonInput[1]=1;
    digitalButtonInput[2]=1;
  } else if(buttonAverage[0]>=994 && buttonAverage[0]<=996){//right
    digitalButtonInput[1]=0;
    digitalButtonInput[2]=1;
  } else if(buttonAverage[0]>=936 && buttonAverage[0]<=938){//left
    digitalButtonInput[1]=1;
    digitalButtonInput[2]=0;
  } else{
    digitalButtonInput[1]=0;
    digitalButtonInput[2]=0;
  }
//Serial.println(buttonAverage[0]);
// this is for the analog to digital for the A9 pin  
  if(buttonAverage[1]>=1003 && buttonAverage[1]<=1005){//both
    digitalButtonInput[3]=1;
    digitalButtonInput[4]=1;
  } else if(buttonAverage[1]>=995 && buttonAverage[1]<=997){//right
    digitalButtonInput[3]=0;
    digitalButtonInput[4]=1;
  } else if(buttonAverage[1]>=937 && buttonAverage[1]<=939){//left
    digitalButtonInput[3]=1;
    digitalButtonInput[4]=0;
  } else{
    digitalButtonInput[3]=0;
    digitalButtonInput[4]=0;
  }

//Serial.print(digitalButtonInput[0]);
//Serial.print(digitalButtonInput[1]);
//Serial.print(digitalButtonInput[2]);
//Serial.print(digitalButtonInput[3]);
//Serial.println(digitalButtonInput[4]);
for(int i=0;i<=4;i++){ 
  int reading = digitalButtonInput[i];
  if (reading != lastButtonState[i]) {
    lastDebounceTime[i] = millis();
  }

  if ((millis() - lastDebounceTime[i]) > debounceDelay) {
    if (reading != buttonState[i]) {
      buttonState[i] = reading;
      if (buttonState[i] == HIGH) {
        noteOn(0,i,127);
        MidiUSB.flush();
      }
      if (buttonState[i] == LOW) {
        noteOff(0,i,127);
        MidiUSB.flush();
      }
    }
  }
  lastButtonState[i] = reading;
}
  delay(2);
}
