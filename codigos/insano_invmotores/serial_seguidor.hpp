#ifndef SERIAL_SEGUIDOR_HPP
#define SERIAL_SEGUIDOR_HPP

#include <Arduino.h>
#include "line_follower.hpp"
String command = ""; // Stores response of bluetooth device  
            // which simply allows n between each  
            // response.  
int invmotores=0;

const char MOTORES = 'M';
const char PID = 'P';
const char BASE = 'B';
const char RUN = 'R';
const char STOP = 'S';
const char CALIBRAR = 'C';

const char INVMOTORES = 'I';
const char INVA = 'D';
const char INVB = 'E';

extern float kp ,ki , kd;
extern bool estado_corrida;
extern bool estado_calibrar;
extern int torque_base;
extern const int dirA1;
extern const int dirA2;
extern const int dirB1;
extern const int dirB2;

void comunica_serial();

void base_handler(){
  Serial.println("Base handler");
  String motorE = "", motorD = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) motorE += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) motorD += command[i];
  long me = motorE.toInt();
  long md = motorD.toInt();
  torque_base = me;
  Serial.print("BASE_E="); Serial.print(me);
  Serial.print("BASE_D="); Serial.println(md);
  }

void run_handler(){
  Serial.println("Run handler");
  estado_corrida=1;
}

void  stop_handler(){
  Serial.println("Stop handler");
  estado_corrida=0;
  power_5(0);
  power_6(0);
}

void motores_handler() {
  Serial.println("Motores handler");
  int temp;  
  String motorE = "", motorD = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) motorE += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) motorD += command[i];
  long me = motorE.toInt();
  long md = motorD.toInt();
  if( invmotores == 1){
    temp=me;
    me=md;
    md=temp;
  }
  Serial.print("ME="); Serial.print(me);
  Serial.print(" MD="); Serial.println(md);
  estado_corrida = 1;
  power_5(me);
  power_6(md);
  Serial.println("######## Correndo ########");
  while(estado_corrida) {
    comunica_serial();
  }
  Serial.println("######## Parando ########");
 }
  
void pid_handler() {
  Serial.println("PID handler");
  String Kp = "", Ki = "", Kd = "", NL = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) Kp += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) Ki += command[i];
  pos = pos2;
  pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) Kd += command[i];
  pos = pos2;
  pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) NL += command[i];
  
  kp = Kp.toInt()/1000.0;
  ki = Ki.toInt()/1000.0;
  kd = Kd.toInt()/1000.0;
  numero_de_leituras = NL.toInt();
  Serial.print("KP="); Serial.print(kp);
  Serial.print(" KI="); Serial.print(ki);
  Serial.print(" KD="); Serial.print(kd);
  Serial.print(" NL="); Serial.println(numero_de_leituras);
  }

void calibrar_handler() {
  Serial.println("Calibrar handler");
  String comando = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) comando += command[i];
  long com = comando.toInt();

  switch(com) {
    case 0:
      sensores_calibrados = false;
      limpa_eeprom();
      Serial.println("Sensores descalibrados");
      break;
    case 1:
      estado_calibrar = true;
      break;
    case 2:
      if(estado_calibrar)
        sensores_calibrados = true;
      estado_calibrar = false;
      break;
    case 3:
      Serial.print("Valores minimos:");
      for(int i = 0 ; i < NUM_SENSORS; i++){
        Serial.print(reads_min[i]); Serial.print(" ");
      }
      Serial.print("\nValores maximos:");
      for(int i = 0 ; i < NUM_SENSORS; i++){
        Serial.print(reads_max[i]); Serial.print(" ");
      }
      Serial.print("\nValores threshold:");
      for(int i = 0 ; i < NUM_SENSORS; i++){
        Serial.print(reads_thresh[i]); Serial.print(" ");
      }
      Serial.println(" ");
  }    

}
void inv_motores(){
  Serial.println("Inv_motores handler");
  if (invmotores == 1) invmotores = 0;
  else invmotores = 1;
  
}
void inv_motorA(){
   Serial.println("Inv_motorA handler");
   if (digitalRead(dirA1)==HIGH ||digitalRead(dirA2)==LOW) {
   digitalWrite(dirA1,LOW);
   digitalWrite(dirA2,HIGH);
  }else {
    digitalWrite(dirA1,HIGH);
    digitalWrite(dirA2,LOW);
  }
}
void inv_motorB(){
  Serial.println("Inv_motorB handler");
  if (digitalRead(dirB1)==HIGH ||digitalRead(dirB2)==LOW) {
   digitalWrite(dirB1,LOW);
   digitalWrite(dirB2,HIGH);
  }else {
    digitalWrite(dirB1,HIGH);
    digitalWrite(dirB2,LOW);
  }
}

  void msg_handler() {
  switch(command[0]){
    case MOTORES:
      motores_handler();
      break;
     case PID:
      pid_handler();
      break;
     case BASE:
      base_handler();
      break;
     case RUN:
      run_handler();
      break;
     case STOP:
      stop_handler();
      break;
     case INVMOTORES:
      inv_motores();
      break;
     case INVA:
      inv_motorA();
      break;
     case INVB:
      inv_motorB();
      break;
     case CALIBRAR:
      calibrar_handler();
      break;
     default:
      Serial.println("ERRO NO HANDLER");
   }
   
}

void init_serial()   
{  
  //Inicia a serial  
  Serial.begin(115200);
  Serial.setTimeout(100);  
  Serial.println("Inicializando o programa");
}  
    
void comunica_serial()  
{  
  // Read device output if available.  
  if (Serial.available()){
    command = Serial.readStringUntil(',');
   Serial.println(command);
   msg_handler();
   command = ""; // No repeats  
  }  
   
}

#endif


  
