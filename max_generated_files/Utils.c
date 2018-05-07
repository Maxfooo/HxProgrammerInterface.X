
#include "Utils.h"

extern int uC_i2c_mode;

void initPortA(char InOut, char AnlgDgtl)
{
    PORTA_LATCH = 0x00;
    PORTA_DATA_DIR = InOut ? 0xFF : 0x00;
    PORTA_ANLG_SEL = AnlgDgtl ? 0xFF : 0x00;    
}

void setnBusyLine(int high_low)
{
    // nBusy stands for !Busy
    // !Busy is the line connected to the ASL1000
    // That will let it know when this uC chip is busy
    // or not.
    // This line is bit0 of PortA
    if(high_low)
    {
        PORTA |= 0x01;
    }
    else
    {
        PORTA &= 0xFE;
    }
    
}

void switchI2CMux(int asl_or_dut)
{
    // This chip needs to act as a SLAVE to the ASL1000
    // and a MASTER to the DUT. Because the uC only
    // has one I2C module, the SDA and SCL lines need
    // to be muxed when switching modes as it is
    // communicating between the ASL or the DUT.
    // this line is bit1 of PortA
    if(asl_or_dut)
    {
        PORTA |= 0x02;
    }
    else
    {
        PORTA &= 0xFD;
    }
}

void initI2CMaster()
{
    uC_i2c_mode = MICRO_I2C_MASTER_MODE;
    I2C2_Initialize();
     
}

void initI2CSlave()
{
    uC_i2c_mode = MICRO_I2C_SLAVE_MODE;
    I2C1_Initialize();
    
}

void MICRO_Initialize()
{
    uC_i2c_mode = MICRO_I2C_SLAVE_MODE;
    initPortA(OUTPUT, DIGITAL);
    setnBusyLine(SET_HIGH);
    switchI2CMux(SET_ASL);
    initI2CSlave(); // init slave 
}

uint8_t calcChecksum(uint8_t data[], int size)
{
    uint8_t temp_checksum = 0;
    for(int i = 0; i < size; i++)
    {
        temp_checksum += data[i];
    }
    temp_checksum = (~temp_checksum) + 1;
    return temp_checksum;
}

void clearArray(uint8_t arr[], int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}