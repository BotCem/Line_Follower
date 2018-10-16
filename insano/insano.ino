/*
      RoboCore - Motor Driver 2x2A example
                    (01/07/2015)

  Written by FranÃ§ois.
  
  Example of how to use the Motor Driver 2x2A.
  Open a Serial Monitor and enter the comands:
    - u : for PWM 0 to 255
    - d : for PWM 255 to 0
    - o : for ON
    - f : for OFF
    - 1 : for Direction 1
    - 2 : for Direction 2

*/

const int motorA = 5;
const int motorB = 6;
const int dirA1 = 7;
const int dirA2 = 8;
const int dirB1 = 9;
const int dirB2 = 10;
const int IR = 13;
byte b;

int leituraDigital(int);
int leituraAnalogica(int);

const unsigned char PS_16 = (1<<ADPS2);
const unsigned char PS_128 = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

// ------------------------------------------------

void setup(){
  //Serial.begin(9600);
  //Serial.println("Start");
  ADCSRA&=~PS_128;
  ADCSRA|=PS_16;
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);
  pinMode(IR,OUTPUT);

  digitalWrite(IR, HIGH);
  digitalWrite(dirA1, LOW);
  digitalWrite(dirA2, HIGH);
  digitalWrite(dirB1, LOW);
  digitalWrite(dirB2, HIGH);
  
  delay(5000);
}

// ------------------------------------------------

void set_motor(char ma , char mb){
  switch (ma){
    case 'f':
            digitalWrite(dirA1, LOW);
            digitalWrite(dirA2, HIGH);
            break;
    case 't':
            digitalWrite(dirA2, LOW);
            digitalWrite(dirA1, HIGH);
            break;

    default :
          digitalWrite(dirA1, LOW);
          digitalWrite(dirA2, LOW);
          while(1);
  }
    switch (mb){
    case 'f':
            digitalWrite(dirB1, LOW);
            digitalWrite(dirB2, HIGH);
            break;
    case 't':
            digitalWrite(dirB2, LOW);
            digitalWrite(dirB1, HIGH);
            break;
    default :
          digitalWrite(dirB1, LOW);
          digitalWrite(dirB2, LOW);
          while(1);
  }
}

// ------------------------------------------------
const int NUM_SENSORS = 8;
int analogReads[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int multipler[] = {-8, -4, -2, -1, 1, 2, 4, 8};
int reads[NUM_SENSORS];
int velbaixa = 0, velmedia = 50, velalta = 127;
int tempo_inicio =0, tempo_fim;

void loop(){
  int pos = 0;
  //ADCSRA&=~PS_128;
  //ADCSRA|=PS_16;
  
  for(int i  = 0 ; i < NUM_SENSORS; i++) {
     reads[i] = analogRead(analogReads[i]);
     if (reads[i] > 820) reads[i] = 0;
     else reads[i] = 1;
     pos += reads[i]* multipler[i];
  }
  //tempo_fim=micros();
  
  //for(int i  = 0 ; i < NUM_SENSORS; i++) {
  //   Serial.print(reads[i]);
  //   Serial.print(' ');
  //}
  //Serial.println(' ');
  
  //Serial.println(tempo_fim-tempo_inicio);
  //tempo_inicio=micros();

//  for(int i = 0; i < NUM_SENSORS; i++) {
//    pos += reads[i] << i;
//  }

  
//  if(pos < 6) { analogWrite(motorA,vel); analogWrite(motorB,0);}
//  else if(pos > 24) {analogWrite(motorA,0); analogWrite(motorB,vel);}
//  else {analogWrite(motorA,vel); analogWrite(motorB,vel);} 
  set_motor('f','f');
  if(pos <= -8) {set_motor('t','f'); analogWrite(motorA, velmedia); analogWrite(motorB, velalta);}
  else if (pos <= -4) {analogWrite(motorA, velbaixa); analogWrite(motorB, velalta);}
  else if (pos <= -2) {analogWrite(motorA, 30); analogWrite(motorB, velalta);}
  else if (pos >= 2) {analogWrite(motorA, velalta); analogWrite(motorB, 30);}
  else if (pos >= 4) {analogWrite(motorA, velalta); analogWrite(motorB, velbaixa);}
  else if (pos >= 8) {set_motor('f','t'); analogWrite(motorA, velalta); analogWrite(motorB, velmedia);}
  else {analogWrite(motorA, velalta); analogWrite(motorB, velalta);}
  delay(1);
}

// ------------------------------------------------
