// Derivation of initial counter value:

// XTAL = 16MHz → Txtal _clock = 1µs/16
// Prescaler = 1:1024 → Tcounter_clock = 1024 × 1/16µs = 64 µs
// Counter increments needed = 1000 000µs /64µs = 15625 increments
// Initial counter value = 1+ 65535 – 15625 = 49911

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define DELAY_TIMER 100

int main()
{
    DDRB = 0x3F;   // 0b00111111 for 5 outputs

    TCNT1 = 49911;

    TCCR1A = 0x00;   // Normal Mode
    TCCR1B = 0x05;   // Prescalar = 1024

    TIMSK1 |= (1 << TOIE1);   // enable timer counter overflow interrupt.

    sei();   // Enable global interrupts

    // First enable timer counter overflow interrupt, Then global.

    // uint8_t mask = 0b00001111;

    while (1)
    {
        uint8_t i=0;

        //toggling LEDs for knight rider circuit
        for(i=0; i<4; i++){
            PORTB = PORTB | (1<<i);
            _delay_ms(DELAY_TIMER);
          	PORTB = PORTB &~ (1<<i);
          	_delay_ms(DELAY_TIMER);
        }

        for(i=2; i<=0; i--){
            PORTB = PORTB | (1<<i);
            _delay_ms(DELAY_TIMER);
          	PORTB = PORTB &~ (1<<i);
          	_delay_ms(DELAY_TIMER);
        }

    }
    return 0;
}

// This will start executing everytime, when timer overflows. It will stop running main function and start execution of the
// below part. Once it is completed, rest of the main function will start executing.

ISR(TIMER1_OVF_vect){
    PORTB ^= (1 << 5); // toggle the port B
    TCNT1 = 49911;     // Set initial value of  timer/ counter register to 49911, so that start counting again.
}
