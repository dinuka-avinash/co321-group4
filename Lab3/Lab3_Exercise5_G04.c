// Derivation of initial counter value:

// for 1000 µs and multiply it by 100 to get 100ms
// XTAL = 16MHz → Txtal _clock = 1µs/16
// Prescaler = 1:64 → Tcounter_clock = 64 × 1/16µs = 4 µs
// Counter increments needed = 1000 µs /4µs = 250 increments
// Initial counter value = 1+ 255 - 250 = 6


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(){
    DDRB |= (1<<5);

    TCNT0 = 6;

    TCCR0A = 0x00;
    TCCR0B = 0x03;

    TIMSK0 |= (1<<TOIE0);  // enable timer counter overflow interrupt.

    sei();  // Enable global interrupts

    while (1){}
    return 0;
}

int i = 0; 

ISR(TIMER0_OVF_vect){
    i++;
    if(i==100){
        PORTB ^= (1<<5);
        i=0;
    }
    TCNT0 = 6;
}