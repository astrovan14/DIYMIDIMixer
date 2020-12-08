#include <ChuckPlex.h>
#include "MIDIUSB.h"

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

int prevOutputValue=0;
const int analogInPin = A0;
int sensorValue = 0;
int outputValue = 0;
bool muteCh[] = {false, false, false, false, false};
int pins[] = {9, 10, 11};
//int nodes = 6;
ChuckPlex muteLEDs = ChuckPlex(pins, 3);
void setup(){

  // print the connections to make
  // you should remove this section once you've done your wiring
//   Serial.begin(9600);
//  delay(3000);
//  muteLEDs.displayConnections(nodes);
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
//         Serial.println(muteCh[i]); 
      }
    }
  
  
    
  for(int i=1; i<=4; i++){
      if(muteCh[i-1]){
        muteLEDs.enable(i);
    delayMicroseconds(500);
    }
  }
  if(muteCh[4]){
    muteLEDs.enable(6);
    delayMicroseconds(500);
  }
  muteLEDs.clear();
   
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 175, 945, 0, 127);
  if(outputValue<0){outputValue=0;}
  if(outputValue>127){outputValue=127;}
//  Serial.print(outputValue);
//  Serial.print("  >=<  ");
//  Serial.println(prevOutputValue);
  if(prevOutputValue >= outputValue +3 || prevOutputValue <= outputValue -3 ){
//    Serial.println("changed");
    controlChange(5,50,outputValue);
    MidiUSB.flush();
    prevOutputValue=outputValue;
  }
  delay(2);
}
