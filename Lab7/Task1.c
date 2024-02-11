#include <avr/io.h>
#include <stdio.h>

void EERPROM_write(unsigned int uiAddress, char ucData)
{
    // Wait for the completion of previous write.
    while (EECR & (1 << EEPE))
        ;
    // Set up address and data registers.
    EEAR = uiAddress;
    EEDR = ucData;

    // Write logical one to EEMPE. only if this is enabled, making EEPE = 1 enables writing.

    EECR |= (1 << EEMPE);

    // Start eeprom
    EECR |= (1 << EEPE);
}

// initialize the USART to work at 9600 baud, 8-bit data, no parity, and 1 stop bit
void usart_init()
{
    // Set baud rate to 9600 (for 16MHz clock)
    UBRR0 = 103;

    // Enable USART transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8-bit data, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// receive a character
char usart_receive()
{
    while (!(UCSR0A & (1 << RXC0)))
        ; // Wait until data is received
    return UDR0;
}

char EEPROMread(unsigned int uiAddress)
{
    // Wait for completion of previous write.
    while (EECR & (1 << EEPE))
        ;

    EEAR = uiAddress; // Set the address.

    // set up EEPROM read
    EECR |= (1 << EERE);

    return EEDR; // Return data from data register.
}

// transmit a character.
void usart_send(char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ; // Wait until the transmitter is ready
    UDR0 = data;
}

int main(void)
{
    usart_init();

    int i = 0;
    char sentence[100];
    char toSend[1024];

    // get the data
    while (sentence[i] != '\r')
    {
        sentence[i] = usart_receive();
        i++;
    }

    // write into EEPROM.
    for (int j = 0; j < i; j++)
    {
        EERPROM_write(j, sentence[j]);
    }

    // read from EEPROM.

    for (int j = 0; j < 1024; j++)
    {
        toSend[j] = EEPROMread(j);
    }

    // send the data.
    for (int j = 0; j < 1024; j++)
    {
        usart_send(toSend[j]);
    }

    return 0;
}


