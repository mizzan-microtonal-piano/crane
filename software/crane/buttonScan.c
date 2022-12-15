#include "buttonScan.h"
#include <Arduino.h>



volatile int buttonPressed[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
volatile int buttonPressedPrevious[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
volatile int buttonEdge[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
volatile int buttonHandle[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
volatile int buttonFlag[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
volatile int buttonFlagTimeStamp[12] = {0,0,0,0,0,0,0,0,0,0,0,0};



void buttonClearFlag(){
  int i;
  for(i=0;i<12;i++){
    if(buttonHandle[i] == 1){
      buttonFlag[i] = 0;
    }
  }
}



void buttonScanInit(){
  pinMode(CRANE_KEY_C0, OUTPUT); 
  pinMode(CRANE_KEY_C1, OUTPUT);    
  pinMode(CRANE_KEY_C2, OUTPUT);   
  pinMode(CRANE_KEY_R0, INPUT);    
  pinMode(CRANE_KEY_R1, INPUT);    
  pinMode(CRANE_KEY_R2, INPUT);    
  pinMode(CRANE_KEY_R3, INPUT);    

  digitalWrite(CRANE_KEY_R0,HIGH );
  digitalWrite(CRANE_KEY_R1,HIGH );
  digitalWrite(CRANE_KEY_R2,HIGH );
  digitalWrite(CRANE_KEY_R3,HIGH );
}




void buttonEdgeScan(){
  int i;
  for(i=0;i<12;i++){
    if( buttonFlag[i] == 0 &&  buttonPressedPrevious[i] == 0  && buttonPressed[i] == 1){ //rising edge
      buttonFlag[i] = 1;
      buttonFlagTimeStamp[i] = millis();
      buttonEdge[i] = 1;
    }
    else if( buttonFlag[i] == 0 &&  buttonPressedPrevious[i] == 1  && buttonPressed[i] == 0){ //falling edge
      buttonFlag[i] = 1;
      buttonFlagTimeStamp[i] = millis();
      buttonEdge[i] = 0;
    }
  }
}



void buttonScan(){
  int i,j;
  int buttonID = 0;

  for(i=0;i<3;i++){
    selectCol(i);
    for(j=0;j<4;j++){
      buttonID = i * 4 + j;
      int val;
      int val1 = readRow(j);
      delay(5); //debouncing delay
      int val2 = readRow(j);
      if (val1 == val2){
        val = val2;
        buttonPressedPrevious[buttonID] = buttonPressed[buttonID];  
        buttonPressed[buttonID] = val;  
      }
    }
  }
}

int readRow(int row){
  switch(row){
    case 0: {
      return digitalRead(CRANE_KEY_R0);
      break;}
    case 1: {
      return digitalRead(CRANE_KEY_R1);
      break;}
    case 2: {
      return digitalRead(CRANE_KEY_R2);
      break;}
    case 3: {
      return digitalRead(CRANE_KEY_R3);
      break;}
  }

  int r0 = digitalRead(CRANE_KEY_R0);
  int r1 = digitalRead(CRANE_KEY_R0);
  int r2 = digitalRead(CRANE_KEY_R0);
  int r3 = digitalRead(CRANE_KEY_R0);

  return r3 << 3 | r2 << 2 | r1 << 1 | r0 <<0 ;
}




void selectCol(int col){
  switch(col){
    case 0: {
      digitalWrite(CRANE_KEY_C0, HIGH);
      digitalWrite(CRANE_KEY_C1, LOW);
      digitalWrite(CRANE_KEY_C2, LOW);
      break;}
    case 1: {
      digitalWrite(CRANE_KEY_C0, LOW);
      digitalWrite(CRANE_KEY_C1, HIGH);
      digitalWrite(CRANE_KEY_C2, LOW);
      break;}
    case 2: {
      digitalWrite(CRANE_KEY_C0, LOW);
      digitalWrite(CRANE_KEY_C1, LOW);
      digitalWrite(CRANE_KEY_C2, HIGH);
      break;}
  }
}
