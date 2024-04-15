#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "customKey.h"

void keypad_init()
{
    // set Rows as output and Columns as input
    DDRD |= _BV(PD6) | _BV(PD7);
    DDRB |= _BV(PB0) | _BV(PB1);

    // Set Rows to low
    PORTD &= ~_BV(PD6) | ~_BV(PD7);
    PORTB &= ~_BV(PB0) | ~_BV(PB1);

    // Set PORTB as input
    DDRB &= ~_BV(PB2) | ~_BV(PB3) | ~_BV(PB4) | ~_BV(PB5);
    // Enable pull-up resistors
    PORTB |= _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);
}

uint8_t check_col()
{
    uint8_t col = 4;

    if (!(PINB & _BV(PB2)))
    {
        col = 0;
    }
    else if (!(PINB & _BV(PB3)))
    {
        col = 1;
    }
    else if (!(PINB & _BV(PB4)))
    {
        col = 2;
    }
    else if (!(PINB & _BV(PB5)))
    {
        col = 3;
    }
    else
    {
        col = 4;
    }

    return col;
}

void check_row(uint8_t row)
{
    if (row == 0)
    {
        // Set only PD6 to low
        PORTD &= ~_BV(PD6);
        PORTD |= _BV(PD7);
        PORTB |= _BV(PB0) | _BV(PB1);
    }

    if (row == 1)
    {
        // Set only PD7 to low
        PORTD &= ~_BV(PD7);
        PORTD |= _BV(PD6);
        PORTB |= _BV(PB0) | _BV(PB1);
    }

    if (row == 2)
    {
        // Set only PB0 to low
        PORTB &= ~_BV(PB0);
        PORTD |= _BV(PD6) | _BV(PD7);
        PORTB |= _BV(PB1);
    }

    if (row == 3)
    {
        // Set only PB1 to low
        PORTB &= ~_BV(PB1);
        PORTB |= _BV(PB0);
        PORTD |= _BV(PD6) | _BV(PD7);
    }
}

char keypad_matrix[4][4] = {{'1', '2', '3', 'A'},
                            {'4', '5', '6', 'B'},
                            {'7', '8', '9', 'C'},
                            {'*', '0', '#', 'D'}};

char KEY_BUFFER = '\0';

void check_key()
{
    for (uint8_t row = 0; row < 4; row++)
    {
        check_row(row);
        uint8_t col = check_col();

        if (col < 4)
        {
            KEY_BUFFER = keypad_matrix[row][col];
            return;
        }
    }
    KEY_BUFFER = '\0';
}

char getKey()
{
    return KEY_BUFFER;
}

void waitKeyRelease()
{
    while (check_col() != 4)
        ;
    KEY_BUFFER = '\0';
    _delay_ms(10);
}