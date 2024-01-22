#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init() {
    // DDRC &= ~(1 << PC1);

    ADCSRA |= (1 << ADEN);    // Enable the ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);    // Set 64 as the conversion speed
    // ADCSRA |= (1 << ADATE);    

    ADMUX |= (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);  // Set voltage reference as AVCC, left adjust result, and set ADC1 as the analog input channel
    UBRR0 = 103;

    // Enable USART transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8-bit data, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_send(int data) {    
    while (!(UCSR0A & (1 << UDRE0)));   // Wait until the transmitter is ready
    UDR0 = data;
}


int main() {
    DDRD |= 0xFF;  // Set all PORTD pins as output

    adc_init();

    while (1) {
        DDRC = (1 << PC1);
        ADCSRA |= (1 << ADSC);  // Start the conversion

        while (!(ADCSRA & (1 << ADIF)) == 1){;}   // Wait until the conversion is over
        // while (!(ADCSRA & (1 << ADSC)) == 1){;}  // Wait until the conversion is over
        
        uint16_t decimal_val = (ADCH << 2) | (ADCL >> 6); // Get the converted decimal value from combining ADCL and ADCH to get 10-bit ADC result

        int voltage = (decimal_val * 8) / 1023;

        usart_send(voltage);

        PORTD = 0x00;  // Turn off all LEDs

        for (int i = 0; i < voltage; i++) {
            PORTD |= (1 << i); 
            //_delay_ms(1000); // Turn on the calculated number of LEDs
        }

        _delay_ms(1000);

        ADCH = 0x00;
        ADCL = 0x00;
        voltage = 0;
        ADCSRA |= (1 << ADIF);

          // Clear the ADIF flag
    }

    return 0;
}
