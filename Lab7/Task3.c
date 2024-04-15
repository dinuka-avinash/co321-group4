// Headers
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
// #include "eeprom.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "customKey.h"

#define Key_None '\0'

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

// Timer0 setup
void Timer0_Setup(void)
{
    OCR0A = 124;
    TCNT0 = 0;
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS01) | _BV(CS00);
    TIMSK0 = _BV(OCIE0A);
}

// Timer0 interrupt service routine
ISR(TIMER0_COMPA_vect)
{
	check_key();
}


// Scroll text on LCD
void lcd_scroll(int line, char *str, char *static_str)
{
	char blank[16] = "                ";
	uint8_t len = strlen(str) + 16;
	char pad_str[len];
	strcpy(pad_str, str);
	strcat(pad_str, blank);

	lcd_clrscr();
	for (uint8_t i = 0; i <= len - 16; i++)
	{
		lcd_gotoxy(0, line);
		for (uint8_t j = 0; j < 16; j++)
		{
			int pos = (j + i) % len;
			lcd_putc(pad_str[pos]);
		}

		lcd_gotoxy(0, line ? 0 : 1);
		lcd_puts(static_str);
		_delay_ms(200);
	}
	lcd_clrscr();
}


// Encrypt plaintext
void Encrypt()
{
	lcd_clrscr();
	lcd_puts("Add Plaintext:\n");

	char secret_txt[3];
	secret_txt[2] = '\0';
	// EEPROM_read(100, secret_txt, 3);

    for(int i=0; i<2; i++){

        secret_txt[i] = EEPROMread(i);

    }

	int secret = atoi(secret_txt);

	char plaintext[17] = "\0";

	uint8_t idx = 0;
	volatile char key = Key_None; // IMPORTANT - If used with interrupt debouncing, must be volatile.

	while (1)
	{
		// Read key value from ISR sequence
		key = getKey();

		// If key is pressed
		if (key != Key_None)
		{
			if ((idx < 16) & ((key == 'A') | (key == 'B') | (key == 'C') | (key == 'D')))
			{
				plaintext[idx] = key;
				idx++;
			}
			else if (key == '*')
			{
				if (idx > 0)
					idx--;
				plaintext[idx] = '\0';
			}
			else if (key == '#')
			{
				// Wait until key is released
				waitKeyRelease();

				// Encrypt
				char ciphertext[17] = "\0";
				for (uint8_t i = 0; i < strlen(plaintext); i++)
				{
					ciphertext[i] = caesar_cipher(plaintext[i], secret);
				}

				lcd_clrscr();
				lcd_puts("Cipher text:\n");
				lcd_puts(ciphertext);

				while (1)
				{
					// Read key value from ISR sequence
					key = getKey();

					// If key is pressed
					if (key != Key_None)
					{
						if (key == '#')
						{
							// Wait until key is released
							waitKeyRelease();
							lcd_clrscr();
							break;
						}
					}
				}

				lcd_clrscr();
				break;
			}

			// Wait until key is released
			waitKeyRelease();

			lcd_clrscr();
			lcd_puts("Ent Plaintext:\n");
			lcd_puts(plaintext);
		}
	}
}

// Change secret
void ChangeSecret()
{
	lcd_clrscr();
	lcd_puts("Enter secret:\n");

	char secret[3];
	secret[2] = '\0';
	// EEPROM_read_batch(100, secret, 3);

    for(int i=0; i<2; i++){

        secret[i] = EEPROMread(i);

    }

	lcd_puts(secret);

	uint8_t idx = strlen(secret);

	volatile char key = Key_None; // IMPORTANT - If used with interrupt debouncing, must be volatile.

	while (1)
	{
		// Read key value from ISR sequence
		key = getKey();

		// If key is pressed
		if (key != Key_None)
		{
			if ((key >= 48) & (key <= 57) & (idx < 2))
			{
				secret[idx] = key;
				idx++;
			}
			else if (key == '*')
			{
				if (idx > 0)
					idx--;
				secret[idx] = '\0';
			}
			else if (key == '#')
			{
				// Wait until key is released
				waitKeyRelease();

				if ((atoi(secret) < 26) & (atoi(secret) > 0))
				{
					// EEPROM_update_batch(100, secret, 3);

                    for(int i=0; i<2; i++){

                        EEPROMwrite(i, secret[i]);

                    }

					lcd_clrscr();
					lcd_puts("Secret changed!\n");
					lcd_puts("New secret: ");

					char test[3];
					test[2] = '\0';

					// EEPROM_read_batch(100, test, 3);

                    for(int i=0; i<2; i++){

                        test[i] = EEPROMread(i);

                    }

					lcd_puts(test);
					_delay_ms(1000);
					lcd_clrscr();
					break;
				}
				else
				{
					lcd_clrscr();
					lcd_scroll(1, "Secret should be less than 26.", "Invalid secret!");
				}
			}

			// Wait until key is released
			waitKeyRelease();

			lcd_clrscr();
			lcd_puts("Enter secret:\n");

			lcd_puts(secret);
		}
	}
}

char caesar_cipher(char pt, int key) {
    if (isupper(pt)) {
        return 'A' + (pt - 'A' + key) % 26;
    } else if (islower(pt)) {
        return 'a' + (pt - 'a' + key) % 26;
    }
    return pt;
}

int main(void)
{
	// Variables
	volatile char key = Key_None; // IMPORTANT - If used with interrupt debouncing, must be volatile.

	// Setup
	keypad_init();		// Setup keypad
	Timer0_Setup(); // Setup timer0 to call interrupt every 1ms @ Fclk = 8MHz
	sei();

	/* initialize display, cursor off */
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();

	lcd_scroll(0, "CeaserCipher Encryption", "Choose action :");

	lcd_puts("1. Encrypt\n2. Change secret");

	// Loop
	while (1)
	{
		lcd_gotoxy(0, 0);
		lcd_puts("1. Encrypt\n2. Change Secret");

		// Read key value from ISR sequence
		key = getKey();

		// If key is pressed
		if (key != Key_None)
		{
			if (key == '1')
			{
				// Wait until key is released
				waitKeyRelease();
				Encrypt();
			}
			if (key == '2')
			{
				// Wait until key is released
				waitKeyRelease();
				ChangeSecret();
			}
			if (key == '*')
			{
				// Wait until key is released
				waitKeyRelease();
				lcd_clrscr();
			}

			// Wait until key is released
			waitKeyRelease();
		}
	}

	return 0;
}




