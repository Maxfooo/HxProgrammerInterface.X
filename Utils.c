
#include "Utils.h"


uint8_t writeI2C(uint8_t txData[], uint8_t size, uint16_t slaveAddr)
{
    I2C1_MESSAGE_STATUS status = I2C1_MESSAGE_PENDING;
    uint16_t timeOut;
    
    timeOut = 0;
    
    while(status != I2C1_MESSAGE_FAIL)
    {
        I2C1_MasterWrite(   txData,
                            size,
                            slaveAddr,
                            &status);

        // wait for the message to be sent or status has changed.
        while(status == I2C1_MESSAGE_PENDING);

        if (status == I2C1_MESSAGE_COMPLETE)
        {
            return I2C_COMM_SUCCESS;
        }

        // if status is  I2C1_MESSAGE_ADDRESS_NO_ACK, or I2C1_DATA_NO_ACK,
        // The device may be busy and needs more time for the last
        // write so we can retry writing the data, this is why we
        // use a while loop here

        // check for max retry and skip this byte
        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
            return I2C_COMM_TIMEOUT;
        else
            timeOut++;
    }
    return I2C_COMM_FAILURE;
    
    
}


uint8_t readI2C(uint8_t rxData[], uint8_t numOfRx, uint16_t slaveAddr)
{
    I2C1_MESSAGE_STATUS status;
    uint16_t    timeOut;
    while(status != I2C1_MESSAGE_FAIL)
    {
        
        I2C1_MasterRead(    rxData,
                            numOfRx,
                            slaveAddr,
                            &status);

        // wait for the message to be sent or status has changed.
        while(status == I2C1_MESSAGE_PENDING);

        if (status == I2C1_MESSAGE_COMPLETE)
            return I2C_COMM_SUCCESS;

        // if status is  I2C1_MESSAGE_ADDRESS_NO_ACK,
        //               or I2C1_DATA_NO_ACK,
        // The device may be busy and needs more time for the last
        // write so we can retry writing the data, this is why we
        // use a while loop here

        // check for max retry and skip this byte
        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
            return I2C_COMM_TIMEOUT;
        else
            timeOut++;
    }
    return I2C_COMM_FAILURE;
}

uint8_t streamBoronComm(uint8_t boron_comm_type, txRxPkt** pktArr, uint8_t pktSize)
{
    if (boron_comm_type == BORON_COMM_I2C)
    {
        return streamI2C(pktArr, pktSize);
    }
    else
    {
        return streamSW(pktArr, pktSize);
    }
}

uint8_t streamSW(txRxPkt** pktArr, uint8_t pktSize)
{
    
    
    return 0;
}

uint8_t streamI2C(txRxPkt** pktArr, uint8_t pktSize)
{
    I2C1_TRANSACTION_REQUEST_BLOCK trBlock[I2C_TR_BLOCK_SIZE];
    I2C1_MESSAGE_STATUS status = I2C1_MESSAGE_PENDING;
    uint8_t i = 0;
    
    for (i = 0; i < pktSize; i++)
    {
        if (pktArr[i]->txOrRx == TX_PACKET_TYPE)
        {
            I2C1_MasterWriteTRBBuild(&trBlock[i], pktArr[i]->data, pktArr[i]->numOfBytes, pktArr[i]->slaveAddr);
        }
        else
        {
            I2C1_MasterReadTRBBuild(&trBlock[i], pktArr[i]->data, pktArr[i]->numOfBytes, pktArr[i]->slaveAddr);
        }
    }
    
    while(status != I2C1_MESSAGE_FAIL)
    {
        // now send the transactions
        I2C1_MasterTRBInsert(pktSize, trBlock, &status);
        
        // wait for the message to be sent or status has changed.
        while(status == I2C1_MESSAGE_PENDING);
        
        if (status == I2C1_MESSAGE_COMPLETE)
        {
            break;
        }
        else
        {
            break;
        }
    }
    return status;
}

/*
uint8_t streamI2C(txRxPkt** pktArr, uint8_t pktSize)
{
    I2C1_TRANSACTION_REQUEST_BLOCK trBlock[pktSize];
    I2C1_MESSAGE_STATUS status;
    uint8_t i = 0;
    
    while ((status != I2C1_MESSAGE_FAIL) && (pktSize > 0) && (i < pktSize))
    {
        
        if (pktArr[i]->txOrRx == TX_PACKET_TYPE)
        {
            I2C1_MasterWriteTRBBuild(&trBlock, pktArr[i]->data, pktArr[i]->numOfBytes, pktArr[i]->slaveAddr);
            I2C1_MasterTRBInsert(1, &trBlock, pktArr[i]->status);
        }
        else
        {
            I2C1_MasterReadTRBBuild(&trBlock, pktArr[i]->data, pktArr[i]->numOfBytes, pktArr[i]->slaveAddr);
            I2C1_MasterTRBInsert(1, &trBlock, pktArr[i]->status);
        }
        status = *pktArr[i]->status;
        
        // wait for the message to be sent or status has changed.
        while(status == I2C1_MESSAGE_PENDING)
        {
            status = *pktArr[i]->status;
        };
        
        i++;
        
        if (status == I2C1_MESSAGE_COMPLETE)
        {
//            trBlock.address = 0;
//            trBlock.length = 0;
//            trBlock.pbuffer = NULL;
            continue;
        }
        else
        {
            break;
        }
    }
    return status;
}
*/

uint8_t calcChecksum(uint8_t data [], uint8_t size)
{
    uint8_t temp_checksum = 0;
    for(uint8_t i = 0; i < size; i++)
    {
        temp_checksum += data[i];
    }
    temp_checksum = (temp_checksum ^ 0xFF) + 1;
    return temp_checksum;
}

void clearArray(uint8_t arr [], uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

uint8_t copyUint8_tArr(uint8_t* toArr, uint8_t* fromArr, uint8_t size)
{
    uint8_t retCount = 0;
    
    if (size > sizeof(toArr) || size > sizeof(fromArr))
    {
        return retCount;
    }
    
    for (retCount; retCount < size; retCount++)
    {
        *toArr++ = *fromArr++;
    }
    return retCount;
}