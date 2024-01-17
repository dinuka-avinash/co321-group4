#include <avr/io.h>

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

// receive a character
char usart_receive() {    
    while (!(UCSR0A & (1 << RXC0)));   // Wait until data is received
    return UDR0;
}

