#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init() {

    ADCSRA |= (1 << ADEN);    // Enable the ADC
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);    // Set 64 as the conversion speed 

    ADMUX |= (1 << ADLAR);  // Set voltage reference as AVCC, left adjust result, and set ADC1 as the analog input channel

}

int main(){

    DDRD |= (1<<PD7);

    adc_init(); 

    while (1){
    DDRC &= ~(1 << PC0);
    
    ADCSRA |= (1 << ADSC);  // Start the conversion

    while (!(ADCSRA & (1 << ADIF)) == 1){;}   // Wait until the conversion is over

    unsigned int decimal_val = (ADCL >> 6) | (ADCH << 2); // Get the converted decimal value from combining ADCL and ADCH to get 10-bit ADC result
    
    unsigned int voltage = (decimal_val * 8) / 1023;

    if (voltage < 2){
        PORTD |= (1<<PD7);
    }

    if (voltage >= 2){
        PORTD &= ~(1<<PD7);
    }

    ADCSRA |= (1 << ADIF);	
    }

    return 0;
}