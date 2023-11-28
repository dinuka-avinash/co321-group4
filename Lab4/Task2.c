#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(){
    DDRB = 0x01;

    OCR0A = 127;

    TCCR0A = 0x68;
    TCCR0B = 0x03;

    while(1){
        for(int i = 0; i < 256; i++){
            OCR0A = i;
            _delay_ms(100);
        }
        
        for(int i = 255; i >= 0; i--){
            OCR0A = i;
            _delay_ms(100);
        }
    }

    return 0;
}