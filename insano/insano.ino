#include "line_follower.hpp"

void setup(){
  Serial.begin(115200);
  Serial.println("Start");

  init2();

  delay(5000);
}


int tempo_inicio =0, tempo_fim ,tempo=0;
int pos = 0;
void loop(){
 tempo_inicio=micros();

  pos = old_line_read();
  //Serial.println(pos);
  
   Serial.println(tempo);
//  for(int i = 0; i < NUM_SENSORS; i++) {
//    pos += reads[i] << i;
//  }
  old_decision(pos);
tempo=tempo_fim-tempo_inicio;
 // delay(1);
 tempo_fim=micros();
 tempo=tempo_fim-tempo_inicio;
}

// ------------------------------------------------
