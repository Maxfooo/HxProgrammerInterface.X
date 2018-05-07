
#include <xc.h>
#include "EEPROM.h"



EEPROM_STATUS EEPROM_WriteByte(uint16_t bAdd, uint8_t bData)
{
    if (bAdd > EEPROM_MAX_ADDR_RANGE)
    {
        return EEPROM_OUT_OF_BOUNDS;
    }
    
    
    uint8_t GIEBitValue = INTCONbits.GIE;
    
    NVMADRL = (bAdd & 0xFF);
    NVMADRH = (bAdd >> 4) & 0xFF;
    
    NVMDAT = bData;
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.WREN = 1;
    INTCONbits.GIE = 0;     // Disable interrupts
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    // Wait for write to complete
    while (NVMCON1bits.WR)
    {
    }

    NVMCON1bits.WREN = 0;
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable
    return EEPROM_WRITE_SUCCESS;
}


EEPROM_STATUS EEPROM_ReadByte(uint16_t bAdd, uint8_t *bData)
{
    
    if (bAdd > EEPROM_MAX_ADDR_RANGE)
    {
        return EEPROM_OUT_OF_BOUNDS;
    }
    
    NVMADRL = (bAdd & 0xFF);
    NVMADRH = (bAdd >> 4) & 0xFF;
    
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();
    
    *bData = NVMDAT;
    return EEPROM_READ_SUCCESS;
}

EEPROM_STATUS EEPROM_EraseBlock(uint16_t bAddStart, uint16_t bAddEnd)
{  
    
    if (bAddStart > bAddEnd)
    {
        INTCONbits.GIE = 1; // re-enable interrupts
        return EEPROM_ERASE_FAIL;
    }
    else if(bAddEnd > EEPROM_MAX_ADDR_RANGE)
    {
        return EEPROM_OUT_OF_BOUNDS;
    }
    
    INTCONbits.GIE = 0; // Disable interrupts
    
    uint16_t eraseAddr = bAddStart;
    while(eraseAddr <= bAddEnd)
    {
        EEPROM_WriteByte(eraseAddr++, EEPROM_ERASE_BYTE);
    }
    
    INTCONbits.GIE = 1; // re-enable interrupts
    return EEPROM_ERASE_SUCCESS;
}

EEPROM_STATUS EEPROM_Initialize()
{
    // OBJECTIVE
    // 1) The first 2 bytes stored in the eeprom is going to be the
    // eeprom current address variable "eeprom_currAddr" which represents
    // the last address of written to.
    // 2) Readback the value stored in the first 2 bytes of the eeprom
    // and shove that into the global variable: "eeprom_currAddr"
    
    uint16_t org = 0x0000;
    uint8_t bData = 0x00;
    
    EEPROM_STATUS eepStat = EEPROM_ReadByte(org, &bData);
    if (eepStat != EEPROM_READ_SUCCESS)
    {
        return eepStat;
    }
    else
    {
        eeprom_currAddr |= (bData & 0x00FF) << 8;
    }
    
            
            
    eepStat = EEPROM_ReadByte(org+1, &bData);
    if (eepStat != EEPROM_READ_SUCCESS)
    {
        return eepStat;
    }
    else
    {
        eeprom_currAddr |= (bData & 0x00FF);
    }
    
    // eeprom_currAddr is pulled in as data from the eeprom.
    // if the value is larger than the max address range, then it is
    // assumed that the address has never been written in.
    if (eeprom_currAddr > EEPROM_MAX_ADDR_RANGE)
    {
        eeprom_currAddr = 0x0000;
    }
    
    return EEPROM_READ_SUCCESS;
}

EEPROM_STATUS EEPROM_Update()
{
    
    if (eeprom_currAddr > EEPROM_MAX_ADDR_RANGE)
    {
        return EEPROM_OUT_OF_BOUNDS;
    }
    
    uint16_t org = 0x0000;
    
    EEPROM_STATUS eepStat = EEPROM_EraseBlock(org, org+1);
    if (eepStat != EEPROM_ERASE_SUCCESS)
    {
        return eepStat;
    }
    
    
    eepStat = EEPROM_WriteByte(org, (eeprom_currAddr & 0xFF));
    if (eepStat != EEPROM_WRITE_SUCCESS)
    {
        return eepStat;
    }
    
    
    eepStat = EEPROM_WriteByte(org+1, ((eeprom_currAddr >> 8) & 0xFF));
    if (eepStat != EEPROM_WRITE_SUCCESS)
    {
        return eepStat;
    }
     
    
    return EEPROM_WRITE_SUCCESS;
}

void MEMORY_Tasks( void )
{
    /* TODO : Add interrupt handling code */
    PIR7bits.NVMIF = 0;
}



