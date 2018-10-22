#include <Arduino.h>

//init2(): 					Usada para definir as configurações iniciais do programa. Colocar aqui 
//							o que não for pra ser alterado frequentemente.
//							Define todos os pinos de entrada/saida, seta o prescaler e configura 
//							os pinos 5 e 6 como saida analogica.
void init2();

//power_5(unsigned int):	Altera a potencia no pino 5. Esse pino não vai ser modificado,
//							pois teria que alterar os registradores. Motor A.
//							Entrada: 0 <= x <= 255.
inline void power_5(unsigned int x);
//power_6(unsigned int):	Altera a potencia no pino 6. Esse pino não vai ser modificado,
//							pois teria que alterar os registradores. Motor B.
//							Entrada: 0 <= x <= 255.
inline void power_6(unsigned int x);

//set_motor(char,char):		Altera as direções dos motores A e B. 
//							Entrada: 'f' para frente e 't' para tras.
void set_motor(char ma, char mb);

//unsigned int line_read():	Lê todos os sensores de forma analógica, faz a média ponderada e
//							retorna a posição relativa na linha. 
//							Saida: 0 < x < NUM_SENSORS*1000
//							Centro: (NUM_SENSORS - 1)/2.0 * 1000	p/ NUM_SENSOR par
//									((NUM_SENSORS)/2 - 1) * 1000	p/ NUM_SENSOR impar														
unsigned int line_read();

//unsigned int old_line_read(): Faz a leitura antiga dos sensores, retornando um valor pra pos
//								baseado na soma dos valores dos sensores. Deve ser utilizado
//								junto com a old_decision();
//								Saida: -14 <= x <= 14
unsigned int old_line_read();

//old_decision(int):		Faz a logica e escreve nos motores baseada na leitura da função
//							old_line_read(). Qualquer outro valor pode ter resultado inesperado;
//							Entrada: -14 <= x <= 14						
void old_decision(int pos);

const int NUM_SENSORS = 8;
int analogReads[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int multipler[] = {-8, -4, -2, -1, 1, 2, 4, 8};
unsigned int reads[NUM_SENSORS];
const int dirA1 = 7;
const int dirA2 = 8;
const int dirB1 = 9;
const int dirB2 = 10;
const int IR = 13;
const unsigned char PS_16 = (1<<ADPS2);
const unsigned char PS_128 = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

int velbaixa = 0, velmedia = 50, velalta = 160;

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

  digitalWrite(IR, HIGH);
  digitalWrite(dirA1, LOW);
  digitalWrite(dirA2, HIGH);
  digitalWrite(dirB1, LOW);
  digitalWrite(dirB2, HIGH);
}

inline void power_5(unsigned int x){
	OCR0B = x;
}
inline void power_6(unsigned int x){
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

unsigned int line_read(){
	unsigned int somat = 0, pos = 0;
	for(int i = 0 ; i < NUM_SENSORS; i++) {
		reads[i] = 1024 - analogRead(analogReads[i]);
		somat += reads[i];
	}
	for(int i = 0 ; i < NUM_SENSORS; i++){
		pos += (reads[i]*1000.0*i)/somat;
	}
 return pos;
}

inline unsigned int old_line_read(){
	unsigned int pos = 0;
	for(int i  = 0 ; i < NUM_SENSORS; i++) {
		reads[i] = analogRead(analogReads[i]);
		if (reads[i] > 820) reads[i] = 0;
		else reads[i] = 1;
		pos += reads[i]* multipler[i];
	}
	return pos;
}

inline void old_decision(int pos) {
	set_motor('f','f');
	if(pos <= -8) {set_motor('t','f'); power_5(velmedia); power_6(velalta);}
	else if (pos <= -4) {power_5(velbaixa); power_6(velalta);}
	else if (pos <= -2) {power_5(45); power_6(velalta);}
	else if (pos >= 2) {power_5(velalta); power_6(45);}
	else if (pos >= 4) {power_5(velalta); power_6(velbaixa);}
	else if (pos >= 8) {set_motor('f','t'); power_5(velalta); power_6(velmedia);}
	else {power_5(velalta); power_6(velalta);}
}
