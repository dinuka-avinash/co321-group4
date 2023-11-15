#include <avr/io.h>

void delay_fun0()
{
    // Initial value
    TCNT0 = 131; 

    //  start in [time scalar] mode. Timer counter control register
    TCCR0A = 0x00; 
    TCCR0B = 0x04; //change according to the timescale

    //waiting until the TOV1 is set
    while ((TIFR0 & 0x01) == 0);

    //reseting the timer settings to stop the timer
    TCCR1A = 0x00;
    TCCR1B = 0X00;

    // set the TOv0 bit to 1 for next round.
    TIFR1 = 0x01; 
}

void delay_fun1()
{
    // Initial value
    TCNT1 = 131; 

    //  start in [time scalar] mode. Timer counter control register
    TCCR1A = 0x00; 
    TCCR1B = 0x04; //change according to the timescale

    //waiting until the TOV1 is set
    while ((TIFR1 & 0x01) == 0);

    //reseting the timer settings to stop the timer
    TCCR1A = 0x00;
    TCCR1B = 0X00;

    // set the TOv0 bit to 1 for next round.
    TIFR1 = 0x01; 
}

int main(void)
{
    //setting PB0 and PB1 as output pins for two leds
    DDRB = 0x02;

    while (1)
    {
	
	PORTB ^= (1 << 5);
        for (int i = 0; i < 250; i++)
        {
            
            delay_fun();
        }

       
    }
}