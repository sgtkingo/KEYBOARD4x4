/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * Author: Jiri Konecny 
 * Comments: EEPROM 22LC64 function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MCP23017_FUNCTION
#define	MCP23017_FUNCTION

#include "MCP23017_Config.h"

void Init_MCP23017();
void Config_MCP23017_Fast();

void Send_ConfigToRegister(unsigned char reg, unsigned char cmd);
void ChangeIOCONMode(unsigned char mode);
void GoToAddress(unsigned char addr);

unsigned char Read_DataFromPort(unsigned char port);
void Send_DataToPort(unsigned char port, unsigned char data);
void Set_DirectionOfPort(unsigned char port, unsigned char dir);

void Set_IOC(unsigned char port, unsigned char pins, unsigned char mode, unsigned char defval);
unsigned char Get_IOC_Flag(unsigned char port);
unsigned char Get_IOC_Cap(unsigned char port);

void Set_PullUp(unsigned char port, unsigned char pins);
void Set_PortPol(unsigned char port, unsigned char pins);

void Config_MCP23017_Fast(){
    I2C_PAUSE(1000);
    
    I2C_STR();
    WriteI2C(ADR_MCP23017_W);
    
    WriteI2C(0xFF); //IODIRA
    WriteI2C(0xFF); //IODIRB
    WriteI2C(0x00); //IPOLA
    WriteI2C(0x00); //IPOLB
    WriteI2C(0x00); //GPINTENA
    WriteI2C(0x00); //GPINTENB
    WriteI2C(0x00); //DEFVALA
    WriteI2C(0x00); //DEFVALB
    WriteI2C(0x00); //INTCONA
    WriteI2C(0x00); //INTCONB
    WriteI2C(IOCON_SEQ_16bit); //IOCON
    WriteI2C(IOCON_SEQ_16bit); //IOCON
    WriteI2C(0x00); //GPPUA
    WriteI2C(0x00); //GPPUB
    WriteI2C(0x00); //INTFA
    WriteI2C(0x00); //INTFB
    WriteI2C(0x00); //INTCAPA
    WriteI2C(0x00); //INTCAPB
    WriteI2C(0x00); //GPIOA
    WriteI2C(0x00); //GPIOB    
    WriteI2C(0x00); //OLATA
    WriteI2C(0x00); //OLATB    
    
    I2C_STP();   
    
    ChangeIOCONMode(IOCON_NOSEQ_8bit); //swap to 8bit A,B mode
}

void Init_MCP23017(){
    I2C_INIT();
    Config_MCP23017_Fast(); //Default config and swap to paraler 8 bit mode A,B
}

void ChangeIOCONMode(unsigned char mode){
    if(mode == IOCON_NOSEQ_8bit){
        Send_ConfigToRegister(IOCON_S,mode);
    }else if(mode == IOCON_SEQ_16bit){
        Send_ConfigToRegister(IOCON_NS,mode);
    }
    I2C_PAUSE(5000);
}

void GoToAddress(unsigned char addr){
    I2C_STR();
    WriteI2C(ADR_MCP23017_W);
    WriteI2C(addr); 
    I2C_STP();
    
    I2C_PAUSE(1000);
}

void Send_ConfigToRegister(unsigned char reg, unsigned char cmd){
    I2C_STR();
    WriteI2C(ADR_MCP23017_W);
    WriteI2C(reg);
    WriteI2C(cmd);
    I2C_STP();   
    
    I2C_PAUSE(1000);
}

char Read_NoSeqConfigFromRegister(unsigned char reg){     
    I2C_PAUSE(1000); 
    char data=0; 
    
    GoToAddress(reg);
    
    I2C_STR();
    WriteI2C(ADR_MCP23017_R); //Read
    data=I2CRead(false);
    I2C_STP();
    
    return data;
}

char* Read_SeqConfigFromRegister(){
    ChangeIOCONMode(IOCON_SEQ_16bit); //Enable Sequential mode, BANK=0, Slew Rate=1
    
    char data[MCP2017_MEM_SIZE];
    
    GoToAddress(0x00);
    
    I2C_STR();
    WriteI2C(ADR_MCP23017_R); //Read

    for(int i=0;i<22;i++){
        data[i]=I2CRead(true);
    }
    I2C_STP();
    
    ChangeIOCONMode(IOCON_NOSEQ_8bit); //Disable Sequential mode, BANK=1, Slew Rate=0
    
    return data;
}

unsigned char Read_DataFromPort(unsigned char port){
    return Read_NoSeqConfigFromRegister(port);
}

void Send_DataToPort(unsigned char port, unsigned char data){
    Send_ConfigToRegister(port,data);
}

void Set_DirectionOfPort(unsigned char port, unsigned char dir){
    Send_ConfigToRegister(port,dir);
}

void Set_IOC(unsigned char port, unsigned char pins, unsigned char mode, unsigned char defval){
    //Enable and config ioc on port
    Send_ConfigToRegister(GPINTEN | port,pins);
    if ( mode == true){
        Send_ConfigToRegister(DEFVAL | port,defval); 
        Send_ConfigToRegister(INTCON_ | port,pins);
    }  
}

unsigned char Get_IOC_Flag(unsigned char port){
    Read_NoSeqConfigFromRegister(INTF | port);
}

unsigned char Get_IOC_Cap(unsigned char port){
    Read_NoSeqConfigFromRegister(INTCAP | port);
}

void Set_PullUp(unsigned char port, unsigned char pins){
    Send_ConfigToRegister(GPPU | port,pins);
}

void Set_PortPol(unsigned char port, unsigned char pins){
    Send_ConfigToRegister(IPOL | port,pins);
}

#endif	/* MCP23017_FUNCTION */

