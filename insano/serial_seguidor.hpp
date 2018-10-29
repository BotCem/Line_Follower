#include <Arduino.h>
#include "line_follower.hpp"

String command = ""; // Stores response of bluetooth device  
            // which simply allows n between each  
            // response.  

const char MOTORES = 'M';
const char PID = 'P';
const char BASE = 'B';
const char RUN = 'R';
const char STOP = 'S';

extern float kp ,ki , kd;
extern int estado_corrida;
extern int torque_base;

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
  Serial.print("BASE="); Serial.println(torque_base);
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
  String motorE = "", motorD = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) motorE += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) motorD += command[i];
  long me = motorE.toInt();
  long md = motorD.toInt();
  Serial.print("ME="); Serial.print(me);
  Serial.print(" MD="); Serial.println(md);
  }
  
void pid_handler() {
  Serial.println("PID handler");
  String Kp = "", Ki = "", Kd = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) Kp += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) Ki += command[i];
  pos = pos2;
  pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) Kd += command[i];
  kp = Kp.toInt()/1000.0;
  ki = Ki.toInt()/1000.0;
  kd = Kd.toInt()/1000.0;
  Serial.print("KP="); Serial.print(kp);
  Serial.print(" KI="); Serial.print(ki);
  Serial.print(" KD="); Serial.println(kd);
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
     default:
      Serial.println("ERROR");
   }
   
}

void init_serial()   
{  
  //Inicia a serial  
  Serial.begin(115200);
  Serial.setTimeout(100);  
  Serial.println("Inicializando o programa");  
//  //Inicia a serial configurada nas portas 10 e 11
//  mySerial.begin(115200); 

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


  
