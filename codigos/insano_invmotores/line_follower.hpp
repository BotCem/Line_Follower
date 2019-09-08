#ifndef LINE_FOLLOWER_HPP
#define LINE_FOLLOWER_HPP
#include <Arduino.h>
#include <EEPROM.h>

#define LINE_READ_TRAIT true

float kp=20 , ki=0 , kd=0 ;

const unsigned int DELTA_TEMPO = 1000; //tempo em microsegundos  
inline void decision();

//init2(): 					Usada para definir as configurações iniciais do programa. Colocar aqui 
//							o que não for pra ser alterado frequentemente.
//							Define todos os pinos de entrada/saida, seta o prescaler e configura 
//							os pinos 5 e 6 como saida analogica.
void init2();

//power_5(unsigned int):	Altera a potencia no pino 5. Esse pino não vai ser modificado,
//							pois teria que alterar os registradores. Motor A.
//							Entrada: 0 <= x <= 255.
inline void power_5(int x);
//power_6(unsigned int):	Altera a potencia no pino 6. Esse pino não vai ser modificado,
//							pois teria que alterar os registradores. Motor B.
//							Entrada: 0 <= x <= 255.
inline void power_6(int x);

//set_motor(char,char):		Altera as direções dos motores A e B. 
//							Entrada: 'f' para frente e 't' para tras.
void set_motor(char ma, char mb);

//unsigned int line_read():	Lê todos os sensores de forma analógica, faz a média ponderada e
//							retorna a posição relativa na linha. 
//							Saida: 0 < x < NUM_SENSORS*1000
//							Centro: (NUM_SENSORS - 1)/2.0 * 1000	p/ NUM_SENSOR par
//									((NUM_SENSORS)/2 - 1) * 1000	p/ NUM_SENSOR impar														
void line_read();

//unsigned int old_line_read(): Faz a leitura antiga dos sensores, retornando um valor pra pos
//								baseado na soma dos valores dos sensores. Deve ser utilizado
//								junto com a old_decision();
//								Saida: -14 <= x <= 14
inline unsigned int old_line_read();

//old_decision(int):		Faz a logica e escreve nos motores baseada na leitura da função
//							old_line_read(). Qualquer outro valor pode ter resultado inesperado;
//							Entrada: -14 <= x <= 14						
void old_decision(int p);



const int NUM_SENSORS = 8;
const int dirA1 = 7;
const int dirA2 = 8;
const int dirB1 = 9;
const int dirB2 = 10;
const int IR = 13;
const unsigned char PS_16 = (1<<ADPS2);
const unsigned char PS_128 = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

int analogReads[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int multipler[] = {-8, -4, -2, -1, 1, 2, 4, 8};
unsigned int reads[NUM_SENSORS];
unsigned int reads_min[NUM_SENSORS] = {1023,1023,1023,1023,1023,1023,1023,1023};
unsigned int reads_max[NUM_SENSORS] = {0,0,0,0,0,0,0,0};
unsigned int reads_thresh[NUM_SENSORS];
int velbaixa = 0, velmedia = 50, velalta = 160;
unsigned int numero_de_leituras = 1;
long pos;
long somat;
bool sensores_calibrados = true;


void init2(){
  DDRD |= (1<<DDD6) | (1 <<DDD5); // set pins PD6(pin 6) and PD5(pin 5) as output

  TCCR0A |= (1 << COM0A1) | (0 << COM0A0); // set none-inverted mode for pin OC0A(PD6)
  TCCR0A |= (1 << COM0B1) | (0 << COM0B0); // set none-inverted mode for pin OC0B(PD5)

  TCCR0A |= (1 << WGM01) | (1 << WGM00);
   // set fast PWM Mode
  TCCR0B |= (1 << CS01);
   // set prescaler to 8 and starts PWM

  ADCSRA&=~PS_128;
  ADCSRA|=PS_16;

  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);
  pinMode(IR,OUTPUT);

  for(int i = 0; i < NUM_SENSORS; i++) {
    pinMode(analogReads[i], INPUT);
    reads_thresh[i] = EEPROM.read(i)*4;
  }

  digitalWrite(IR, HIGH);
  digitalWrite(dirA1, LOW);
  digitalWrite(dirA2, HIGH);
  digitalWrite(dirB1, LOW);
  digitalWrite(dirB2, HIGH);
}

inline void power_5(int x){
  if(x < 0) x = 0;
  if(x > 255) x = 255;
	OCR0B = x;
}
inline void power_6(int x){
  if(x < 0) x = 0;
  if(x > 255) x = 255;
	OCR0A = x;
}

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

void line_read(){
	somat = 0;
	pos = 0;
	for(long i = 0 ; i < NUM_SENSORS; i++) {
    reads[i] = 0;    
    for(unsigned int j = 0; j < numero_de_leituras; j++){
      reads[i] += 1024 - analogRead(analogReads[i]);
    }
		reads[i] /= numero_de_leituras;
    if(sensores_calibrados) {
//      Serial.print(" ## "); Serial.print(reads[i]); 
      reads[i] = (reads[i] > reads_thresh[i]) ?  1 : 0; 
//      Serial.print(" x "); Serial.print(reads[i]); 
    }
		somat += reads[i];
    pos += (i*reads[i]*1000);
    /*Serial.print(i); Serial.print(" = ");*/// Serial.print (reads[i]); Serial.print(" +++ ");// Serial.println(pos);
//    #if LINE_READ_TRAIT
//      String msg("LINE_READ: sensor ");
//      msg += i;
//      msg += " valor ";
//      msg += reads[i];    
//      Serial.println(msg); 
//    #endif 
	}
//     Serial.println(" ");
// for(int i = 0; i < NUM_SENSORS; i++) {
//  pos += (reads[i]*1000.0*i)/somat;
// }
//  Serial.println(somat);
	pos /= somat;

//  #if LINE_READ_TRAIT
//    String msg("p ");  
//    msg += pos;
//    Serial.println(msg); 
//   #endif 
}

inline unsigned int old_line_read(){
  unsigned int p = 0;
	for(int i  = 0 ; i < NUM_SENSORS; i++) {
		reads[i] = analogRead(analogReads[i]);
		if (reads[i] > 820) reads[i] = 0;
		else reads[i] = 1;
		p += reads[i]* multipler[i];
	}
}

inline void old_decision(int p) {
	set_motor('f','f');
	if(p <= -8) {set_motor('t','f'); power_5(velmedia); power_6(velalta);}
	else if (p <= -4) {power_5(velbaixa); power_6(velalta);}
	else if (p <= -2) {power_5(45); power_6(velalta);}
	else if (p >= 2) {power_5(velalta); power_6(45);}
	else if (p >= 4) {power_5(velalta); power_6(velbaixa);}
	else if (p >= 8) {set_motor('f','t'); power_5(velalta); power_6(velmedia);}
	else {power_5(velalta); power_6(velalta);}
}
float erro = 0.0;
int pid = 0 , torque_base=220;
unsigned long int time_1 = 0;
inline void decision(){  
  erro=3500-pos;
  pid=erro*kp;
  power_5(torque_base+pid);//motor direito
  power_6(torque_base-pid);//motor esquerdo
}

inline void calibra_sensor(){
  for(int i = 0 ; i < NUM_SENSORS; i++){
    reads[i] = 1024 - analogRead(analogReads[i]); 
//    Serial.print(reads[i]); Serial.print(" ");
    if(reads[i] < reads_min[i]) reads_min[i] = reads[i];
    if(reads[i] > reads_max[i]) reads_max[i] = reads[i];
    reads_thresh[i] = reads_min[i] + (reads_max[i] - reads_min[i])/2;
    Serial.print(reads_thresh[i]); Serial.print(" ");
  }
  Serial.println(" ");
}

void salva_eeprom() {
  for(int i = 0; i < NUM_SENSORS; i++) {
    EEPROM.write(i, reads_thresh[i]/4); //esta divido por 4 para caber em 1 byte, que é a unidade minima da eeprom
  }
}

void limpa_eeprom() {
  for(int i = 0; i < NUM_SENSORS; i++) {
    EEPROM.write(i, 255); //esta divido por 4 para caber em 1 byte, que é a unidade minima da eeprom
  }
}



#endif
