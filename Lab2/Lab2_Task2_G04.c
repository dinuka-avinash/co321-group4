#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define DELAY 500

int main(){
    DDRD &= ~(1<<2); //INPUT 2ND BIT OF PORTD
    DDRB |= (1<<0);  //OUTPUT PB0

    EICRA |= (0<<ISC00);  //FOR RISING EDGE DETECTION
    EICRA |= (1<<ISC01);

    sei();  //ENABLE GLOBAL INTERRUPTS

    EIMSK |= (1<<INT0); //ENABLE EXTERNAL INTERRUPT FOR INT0

    while(1){} //STAY IN THE MAIN CODE

}

ISR(INT0_vect){
    _delay_ms(DELAY);
    PORTB ^= (1<<0);  //TOGGLE PB0
    _delay_ms(DELAY);
}