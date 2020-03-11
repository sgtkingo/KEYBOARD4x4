
#ifndef KEYBOARD4x4_CHARMAP_H
#define	KEYBOARD4x4_CHARMAP_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define N_KEYBOARD 16
char getChar(unsigned char value);

char charSet[N_KEYBOARD]={
    '1',
    '4',
    '7',
    '*',
    '2',
    '5',
    '8',
    '0',
    '3',
    '6',
    '9',
    '#',
    'A',
    'B',
    'C',
    'D',
};

char getChar(unsigned char value){
    if(value < 1 || value > N_KEYBOARD){
        return 0;
    }
    else return charSet[value-1];
}

#endif	/* KEYBOARD4x4_CHARMAP_H */

