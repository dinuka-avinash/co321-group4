// Derivation of initial counter value:

// for 2ms
// XTAL = 16MHz → Txtal _clock = 1µs/16
// Prescaler = 1:256 → Tcounter_clock = 256 × 1/16µs = 16 µs
// Counter increments needed = 2000µs / 16µs = 125 increments
// Initial counter value = 1+255 – 125 = 131

// for 500ms
// XTAL = 16MHz → Txtal _clock = 1µs/16
// Prescaler = 1:256 → Tcounter_clock = 256 × 1/16µs = 16 µs
// Counter increments needed = 500 000µs /16µs = 31250 increments
// Initial counter value = 1+ 65535 – 31250 = 34286

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(){
    DDRB = 0x03;

    TCNT0 = 131;
    TCNT1 = 34286;

    TCCR0A = 0x00;
    TCCR0B = 0x04;

    TCCR1A = 0x00;
    TCCR1B = 0x04;

    // enable timer counter overflow interrupt.
    TIMSK0 |= (1<<TOIE0);
    TIMSK1 |= (1<<TOIE1);
    
    sei();   // Enable global interrupts

    while(1){}
    return 0;
}

int i=0;

//ISR is executed everytime the timer overflows
ISR(TIMER0_OVF_vect){
    i++;
    if(i==25){  //if block is executed only when the i reaches 25 (25 x 2ms delay)
        PORTB ^= (1<<0);
        i=0; //again set to 0
    }
    TCNT0 = 131; //initial timer value
}

ISR(TIMER1_OVF_vect){
    PORTB ^= (1<<1);    //toggling the LED
    TCNT1 = 34286;
}
