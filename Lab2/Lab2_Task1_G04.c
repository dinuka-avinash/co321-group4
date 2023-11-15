#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define DELAY 500

int main(){
    DDRD &= ~(1<<7);  //INPUT 7TH BIT OF  PORTD 
    DDRB = 0b00111111;  //OUTPUT 6 BITS OF PORTB

    unsigned char z=0;
    unsigned char x=0;  //TO CHECK WHETHER THE BUTTON IS PRESSED

    while (1){
        if((PIND & (1<<7)) && !x) {  
            //_delay_ms(DELAY);
            z++; //INCREMENTING THE NUMBER OF TIMES BUTTON IS PRESSED
            PORTB = z;
            _delay_ms(DELAY);
        }
        x = PIND & (1<<7);  //x=1 IF THE BUTTON IS PRESSED

    }
    
    return 0;
}