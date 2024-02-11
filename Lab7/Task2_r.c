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

char EEPROMread(unsigned int address)
{
  // Wait for completion of writes
  while (EECR & (1 << EEPE));

  // set up address
  EEAR = address;

  // start eeprom read
  EECR |= (1 << EERE);

  // return data
  return EEDR;
}

int main()
{

  usart_init();

  unsigned int address = 0;   // Initialize the address as 0
  char data;                  // Variable to keep char data

  // Loop from address 0 to x
  for (address = 0; address < 1023; address++)
  {
    data = EEPROMread(address);   // Read each address in EEPROM
    usart_send(data);
  }
  
  return 0;
}