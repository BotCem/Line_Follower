#include "line_follower.hpp"

void setup(){
  //pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial.println("Start");
  /*char ler;
  while(1){
  while(Serial.available()){
    delay(100);
    ler=Serial.read();
    delay(100);
    if(ler == '0') digitalWrite(LED_BUILTIN,HIGH);
    if(ler == '1') digitalWrite(LED_BUILTIN,LOW);
    delay(100);
  }
  
  }
  
  Serial.print("kp ");
  Serial.println(kp);
  Serial.print("ki ");
  Serial.println(ki);
  Serial.print("kd ");
  Serial.println(kd);
  Serial.print("motor base ");
  Serial.println(torque_base);

  while(1);*/
    init2();
    delay(5000);
  
}


int tempo_inicio =0, tempo_fim ,tempo=0;
void loop(){
 //tempo_inicio=micros();
  line_read();
  //Serial.println(pos);
  
 
//  for(int i = 0; i < NUM_SENSORS; i++) {
//    pos += reads[i] << i;
//  }
  decision();
  //tempo=tempo_fim-tempo_inicio;
 // delay(1);
 //tempo_fim=micros();
  // Serial.println(tempo);
 //tempo=tempo_fim-tempo_inicio;
}

// ------------------------------------------------
