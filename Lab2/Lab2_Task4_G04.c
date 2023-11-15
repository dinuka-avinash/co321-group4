#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define DELAY 1000

int main(){
    DDRB = 0b00111111; //OUTPUT PORTB
    DDRD &= ~(1<<2);   //INPUT PD2
    DDRD &= ~(1<<3);   //INPUT PD3

    //RISING EDGE FOR BOTH BUTTONS
    EICRA |= (1<<ISC00);
    EICRA |= (1<<ISC10);
    EICRA |= (1<<ISC01);
    EICRA |= (1<<ISC11);

    sei();

    //ENABLE EXTERNAL INTERRUPT FOR BOTH INT0 NAD INT1
    EIMSK |= (1<<INT0);
    EIMSK |= (1<<INT1);

    while (1){}
    
    return 0;
}

unsigned char z = 0;

//FOR BUTTON A
ISR(INT0_vect){
    z = (z<<1) | 1;  //CHANGING THE MSB TO 1
    //PORTB = (PORTB<<1) | 1;
    _delay_ms(DELAY);
    PORTB = z;
    _delay_ms(DELAY);
}

//FOR BUTTON B
ISR(INT1_vect){
    z = (z<<1); //CHANGING THE MSB TO 0
    _delay_ms(DELAY);
    PORTB = z;
    _delay_ms(DELAY);
}

