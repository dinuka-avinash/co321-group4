#include <avr/io.h>
#include <util/delay.h>
// initialize the USART to work at 9600 baud, 8-bit data, no parity, and 1 stop bit
void usart_init() {
    // Set baud rate to 9600 (for 16MHz clock)
    UBRR0 = 103;

    // Enable USART transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8-bit data, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}

// transmit a character
void usart_send(char data) {    
    while (!(UCSR0A & (1 << UDRE0)));   // Wait until the transmitter is ready
    UDR0 = data;
}

// transmit a string
void usart_send_string(char * str) {
    // Transmit each character of the string
    while (*str) {
        usart_send(*str);
        str++;
    }
}

int main() {
    // Initialize USART
    usart_init();

    // Transmit names with E-numbers
    usart_send_string("E/19/028 Nilakshi\n");
    _delay_ms(1000);  
    usart_send_string("E/19/028 Avinash\n");
    _delay_ms(1000);  
    usart_send_string("E/19/034 Kavindu\n");

    while (1) {
    }

    return 0;
}

