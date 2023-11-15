#include <avr/io.h>
#include <util/delay.h>
#define DELAY 200

int main(void){

    

    DDRB = 0x0F;

    while(1){

        PORTB = 0b00000000;

        _delay_ms(DELAY);

        PORTB = 0b00001111;
        
        _delay_ms(DELAY);
    }


}

