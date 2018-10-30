#include "line_follower.hpp"
#include "serial_seguidor.hpp"

int estado_corrida=0;

void setup(){
    init_serial();
    init2();
    delay(5000);
  
}


int tempo_inicio =0, tempo_fim ,tempo=0;
void loop(){
  comunica_serial();
  while(estado_corrida){
   //tempo_inicio=micros();
    
    line_read();
    //Serial.println(pos);
    
   
  //  for(int i = 0; i < NUM_SENSORS; i++) {
  //    pos += reads[i] << i;
  //  }
    decision();
//    tempo=tempo_fim-tempo_inicio;
//   // delay(1);
//   tempo_fim=micros();
//     Serial.println(tempo);
   //tempo=tempo_fim-tempo_inicio;
   comunica_serial();
  }
}

// ------------------------------------------------
