String command = ""; // Stores response of bluetooth device  
            // which simply allows n between each  
            // response.  

const char MOTORES = 'M';
const char PID = 'P';
const char BASE = 'B';
const char RUN = 'R';
const char STOP = 'S';
    
void setup()   
{  
  //Inicia a serial  
  Serial.begin(115200);
  Serial.setTimeout(100);  
  Serial.println("Inicializando o programa");  
//  //Inicia a serial configurada nas portas 10 e 11
//  mySerial.begin(115200); 
//  pinMode(13,OUTPUT); 
}  
    
void loop()  
{  
  // Read device output if available.  
  if (Serial.available()){
    command = Serial.readStringUntil(',');
   Serial.println(command);
   msg_handler();
   command = ""; // No repeats  
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
//     case RUN:
//      run_handler();
//      break;
//     case STOP:
//      stop_handler();
//      break;
     default:
      Serial.println("ERROR");
   }
   
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
  String kp = "", ki = "", kd = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) kp += command[i];
  int pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) ki += command[i];
  pos = pos2;
  pos2 = command.indexOf(';',pos+1);
  for(int i = pos+1; i < pos2; i++) kd += command[i];
  long Kp = kp.toInt();
  long Ki = ki.toInt();
  long Kd = kd.toInt();
  Serial.print("KP="); Serial.print(Kp);
  Serial.print(" KI="); Serial.print(Ki);
  Serial.print(" KD="); Serial.println(Kd);
  }
void base_handler() {
  Serial.println("Base handler");
  String base = "";
  int pos = command.indexOf(';');
  for(int i = 1; i < pos; i++) base += command[i];
  long bs = base.toInt();
  Serial.print("BASE="); Serial.println(bs);
  }

//void run_handler() {
//  Serial.println("Run handler");
//
//
//
//
//}


