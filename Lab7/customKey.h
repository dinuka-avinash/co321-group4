#include <stdint.h>
#ifndef CUSTOMKEY_H
#define CUSTOMKEY_H

void keypad_init();
char getKey();
void waitKeyRelease();
void check_key();

#endif
