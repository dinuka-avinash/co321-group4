#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
    DDRC &= ~(1 << PC1);

    ADCSRA |= (1 << ADEN);    // Enable the ADC
    // ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);    // Set 64 as the conversion speed
    ADCSRA |= (1 << ADATE);    

    ADMUX |= (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);  // Set voltage reference as AVCC, left adjust result, and set ADC1 as the analog input channel
}

int main() {
    DDRD |= 0xFF;  // Set all PORTD pins as output

    adc_init();

    while (1) {
        ADCSRA |= (1 << ADSC);  // Start the conversion

        while (!(ADCSRA & (1 << ADIF))){;}  // Wait until the conversion is over
        // while (!(ADCSRA & (1 << ADSC))){;}  // Wait until the conversion is over
        ADCSRA |= (1 << ADIF);

        uint16_t decimal_val = (ADCH << 2) | (ADCL >> 6); // Get the converted decimal value from combining ADCL and ADCH to get 10-bit ADC result

        uint16_t voltage = (decimal_val * 8) / 1023;

        for (int i = 0; i < voltage; i++) {
            PORTD |= (1 << i);  // Turn on the calculated number of LEDs
        }

        _delay_ms(1000);

          // Clear the ADIF flag
    }

    return 0;
}
