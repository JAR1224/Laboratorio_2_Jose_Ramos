//#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include<avr/io.h>
uint8_t tosend[4]={'t','e','s','t'};
//PB7 -SCL
//PB6 - SDA

void i2c_transfer()
{
  do
  {
  USICR|=(1<<USITC);   //Clock signal from SCL
  while((PINB&(1<<PB7))); //Waiting for SCL to go high
  //PORTB=(1<<PB0);
  //while(1) {}
  _delay_ms(5/1000);
  USICR|=(1<<USITC);  //Toggle SCL to go low
  _delay_ms(5/1000);
  }while(!(USISR&(1<<USIOIF)));  //Repeat clock generation at SCL until the counter overflows and a byte is transferred
  PORTB=(1<<PB0);
  while(1) {}
  _delay_ms(5/1000);
  USISR|=(1<<USIOIF);      //Clear overflow flag
}

void i2c_ack()
{
  DDRB&=~(0<<PB6);  //Change direction of SDA to receive acknowledge bit
  USISR|=(1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1); //Set counter to 1110 to force it to overflow when ACK bit is received
  i2c_transfer();   //Byte transmission
}

void i2c_actual_data()
{
  DDRB|=(1<<PB6);      
  short int i=0;
  while(tosend[i]!='\0')  //Actual data string "test"
  {
    USIDR=tosend[i];     //Placing byte in Data register
    i2c_transfer();      //Transfer the data placed in register
    i++;
  }
}

void i2c_start()
{
  PORTB&=~(1<<PB6);  //Pulling SDA line low
  _delay_ms(5/1000);
  PORTB&=~(1<<PB7);  //Pulling SLC line low
  _delay_ms(5/1000);
  while(USISIF==1); //detection of start condition
}

void initialize()
{
  USICR=(1<<USIWM1)|(1<<USICS1)|(1<<USICLK);  //TWI mode
  DDRB=(0<<PB6)|(0<<PB7); //SDA & SCL direction as output
  PORTB=(1<<PB6)|(1<<PB7); //SDA & SCL default state
  //while(1) {}
  i2c_start();
  //PORTB=(1<<PB0);
  //while(1) {}
  USIDR=0b00000000; //address of slave and select write operation
  i2c_transfer();
  //PORTB=(1<<PB0);
  //while(1) {}
  i2c_ack();
}

void i2c_stop()
{
  PORTB|=(1<<PB7);       //Pulling SDA high 
  _delay_ms(5/1000);
  PORTB|=(1<<PB6);       //Pulling SCL low
  _delay_ms(5/1000);
}

int main()
{
 if (USISIF==1) {
   //PORTB=(0<<PB0);
 } else {
   //PORTB=(1<<PB0);
 }
 //while(1) {}
 initialize();      //initialize i2c
 //PORTB=(1<<PB0);
 //while(1) {}
 char usi_data;
 usi_data=USIDR;
 if(usi_data&0x01)
 i2c_actual_data();  //transmit data
 i2c_stop();         //i2c stop
 return 0;
}