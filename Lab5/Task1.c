#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(){
    DDRD |= 0xFF;   // Initialize the PORTD as output
    DDRC &= ~(1<<ADC1);

    ADCSRA |= (1<<ADEN);    // Enable the ADC
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1);    // Set 64 as the conversion speed

    ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX0);  // Set voltage reference as AVCC, left adjust the result, and set ADC1 as the analog input channel

    while(1){
        ADSCRA |= (1<<ADSC);    // Start the conversion

        while(!(ADCSRA & (1<<ADIF)));   // Wait until the conversion is over

        //not clear
        uint16_t decimal_val = (ADCH<<2) | (ADCL>>6); // Get the converted decimal value from combining ADCL and ADCH to get 10-bit ADC result

        uint16_t voltage = (decimal_val * 8) / 1023; // Get the voltage value

        for(int i = 0; i < voltage; i++){
            PORTD |= (1<<i);    // Turn on the calculated LEDs
        }

        for(int i = voltage; i >= 0; i--){
            PORTD &= ~(1<<i);   // Turn off the remaining LEDs
        }

        ADCSRA |= (1<<ADIF);    // Clear the ADC start conversion bit

    }
    
}