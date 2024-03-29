/*************************************************************************
Title:    Testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://tinyurl.com/peterfleury
File:     $Id: test_lcd.c,v 1.8 2015/01/31 18:04:08 peter Exp $
Software: AVR-GCC 4.x
Hardware: HD44780 compatible LCD text display
          AVR with external SRAM interface if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
#include "lcd.c"


/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};

int main(void)
{
    char buffer[7];
    int  num=134;
    unsigned char i;
    
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);

    for (;;) {/* loop forever */
        /* 
         * Test 1:  write text to display
         */

        /* clear display and home cursor */
        lcd_clrscr();
        
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test 5\n");
        
        /* move cursor to position 8 on line 2 */
        lcd_gotoxy(7,1);  
        
        /* write single char to display */
        lcd_putc(':');
        
        _delay_ms(5000);
		
        lcd_clrscr();
		_delay_ms(1000);
        
        /*
         * Test 2: use lcd_command() to turn on cursor
         */
        
        /* turn on cursor */
        lcd_command(LCD_DISP_ON_CURSOR);

        /* put string */
        lcd_puts("CurOn");
        
        _delay_ms(5000);

        /*
         * Test 3: display shift
         */
        
        lcd_clrscr();     /* clear display home cursor */
		_delay_ms(1000);
        
		//* put string from program memory to display */
        lcd_puts_P( "Line 1 longer than 14 characters\n" );
        lcd_puts_P( "Line 2 longer than 14 characters" );
        
        /* move BOTH lines one position to the left */
        lcd_command(LCD_MOVE_DISP_LEFT);
        
        _delay_ms(5000);

        /* turn off cursor */
        lcd_command(LCD_DISP_ON);
        _delay_ms(1000);
        
        /*
         *   Test: Display integer values
         */
        
		lcd_clrscr();   /* clear display home cursor */
        _delay_ms(1000);
        
		/* convert interger into string */
		itoa( num , buffer, 10);
        
        /* put converted string to display */
        lcd_puts(buffer);
        
        _delay_ms(5000);
        
        
        /*
         *  Test: Display userdefined characters
         */

        lcd_clrscr();   /* clear display home cursor */
        _delay_ms(1000);
        lcd_puts("Copyright: ");
       
       /*
        * load two userdefined characters from program memory
        * into LCD controller CG RAM location 0 and 1
        */
        lcd_command(_BV(LCD_CGRAM));  /* set CG RAM start address 0 */
        for(i=0; i<16; i++){
           lcd_data(pgm_read_byte_near(&copyRightChar[i]));
        }
       
       /* move cursor to position 0 on line 2 */
       /* Note: this switched back to DD RAM adresses */
       lcd_gotoxy(0,1);
       
       /* display user defined (c), built using two user defined chars */
       lcd_putc(0);
       lcd_putc(1);
       
		_delay_ms(5000);
              
    }
}
