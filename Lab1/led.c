#include <avr/io.h>
#include <util/delay.h>
#define DELAY 200

int main(void){

    

    DDRB = 0x0F;

    while(1){

        PORTB = PORTB | (1<<1);

        _delay_ms(DELAY);

        PORTB = PORTB & ~(1<<1);
        
        _delay_ms(DELAY);
    }


}

