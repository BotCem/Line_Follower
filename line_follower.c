#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBUG true
#define TIMEOUT 2500
#define NUM_SENSORS 1



void init2(){
  DDRD |= (1<<DDD6) | (1 <<DDD5); // set pins PD6(pin 6) and PD5(pin 5) as output

  TCCR0A |= (1 << COM0A1) | (0 << COM0A0); // set none-inverted mode for pin OC0A(PD6)
  TCCR0A |= (1 << COM0B1) | (0 << COM0B0); // set none-inverted mode for pin OC0B(PD5)

  TCCR0A |= (1 << WGM01) | (1 << WGM00);
   // set fast PWM Mode
  TCCR0B |= (1 << CS01);
   // set prescaler to 8 and starts PWM
}

void powerPin5(unsigned int x){
  OCR0B = x;
}
void powerPin6(unsigned int x){
  OCR0A = x;
}

void readLine(int sensor_list[NUM_SENSORS]){
  //int _sensors_mask[NUM_SENSORS] = { (1<<DDB0), (1<<DDB1), (1<<DDB2), (1<<DDB3), (1<<DDB4)};  //8,9,10,11,12
  //int _sensors = (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4);

  int _sensors = (1<<DDB0);
  int _sensors_mask[1] = {(1<<DDB0)};
  
  int last_read = _sensors;
  register int i;
  for(i = 0; i < NUM_SENSORS; i++){
    sensor_list[i] = 0;
  }
  DDRB |= _sensors;
  PORTB |= _sensors;

  _delay_us(10);

  DDRB &= ~_sensors;
  PORTB &= ~_sensors;

  register unsigned int last_time = TCNT2, _time = 0, delta_time;

  while(_time < TIMEOUT){
    delta_time = TCNT2 - last_time;
    _time += delta_time;
    last_time += delta_time;

    if(PINB == last_read) continue;

    last_read = PINB;

    for(i = 0; i < NUM_SENSORS; i++){
      if(sensor_list[i] == 0 && !(PINB & _sensors_mask[i])) sensor_list[i] = _time;
    }
  }

  for(i = 0 ; i < NUM_SENSORS; i++){
    if (!sensor_list[i]) sensor_list[i] = TIMEOUT;
  }

}

