#include <avr/io.h>
#include <avr/interrupt.h>

#define OCR1A_VALUE 52

uint8_t counter = 0;
uint8_t duty_cycle = 2;

int main(void)
{
    MCUCR = 0x03; // Configuracion External Interrupt INT0
    GIMSK = 0x40;

    DDRB |= (1 << PB0) | (1 << PB1); // Set PB0 as output
    //PORTB |= (1 << PB1);

    TCCR1B |= (1 << WGM12); // Set Timer1 to CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10); // Set prescaler to 1024
    OCR1A = OCR1A_VALUE; // Set OCR1A value for 1 second delay
    TIMSK |= (1 << OCIE1A); // Enable Timer1 compare match A interrupt

    sei(); // Enable global interrupts

    while(1)
    {
        // Main loop
    }
}

ISR(TIMER1_COMPA_vect)
{
    counter++;
    if (counter == duty_cycle) {
        PORTB ^= (1 << PB0); // Toggle PB0
    } else if (counter == 10) {
        PORTB ^= (1 << PB0); // Toggle PB0
        counter = 1;
    }
}

ISR( INT0_vect )
{
  PORTB ^= (1 << PB1);

}