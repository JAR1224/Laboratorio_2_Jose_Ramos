#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t counter = 0;

int main(void)
{

  DDRD = 0x03; // Configuracion del puerto
  //PORTD = 0x00;
  DDRB = 0x7F;
  //PORTB = 0x00;

  TCCR0A = 0x02; // Configuracion Timer0
  TCCR0B = 0x05;
  OCR0A = 0x4E;
  TIMSK = 0x01; 

  MCUCR = 0x03; // Configuracion External Interrupt INT0/1 y PCINT
  GIMSK = 0x40;


  sei();

  PORTB = 0x7F;
  PORTD = 0x01;

  while (1) {
    _delay_ms(10000);
  }
}

ISR( TIMER0_COMPA_vect ) //ISR es la macro que atiende interrupciones
{

  if (counter==0x00) {
    //PORTB ^= 0x80;
  }
  counter +=1;
  if (counter==100) {
    counter=0;
  }

}

ISR( INT0_vect )
{
  PORTD ^= 0x03;
  PORTB = (PIND & 0x78)>>3;
}

ISR( INT1_vect )
{
  //PORTB = 0xFF;
}

ISR( PCINT0_vect )
{
  //PORTD ^= 0x40;
}
