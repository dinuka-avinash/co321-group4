#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init() {
    DDRC &= ~(1 << PC0);

    ADCSRA |= (1 << ADEN);    // Enable the ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);    // Set 64 as the conversion speed
    // ADCSRA |= (1 << ADATE);    

    ADMUX |= (1 << ADLAR);  // Set voltage reference as AVCC, left adjust result, and set ADC1 as the analog input channel

    UBRR0 = 103;

    // Enable USART transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8-bit data, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_send(uint16_t data) {    
    while (!(UCSR0A & (1 << UDRE0)));   // Wait until the transmitter is ready
    UDR0 = data;
}

// // transmit a string
// void usart_send_string(int str) {
//     // Transmit each character of the string
//     while (str) {
//         usart_send*str);
//         str++;
//     }
// }

int main(){

    DDRD |= (1<<PD7);

    adc_init(); 



    while (1){
    // PORTD = (1<<PD7);
    
    ADCSRA |= (1 << ADSC);  // Start the conversion

    while (!(ADCSRA & (1 << ADIF)) == 1){;}   // Wait until the conversion is over

    uint16_t decimal_val = (ADCH << 2) | (ADCL >> 6); // Get the converted decimal value from combining ADCL and ADCH to get 10-bit ADC result
    uint16_t voltage = (decimal_val * 8) / 64;

    //print value to serial monitor
    usart_send(voltage);

    ADCSRA |= (1 << ADIF);

    ADCH = b00000000;
    ADCL = b00000000;	

    }

    return 0;
}