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

void EEPROMwrite(unsigned int address, char data)
{

  // wait for completion of previous write
  while (EECR & (1 << EEPE));

  // set up address and data regs
  EEAR = address;
  EEDR = data;

  // Write logical 1 to EEMPE
  EECR |= (1 << EEMPE);

  // start eeprom write
  EECR |= (1 << EEPE);
}


int main()
{

  usart_init();

  unsigned int address = 0, i = 0, j = 0; // Initialize the address as 0 and the i, j variables
  char words[1024];                       // Define a char array of size 1024
  char data;                              // Variable to keep char data

  while (1)
  {

    data = ' ';

    while (data != '\r')
    {
      data = usart_receive();   // Get the character from serial communication
      words[i] = data;          // Store it in the words array
      i++;                      // Increment the i by one
    }

    data = ' ';
    words[i] = '\n';            // Add the new line chacrater to the end of the sentence

    while (j <= i)
    {
      // usart_send(words[j]);
      EEPROMwrite(address, words[j]);  // Get characters one by one from words array and store it in EEPROM
      address++;
      j++;
    }

    i++;
  }

  return 0;
}