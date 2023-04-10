#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t counter = 0;

int main(void)
{
  DDRB = 0x08; //Configuracion del puerto
  TCCR0A = 0x02;
  TCCR0B = 0x05;
  OCR0A = 0x4E;
  TIMSK = 0x01;

  MCUCR = 0x0B;
  GIMSK = 0xC0;

  sei();

  //Parpadear
  while (1) {
    //PORTB = 0x00;
    _delay_ms(10000);
    //PORTB ^= 0x08;
    //PORTB = 0x08;
    //_delay_ms(10000); 
  }
}

ISR( TIMER0_COMPA_vect ) //ISR es la macro que atiende interrupciones
{
// Instrucciones para atender interrupcion
  //if (counter==0x00) {
   // PORTB ^= 0x08;
  //}
  counter +=1;
  if (counter==100) {
    counter=0;
  }
  //PORTB ^= 0x08;
}

ISR( INT0_vect ) //ISR es la macro que atiende interrupciones
{
  PORTB ^= 0x08;
}

ISR( INT1_vect ) //ISR es la macro que atiende interrupciones
{
  PORTB ^= 0x08;
}
