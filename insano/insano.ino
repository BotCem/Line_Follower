#include "line_follower.hpp"

void setup(){
  Serial.begin(9600);
  Serial.println("Start");

  init2();

  delay(5000);
}


int tempo_inicio =0, tempo_fim;

void loop(){
  int pos = 0;

  Serial.println(pos);
  pos = line_read();
  // tempo_fim=micros();
  
  
  // Serial.println(tempo_fim-tempo_inicio);
  // tempo_inicio=micros();

//  for(int i = 0; i < NUM_SENSORS; i++) {
//    pos += reads[i] << i;
//  }
  old_decision(pos);

  delay(1);
}

// ------------------------------------------------