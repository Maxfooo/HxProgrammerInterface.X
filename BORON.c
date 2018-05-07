
#include "BORON.h"

// ===== TESTS =====
uint8_t BORON_test_reset()
{
    return 0;
}

uint8_t BORON_test_wrRdRAM()
{
    uint8_t ramAddrHigh = 0x00;
    uint8_t ramAddrLow = 0x30;
    uint8_t ramData = 0xAA;
    
    // dws = boron write status
    // drs = boron read status
    uint8_t wr_dws = 0;
    uint8_t rd_dws = 0;
    uint8_t rd_drs = 0;
    
    uint8_t ramWrMessage[6];
    ramWrMessage[0] = BORON_WRITE_RAM;
    ramWrMessage[1] = ramAddrLow;
    ramWrMessage[2] = ramAddrHigh;
    ramWrMessage[3] = 1;
    ramWrMessage[4] = ramData;
    ramWrMessage[5] = 0; // initialize, then replace with checksum
    ramWrMessage[5] = calcChecksum(ramWrMessage, 6);
    
    uint8_t ramRdMessage[5]; 
    ramRdMessage[0] = BORON_READ_RAM;
    ramRdMessage[1] = ramAddrLow;
    ramRdMessage[2] = ramAddrHigh;
    ramRdMessage[3] = 1;
    ramRdMessage[4] = 0;
    ramRdMessage[4] = calcChecksum(ramRdMessage, 5);
    
    uint8_t ramReadback[3];
    
    wr_dws = writeI2C(ramWrMessage, 6, 0x5A);
    //rd_dws = writeI2C(ramRdMessage, 5, 0x5A);
    rd_drs = readI2C(ramReadback, 3, 0x5A);
    return rd_drs;
    
    /*
     * 
    uint8_t readbackPassFail;
    
    // do write/read error checking
    if(wr_dws != I2C_COMM_SUCCESS)
    {
        readbackPassFail = ASL_RETURN_ERR_BORON_COMM;
    }
    else if(rd_dws != I2C_COMM_SUCCESS)
    {
        readbackPassFail = ASL_RETURN_ERR_BORON_COMM;
    }
    else if(rd_drs != I2C_COMM_SUCCESS)
    {
        readbackPassFail = ASL_RETURN_ERR_BORON_COMM;
    }
    else
    {
        // compare sent value to return value
        // need to do further checks here for error returns from boron
        readbackPassFail = ramReadback[1] == ramData ? ASL_RETURN_PASS : ASL_RETURN_FAIL;
    }
    
    return readbackPassFail;
    */
}

// just sending back pass if the ASL1000 was able to talk to this micro
uint8_t uC_test_debug()
{
    return ASL_RETURN_PASS;
}

// send and receive data to and from an arduino via i2c
uint8_t ARDUINO_test_debug()
{
    
    /* ***************************************
    * Test I2C Master transmit
    * ***************************************/
    if (boronDebugTxData[0] < 0x30 || boronDebugTxData[0] >= 0x39)
    {
        boronDebugTxData[0] = 0x30;
    }
    writeI2C(boronDebugTxData, 1, 0x2A);
    //__delay_ms(500);
    __delay_ms(50);
    /* ***************************************
    * Test I2C Master request
    * ***************************************/
    readI2C(boronDebugTxData, 1, 0x2A);
    //__delay_ms(500);
    __delay_ms(50);
    return boronDebugTxData[0];
}


// ===== CONFIGURATIONS =====
uint8_t BORON_cnfg_oscDiv()
{
    return 0;
}

uint8_t BORON_cnfg_OTP()
{
    return 0;
}

uint8_t BORON_cnfg_debug()
{
    // just sending back pass if the ASL1000 was able to talk to this micro
    return ASL_RETURN_PASS;
}



