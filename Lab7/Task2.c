#include <avr/io.h>

void usart_init() {
    // Set baud rate to 9600 (for 16MHz clock)
    UBRR0 = 103;

    // Enable USART transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8-bit data, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}

char usart_receive() {    
    while (!(UCSR0A & (1 << RXC0)));   // Wait until data is received
    return UDR0;
}

// transmit a character
void usart_send(char data) {    
    while (!(UCSR0A & (1 << UDRE0)));   // Wait until the transmitter is ready
    UDR0 = data;
}

void EEPROMwrite(unsigned int address, char data){

    //wait for prevous write complete
    while(EECR & (1<<EEPE));

    //set values
    EEAR = address;
    EEDR = data;

    //allow master write
    EECR |= (1<<EEMPE);

    //start write
    EECR |= (1<<EEPE);
}

char EEPROMread(unsigned int address){

    //wait for completion of write
    while(EECR & (1<<EEPE));

    //set address
    EEAR = address;

    //start readung
    EECR |= (1<EERE);

    //return data
    return EEDR;
}

int address_count = 0;

int main(){

    usart_init();

    //get the sentence and wrtie to the EEPROM
    while (1) {
        char get_char = usart_receive();
        if (get_char == '\r') {
            break;

        } else {
            EEPROMwrite(address_count, get_char);
            address_count++;
        }
    }

    //loop for first 1024 addresses
    for (int i=0; i<1023; i++){

        usart_send(EEPROMread(i));

    }
    return 0;
}