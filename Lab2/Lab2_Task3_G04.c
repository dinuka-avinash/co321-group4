#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define DELAY 500

int main(){
    DDRB = 0x0F;  //OUTPUT PORTB
    DDRD &= ~(1<<2);  //INPUT PD2

    //unsigned char z=0;

    EICRA |= (1<<ISC01);
    EICRA |= (1<<ISC00);

    sei();

    EIMSK |= (1<<INT0);

    while (1){}
    
    return 0;

}

unsigned char z;

ISR(INT0_vect){
    _delay_ms(DELAY);
    z++; //INCREMENTING NUMBER OF TIMES BUTTON IS PRESSED
    PORTB = z; 
    _delay_ms(DELAY);

}