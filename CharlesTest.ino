#include <ChuckPlex.h>
#include "MIDIUSB.h"

bool muteCh[] = {false, false, false, false, false};
int pins[] = {9, 10, 11};
//int nodes = 6;
ChuckPlex muteLEDs = ChuckPlex(pins, 3);
void setup(){
  // print the connections to make
  // you should remove this section once you've done your wiring
  Serial.begin(9600);
//  delay(3000);
//  muteLEDs.displayConnections(nodes);
}
void loop(){
  
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  
if (rx.header != 0) {
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
    for(int i=0;i<=4;i++){
        if (rx.byte2==i+1){
         if (rx.byte3==0){
          muteCh[i]=false;
          }else{
          muteCh[i]=true;
        } 
        } 
         Serial.println(muteCh[i]); 
      }
    }
  
  
    
  for(int i=1; i<=4; i++){ //need to do protothreading(multithreading)
      if(muteCh[i-1]){
        muteLEDs.enable(i);
    }
  }
  if(muteCh[4]){
    muteLEDs.enable(6);
  }
  muteLEDs.clear();
}
