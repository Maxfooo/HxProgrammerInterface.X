
#include "DUT.h"
//----- #define -----


// ===== STANDARD FUNCTIONS =====

DUT_TEST_STATUS DUT_writeBytes(uint8_t data[], int size)
{
    uint8_t timeOut;
    I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;
    DUT_TEST_STATUS dws;
    
    timeOut = 0;
    while(status != I2C2_MESSAGE_FAIL)
    {
        I2C2_MasterWrite(data, size, DUT_SLAVE_ADDR, &status);
        while(status == I2C2_MESSAGE_PENDING);

        if (status == I2C2_MESSAGE_COMPLETE)
        {
            dws = DUT_WRITE_SUCCESSFUL;
            break;
        }

        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
        {
            dws = DUT_WRITE_TIMEOUT;
            break;
        }
        else
        {
            timeOut++;
        }
    }
    
    if (status == I2C2_MESSAGE_FAIL)
    {
        // NEED TO CAPTURE ERROR AND RETURN IT TO ASL1000
        // OR WRITE TO EXTERNAL FILE FOR DEBUGGING.
        
        
        dws = DUT_WRITE_FAILED;
    }
    
    return dws;
       
}

DUT_TEST_STATUS DUT_readBytes(uint8_t *pData, uint16_t numOfBytes)
{
    I2C2_MESSAGE_STATUS status;
    I2C2_TRANSACTION_REQUEST_BLOCK readTRB[1];
    uint16_t timeOut;
    DUT_TEST_STATUS drs;
    
    status = I2C2_MESSAGE_PENDING;
    
    // requesting a read from the DUT
    I2C2_MasterReadTRBBuild(&readTRB[0], pData, numOfBytes, DUT_SLAVE_ADDR);
    
    timeOut = 0;
    while(status != I2C2_MESSAGE_FAIL)
    {
        I2C2_MasterTRBInsert(1, readTRB, &status);
        
        while(status == I2C2_MESSAGE_PENDING);
        
        if(status == I2C2_MESSAGE_COMPLETE)
        {
            drs = DUT_READ_SUCCESSFUL;
            break;
        }
        
        if(timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
        {
            drs = DUT_READ_TIMEOUT;
            break;
        }
        else
        {
            timeOut++;
        }
    }
    
    return drs;
    
    
}




// ===== TESTS =====
uint8_t DUT_test_reset()
{
    return 0;
}

uint8_t DUT_test_wrRdRAM()
{
    uint8_t *pData; // pointer to mem loc where data will be stored when reading
    uint8_t ramAddrHigh = 0x00;
    uint8_t ramAddrLow = 0x30;
    uint8_t ramData = 0xAA;

    DUT_TEST_STATUS wr_dws;
    DUT_TEST_STATUS rd_dws;
    DUT_TEST_STATUS rd_drs;
    
    uint8_t ramWrMessage[6];
    ramWrMessage[0] = DUT_WRITE_RAM;
    ramWrMessage[1] = ramAddrLow;
    ramWrMessage[2] = ramAddrHigh;
    ramWrMessage[3] = 1;
    ramWrMessage[4] = ramData;
    ramWrMessage[5] = 0; // initialize, then replace with checksum
    ramWrMessage[5] = calcChecksum(ramWrMessage, 6);
    
    uint8_t ramRdMessage[5]; 
    ramRdMessage[0] = DUT_READ_RAM;
    ramRdMessage[1] = ramAddrLow;
    ramRdMessage[2] = ramAddrHigh;
    ramRdMessage[3] = 1;
    ramRdMessage[4] = 0;
    ramRdMessage[4] = calcChecksum(ramRdMessage, 5);
    
    wr_dws = DUT_writeBytes(ramWrMessage, 6);
    rd_dws = DUT_writeBytes(ramRdMessage, 5);
    rd_drs = DUT_readBytes(pData, 3);
    
    uint8_t ramReadback[3];
    ramReadback[0] = *pData;
    ramReadback[1] = *(pData+1);
    ramReadback[2] = *(pData+2);
    
    uint8_t readbackPassFail;
    
    // do write/read error checking
    if(wr_dws != DUT_WRITE_SUCCESSFUL)
    {
        readbackPassFail = ASL_RETURN_ERR_DUT_COMM;
    }
    else if(rd_dws != DUT_WRITE_SUCCESSFUL)
    {
        readbackPassFail = ASL_RETURN_ERR_DUT_COMM;
    }
    else if(rd_drs != DUT_READ_SUCCESSFUL)
    {
        readbackPassFail = ASL_RETURN_ERR_DUT_COMM;
    }
    else
    {
        // compare sent value to return value
        readbackPassFail = ramReadback[1] == ramData ? ASL_RETURN_PASS : ASL_RETURN_FAIL;
    }
    
    return readbackPassFail;
}


uint8_t uC_test_debug()
{
    // just sending back pass if the ASL1000 was able to talk to this micro
    return ASL_RETURN_PASS;
}


// ===== CONFIGURATIONS =====
uint8_t DUT_cnfg_oscDiv()
{
    return 0;
}

uint8_t DUT_cnfg_OTP()
{
    return 0;
}




