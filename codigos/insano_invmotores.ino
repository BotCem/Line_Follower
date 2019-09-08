#include "line_follower.hpp"
#include "serial_seguidor.hpp"

#define CORRIDA_TRAIT true

bool estado_corrida = false;
bool estado_calibrar = false;
const bool TEMPO_FIXO = false;
void setup(){
    init_serial();
    init2();
  
}

const unsigned int DELTA_TEMPO = 1000; //tempo em microsegundos  
unsigned long tempo_inicio = 0, tempo_passado = 0;
void loop(){
  comunica_serial();
  while(estado_corrida){
   tempo_inicio=micros();    
    line_read();
    decision();
    comunica_serial();
    tempo_passado = micros() - tempo_inicio;
    #if CORRIDA_TRAIT
      String msg("t ");
      Serial.println(msg + tempo_passado);
    #endif 
    if(TEMPO_FIXO && tempo_passado < DELTA_TEMPO) delayMicroseconds(DELTA_TEMPO - tempo_passado);
  }
  while(estado_calibrar) {
    calibra_sensor();
    comunica_serial();
  }
}

// ------------------------------------------------
