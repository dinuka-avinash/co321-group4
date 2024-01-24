#include <avr/io.h>

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