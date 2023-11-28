#include <avr/interrupt.h>
#include <avr/io.h>

int main(){
    DDRB = 0x01;

    OCR0A = 127;

    TCCR0A = 0x68;
    TCCR0A = 0x68;

    while(1){
        for(int i=2; i<6; i++){
            TCCR0B = i;
            _delay_ms(1000);
        }
    }

    return 0;
}
