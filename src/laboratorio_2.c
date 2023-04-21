#include <avr/io.h>
#include <avr/interrupt.h>

#define OCR1A_VALUE 52
#define ESPERAR_USUARIO 0
#define LAVAR 1
#define Hz60_FLAG 0
#define START_STOP_FLAG 1
#define TIMER_FLAG 2
#define duty_cycle 9
#define CARGA_1 0
#define CARGA_2 1
#define CARGA_3 2
#define LAVADO_1 3
#define LAVADO_2 4
#define LAVADO_3 5
#define LAVADO_4 6
#define START_STOP 7



uint8_t counter_60Hz = 0;
uint8_t counter_1Hz = 0;
uint8_t flag = 0;
uint8_t LED1 = 0;
uint8_t LED2 = 0;
uint8_t nivel_de_carga = 0;
uint8_t estado_de_lavado = 0;
uint8_t estado = 0;
uint8_t botones_LED = 0;




void send_byte(uint8_t cod) {

	for (unsigned int c = 0 ; c < 8 ; c++) {
		PORTA = (PORTA & 0xFB) | ((cod&0x01) << 2);
		PORTA ^= (1 << PA1);
		PORTA ^= (1 << PA1);
		cod>>=1;
	}
	PORTA ^= (1 << PA0);
	PORTA ^= (1 << PA0);
}

void add_time() {
	if (nivel_de_carga == 1) {
		if (estado_de_lavado == 1) {
			LED2=1;
			botones_LED = (botones_LED & 0x87) | (0x08);
		} else if (estado_de_lavado == 2) {
			LED2=3;
			botones_LED = (botones_LED & 0x87) | (0x10);
		} else if (estado_de_lavado == 3) {
			LED2=2;
			botones_LED = (botones_LED & 0x87) | (0x20);
		} else if (estado_de_lavado == 4) {
			LED2=3;	
			botones_LED = (botones_LED & 0x87) | (0x40);
		} else {
			botones_LED = (botones_LED & 0x87) | (0x00);
		}
	} else if (nivel_de_carga == 2) {
		if (estado_de_lavado == 1) {
			LED2=2;
			botones_LED = (botones_LED & 0x87) | (0x08);
		} else if (estado_de_lavado == 2) {
			LED2=7;
			botones_LED = (botones_LED & 0x87) | (0x10);
		} else if (estado_de_lavado == 3) {
			LED2=4;
			botones_LED = (botones_LED & 0x87) | (0x20);
		} else if (estado_de_lavado == 4) {
			LED2=6;
			botones_LED = (botones_LED & 0x87) | (0x40);
		} else {
			botones_LED = (botones_LED & 0x87) | (0x00);
		}
	} else if (nivel_de_carga == 3) {
		if (estado_de_lavado == 1) {
			LED2=3;
			botones_LED = (botones_LED & 0x87) | (0x08);
		} else if (estado_de_lavado == 2) {
			LED1=1;
			botones_LED = (botones_LED & 0x87) | (0x10);
		} else if (estado_de_lavado == 3) {
			LED2=5;
			botones_LED = (botones_LED & 0x87) | (0x20);
		} else if (estado_de_lavado == 4) {
			LED2=9;
			botones_LED = (botones_LED & 0x87) | (0x40);
		} else {
			botones_LED = (botones_LED & 0x87) | (0x00);
		}
	}
}

void dec_value() {
	if ( ((flag&(1<<START_STOP_FLAG))>0) ) {
		if (LED2 == 0) {
			if (LED1 == 0) {
				flag |= (1<<TIMER_FLAG);
				return;
			} else {
				LED1--;
				LED2=9;
			}
		} else {
			LED2--;
		}
	}
}

void display_LED() {

	if ((flag&(1<<Hz60_FLAG)) == 0x00) {
		PORTD &= ~(1 << PD0);
		if (LED1 == 0x00) {
			PORTB = (0b01111110);
		} else if (LED1 == 0x01) {
			PORTB = (0b00110000);
		} else if (LED1 == 0x02) {
			PORTB = (0b01101101);
		} else if (LED1 == 0x03) {
			PORTB = (0b01111001);
		} else if (LED1 == 0x04) {
			PORTB = (0b00110011);
		} else if (LED1 == 0x05) {
			PORTB = (0b01011011);
		} else if (LED1 == 0x06) {
			PORTB = (0b01011111);
		} else if (LED1 == 0x07) {
			PORTB = (0b01110000);
		} else if (LED1 == 0x08) {
			PORTB = (0b01111111);
		} else if (LED1 == 0x09) {
			PORTB = (0b01110011);
		}
	} else if ((flag&(1<<Hz60_FLAG)) > 0) {
		PORTD &= ~(1 << PD1);
		if (LED2 == 0x00) {
			PORTB = (0b01111110);
		} else if (LED2 == 0x01) {
			PORTB = (0b00110000);
		} else if (LED2 == 0x02) {
			PORTB = (0b01101101);
		} else if (LED2 == 0x03) {
			PORTB = (0b01111001);
		} else if (LED2 == 0x04) {
			PORTB = (0b00110011);
		} else if (LED2 == 0x05) {
			PORTB = (0b01011011);
		} else if (LED2 == 0x06) {
			PORTB = (0b01011111);
		} else if (LED2 == 0x07) {
			PORTB = (0b01110000);
		} else if (LED2 == 0x08) {
			PORTB = (0b01111111);
		} else if (LED2 == 0x09) {
			PORTB = (0b01110011);
		}
	}

}

int main(void)
{
    MCUCR = 0x03; 				// External Interrupt INT0
    GIMSK = 0x40;

    DDRA |= (1 << PA2) | (1 << PA1) | (1 << PA0);
    DDRB |= 0xFF; 				// Set PB0...PB6 as output
    DDRD |= (1 << PD0) | (1 << PD1); 		// Set PD0,PD1 as output
    //PORTB |= (1 << PB1);

    TCCR1B |= (1 << WGM12); 			// Set Timer1 to CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10); 	// Set prescaler to 1024
    OCR1A = OCR1A_VALUE; 			// Set OCR1A value for 1 second delay
    TIMSK |= (1 << OCIE1A); 			// Enable Timer1 compare match A interrupt

    sei(); 					// Enable global interrupts

    while(1)
    {
	 					// Main loop

	if (estado == ESPERAR_USUARIO) {
		if ( ((flag&(1<<START_STOP_FLAG))>0) && nivel_de_carga != 0) {
			estado = LAVAR;
		}

	} else if (estado == LAVAR) {
		if ( (estado_de_lavado == 5) ) {
			flag &= ~(1<<START_STOP_FLAG);
			estado_de_lavado=0;
			LED1=0;
			LED2=0;
			botones_LED = (botones_LED & 0x07) | (0x00);
			estado = ESPERAR_USUARIO;
		} else if (((flag&(1<<TIMER_FLAG))>0)){
			estado_de_lavado++;
			counter_1Hz=0;
			add_time();
			flag &= ~(1<<TIMER_FLAG);
		}
	}
    }
}


						// INTERRUPCIONES
ISR(TIMER1_COMPA_vect)
{
    counter_60Hz++;
    if (counter_60Hz == duty_cycle) {
        PORTB = 0x00;
        PORTD |= (1 << PD0) | (1 << PD1);
    } else if (counter_60Hz == 10) {
	counter_1Hz++;
	if (counter_1Hz == 255) {
	    dec_value();
	}
	display_LED();
	send_byte(botones_LED);
        counter_60Hz = 1;
	flag ^= (1<<Hz60_FLAG);
    }
}

ISR( INT0_vect )
{
	if ( ((PIND >> 3) & 0x03) != 0 ) {
		if (estado == ESPERAR_USUARIO) {
			nivel_de_carga = (PIND >> 3) & 0x03;
			botones_LED = (botones_LED & 0xF8) | (1<<(nivel_de_carga-1));
		}
	} else {
		flag ^= (1<<START_STOP_FLAG);
		botones_LED = (botones_LED & 0x7F) | ((flag&(1<<START_STOP_FLAG))<<6);
	}
}