#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <ctype.h>

// initialize the USART to work at 9600 baud, 8-bit data, no parity, and 1 stop bit
void usart_init() {
    // Set baud rate to 9600 (for 16MHz clock) -103 received from tables
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

char caesar_cipher(char pt) {
    if (isupper(pt)) {
        return 'A' + (pt - 'A' + 3) % 26;
    } else if (islower(pt)) {
        return 'a' + (pt - 'a' + 3) % 26;
    }
    return pt;
}

int main() {
    
    //usart initialization
    usart_init();
    char plaintext[100];
    char ciphertext[100];
    int i = 0; //counter for plaintext


    while (1) {
        char get_char = usart_receive();
        if (get_char == '\r') {
            plaintext[i] = '\0'; //add null terminate
            break;
        } else {
            plaintext[i] = get_char;
            i++;
        }
    }

        for(int k=0; plaintext[k] != '\0'; k++){
            ciphertext[k] = caesar_cipher(plaintext[k]);
        }	

        printf("Plaintext: %s\n", plaintext);

        for(int j=0; plaintext[j] != '\0'; j++){
            usart_send(ciphertext[j]);
        }   

    return 0;
}

