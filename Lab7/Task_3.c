#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <LiquidCrystal.h>

#define KEY_PRT PORTD
#define KEY_DDR DDRD
#define KEY_PIN PIND

#define ADDRESS 0x00
#define KEY_ADDR 0X0A
#define ALPHABET_SIZE 16

const char alphabet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};

unsigned char keypad[4][4] = {{'D', '#', '0', '*'},
                              {'C', '9', '8', '7'},
                              {'B', '6', '5', '4'},
                              {'A', '3', '2', '1'}};

unsigned char colloc, rowloc;

// LiquidCrystal lcd(0, 1, 5, 4, 3, 2);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);    

void EEPROMwrite(unsigned int address, char data)
{

  // wait for completion of previous write
  while (EECR & (1 << EEPE))
    ;

  // set up address and data regs
  EEAR = address;
  EEDR = data;

  // Write logical 1 to EEMPE
  EECR |= (1 << EEMPE);

  // start eeprom write
  EECR |= (1 << EEPE);
}

char EEPROMread(unsigned int address)
{
  // Wait for completion of writes
  while (EECR & (1 << EEPE))
    ;

  // set up address
  EEAR = address;

  // start eeprom read
  EECR |= (1 << EERE);

  // return data
  return EEDR;
}

char getKeyPressed()
{
  while (1)
  {
    KEY_DDR = 0xF0; /* set port direction as input-output */
    KEY_PRT = 0xFF;

    do
    {
      KEY_PRT &= 0x0F; /* mask PORT for column read only */
      asm("NOP");
      colloc = (KEY_PIN & 0x0F); /* read status of column */
    } while (colloc != 0x0F);

    do
    {
      do
      {
        _delay_ms(20);             /* 20ms key debounce time */
        colloc = (KEY_PIN & 0x0F); /* read status of column */
      } while (colloc == 0x0F);    /* check for any key press */

      _delay_ms(40); /* 20 ms key debounce time */
      colloc = (KEY_PIN & 0x0F);
    } while (colloc == 0x0F);

    /* now check for rows */
    KEY_PRT = 0xEF; /* check for pressed key in 1st row */
    asm("NOP");
    colloc = (KEY_PIN & 0x0F);
    if (colloc != 0x0F)
    {
      rowloc = 0;
      break;
    }

    KEY_PRT = 0xDF; /* check for pressed key in 2nd row */
    asm("NOP");
    colloc = (KEY_PIN & 0x0F);
    if (colloc != 0x0F)
    {
      rowloc = 1;
      break;
    }

    KEY_PRT = 0xBF; /* check for pressed key in 3rd row */
    asm("NOP");
    colloc = (KEY_PIN & 0x0F);
    if (colloc != 0x0F)
    {
      rowloc = 2;
      break;
    }

    KEY_PRT = 0x7F; /* check for pressed key in 4th row */
    asm("NOP");
    colloc = (KEY_PIN & 0x0F);
    if (colloc != 0x0F)
    {
      rowloc = 3;
      break;
    }
  }

  if (colloc == 0x0E)
    return (keypad[rowloc][0]);
  else if (colloc == 0x0D)
    return (keypad[rowloc][1]);
  else if (colloc == 0x0B)
    return (keypad[rowloc][2]);
  else
    return (keypad[rowloc][3]);
}

void encryptInput()
{
  unsigned int i;
  char c;
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Input (Size10)");
  _delay_ms(500);
  lcd.setCursor(0, 1);

  // enter each character
  for (i = 0; i < 10; i++)
  {
    c = getKeyPressed(); // get pressed key
    eeprom_write_byte((uint8_t *)(ADDRESS + i), c);
    lcd.print(c); // print character
  }

  _delay_ms(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encrypted Text");
  lcd.setCursor(0, 1);

  _delay_ms(300);

  // encript char by char and show
  for (i = 0; i < 10; i++)
  {
    // read from the memory
    c = eeprom_read_byte((uint8_t *)(ADDRESS + i));
    lcd.print(ceaserCipherr(c));
    _delay_ms(300);
  }

  _delay_ms(1000);
  lcd.clear();
  lcd.print("A to main menu");
  while ((c = getKeyPressed()) != 'A')
    ;
}

char ceaserCipherr(char c)
{
  int i;

  // find the index
  if (c >= 48 && c <= 57) // for numbers
    i = c - 48;

  else if (c >= 65 && c <= 68) // for A,B,C,D
    i = c - 65 + 10;

  else if (c == '*')
    i = 14;

  else
    i = 15;

  // read the key from the memory
  char secret_key = eeprom_read_byte((uint8_t *)KEY_ADDR);

  // encrypt
  return alphabet[(i + secret_key) % ALPHABET_SIZE];
}

void showSecretKey()
{
  char key;

  // read from the memory
  key = eeprom_read_byte((uint8_t *)KEY_ADDR);

  // display
  lcd.clear();
  lcd.print("Secret Key");
  lcd.setCursor(0, 1);
  lcd.print(c);

  _delay_ms(2000);
  lcd.clear();
  lcd.print("Menu - Press A");
  while ((c = getKeyPressed()) != 'A');
}

void changeSecretKey()
{
  char c;
  lcd.clear();
  lcd.print("Enter New Key");
  lcd.setCursor(0, 1);

  while ((c = getKeyPressed()) < '0' && c > '9')
    ;
  lcd.print(c);
  eeprom_write_byte((uint8_t *)KEY_ADDR, c);

  _delay_ms(2000);
}

int main()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("START");
  _delay_ms(2000);
  char c;

  while (1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Encrypt : 1");
    lcd.setCursor(0, 1);
    lcd.print("Secret Key : 2");

    while ((c = getKeyPressed()) != '1' && c != '2')
      ;

    if (c == '1')
    {
      encryptInput();
    }

    else
    {
      lcd.clear();
      lcd.print("View key : 1");
      lcd.setCursor(0, 1);
      lcd.print("Edit key : 2");

      while ((c = getKeyPressed()) != '1' && c != '2')
        ;

      if (c == '1')
      {
        showSecretKey();
      }

      else
      {
        changeSecretKey();
      }
    }
    _delay_ms(1000);
  }
}