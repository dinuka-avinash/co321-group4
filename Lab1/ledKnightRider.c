#include <avr/io.h>
#include <util/delay.h>

#define DELAY 200

int main(void)
{

    DDRB = 0x0F; //  PORTB is O/P

    while (1)
    {
        PORTB = 0b00001000;
        _delay_ms(DELAY);

        PORTB = 0b00000100;
        _delay_ms(DELAY);

        PORTB = 0b00000010;
        _delay_ms(DELAY);

        PORTB = 0b00000001;
        _delay_ms(DELAY);
        
        PORTB = 0b00000010;
        _delay_ms(DELAY);

        PORTB = 0b00000100;
        _delay_ms(DELAY);
    }
} 