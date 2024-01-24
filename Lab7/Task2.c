#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

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

    //start reading
    EECR |= (1<<EERE);

    while(EECR & (1<<EERE));

    if(EEDR == '\0'){
        return '\r';

    } else { 
        return EEDR;
        //return value
    }



    // _delay_ms(100);

    // return EEDR;
}

int main(){

    int address_count = 0;

    usart_init();

    //get the sentence and wrtie to the EEPROM
    while (1) {

        char get_char = usart_receive();

        if (get_char == '\r') {
            EEPROMwrite(address_count, '\0');
            address_count++;
            break;

        } else {

            EEPROMwrite(address_count, get_char);
            address_count++;
        }
    }
        
    _delay_ms(2000);

    //loop for first 1024 addresses
    for (int i=0; i<address_count; i++){

        char result = EEPROMread(i);
        usart_send(result);

    }

    return 0;
}