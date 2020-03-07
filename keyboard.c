#include "PICF18LF46K22_ConfigSFR.h"
#include "KEYBOARD4x4_via_MCP23017.h"

//declared init fce
void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    OSCTUNE=0x00; //PLLx4 OFF
    ANSELD=0x00;
    TRISD=0x00;
}
//declared clear fce
void ClearDevice(){
    PORTD=0;
    LATD=0;
}
//declared test fce
void TestDevice(){
    delay_ms(1000);
    LATD=0xFF;
    delay_ms(1000);
    LATD=0x00;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
    
    Init_Keyboard();

    while(1){
        delay_ms(100);
        LATD=Read_Keyboard();
        NOP();
    }
    return;
}
