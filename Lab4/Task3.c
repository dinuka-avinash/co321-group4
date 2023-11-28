#include <avr/interrupt.h>
#include <avr/io.h>

int main(){
    DDRB = 0x01;

    OCR0A = 127;

    TCCR0A = 0x68;
    TCCR0B = 0x03;

    while(1){}

    return 0;
}