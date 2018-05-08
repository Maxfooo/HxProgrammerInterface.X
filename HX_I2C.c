

#include "HX_I2C.h"

// STRUCTURE
// MODE : [0xhh
// SLAVE_ADDR : 0xhh,
// NUM_TX : 0xhh,
// NUM_RX : 0xhh,
// {TXB0, TXB1, ... , TXBN},
// CHECKSUM : 0xhh]


void run_I2C(uint8_t hx_inData[], uint8_t size)
{
    uint8_t mode = 0;
    uint8_t numTx = 0;
    uint8_t numRx = 0;
    uint16_t slaveAddr = 0;
    uint8_t checksum = 0;
    uint16_t data_index = 0;
    uint8_t calc_checksum   = 0;
    uint8_t rx_index = 0;
    
    
    uint8_t * debugArr;
    
    uint8_t i = 0;
    
    txRxPkt txPkt;
    txRxPkt rxPkt;
    txRxPkt* stream[2];
    stream[0] = &txPkt;
    stream[1] = &rxPkt;
    txPkt.txOrRx = TX_PACKET_TYPE;
    rxPkt.txOrRx = RX_PACKET_TYPE;
    
    
    mode = hx_inData[data_index++];
    slaveAddr = hx_inData[data_index++];
    numTx = hx_inData[data_index++];
    numRx = hx_inData[data_index++];
    
    txPkt.slaveAddr = slaveAddr;
    txPkt.numOfBytes = numTx;
    rxPkt.slaveAddr = slaveAddr;
    rxPkt.numOfBytes = numRx;
    
    if (mode < HX_I2C_READ_MODE)
    {
        if (numTx < 1)
        {
            hx_state = HX_UC_ERROR_INTERNAL_PKT;
            return;
        }
        
        if (numTx > TX_ARR_SIZE)
        {
            hx_state = HX_UC_ERROR_INTERNAL_PKT;
            return;
        }
        
        //for (i = 0; i <= numTx; i++)
        for (i = 0; i < numTx; i++)
        {
//            if (i == numTx)
//            {
//                txPkt.data[i] = calcChecksum(txPkt.data, numTx);
//                txPkt.numOfBytes++;
//            }
            txPkt.data[i] = hx_inData[data_index++];
        }
        
        
        
        checksum = hx_inData[data_index++];
        calc_checksum = calcChecksum(hx_inData, data_index-1);
        if (calc_checksum != checksum)
        {
            hx_state = HX_UC_ERROR_PC_CHECKSUM;
            return;
        }
        
    }
    else 
    {
        checksum = hx_inData[data_index++];
        calc_checksum = calcChecksum(hx_inData, data_index-1);
        if (calc_checksum != checksum)
        {
            hx_state = HX_UC_ERROR_PC_CHECKSUM;
            return;
        }
    }
    
    
    if (mode == HX_I2C_WRITE_MODE)
    {
        streamI2C(&stream[0], 1); // send exactly the txPkt
        
        if (*txPkt.status == I2C1_MESSAGE_FAIL)
        {
            hx_mainData[0] = HX_UC_ERROR_DUT_COMM;
            hx_mainDataIndex++;
            hx_state = HX_UC_COMPLETE;
            return;
        }
        
        hx_mainData[0] = HX_DUT_COMPLETE;
        hx_mainDataIndex++;
        hx_state = HX_UC_COMPLETE;
    }
    else if (mode == HX_I2C_WR_RD_MODE)
    {
        streamI2C(stream, 2);
        if (*txPkt.status == I2C1_MESSAGE_FAIL || *rxPkt.status == I2C1_MESSAGE_FAIL)
        {
            hx_mainData[0] = HX_UC_ERROR_DUT_COMM;
            hx_mainDataIndex++;
            hx_state = HX_UC_COMPLETE;
            return;
        }
        
        rx_index = 0;
        hx_mainData[rx_index++] = HX_DUT_COMPLETE;
        for (uint8_t i = 0; i < numRx; i++)
        {
            hx_mainData[rx_index++] = rxPkt.data[i];
        }
        
        hx_mainDataIndex = rx_index;
        hx_state = HX_UC_COMPLETE;
    }
    else if (mode == HX_I2C_READ_MODE)
    {
        streamI2C(&stream[1], 1); // send exactly the rxPkt
        if (*rxPkt.status == I2C1_MESSAGE_FAIL)
        {
            hx_mainData[0] = HX_UC_ERROR_DUT_COMM;
            hx_mainDataIndex++;
            hx_state = HX_UC_COMPLETE;
            return;
        }
        
        rx_index = 0;
        hx_mainData[rx_index++] = HX_DUT_COMPLETE;
        for (uint8_t i = 0; i < numRx; i++)
        {
            hx_mainData[rx_index++] = rxPkt.data[i];
        }
        
        hx_mainDataIndex = rx_index;
        hx_state = HX_UC_COMPLETE;
    }
    else if (mode == HX_I2C_DEBUG_MODE0)
    {
        hx_mainData[0] = HX_DUT_COMPLETE;
        hx_mainData[1] = 0xAA;
        hx_mainDataIndex = 2;
        hx_state = HX_UC_COMPLETE;
    }
    else if (mode == HX_I2C_DEBUG_MODE1)
    {
        *debugArr = 0xBB;
        writeI2C(debugArr, 1, 0x5a);
        hx_mainData[0] = HX_DUT_COMPLETE;
        hx_mainData[1] = 0xBB;
        hx_mainDataIndex = 2;
        hx_state = HX_UC_COMPLETE;
    }
    else if (mode == HX_I2C_DEBUG_MODE2)
    {
        readI2C(&hx_mainData[1], 1, 0x5a);
        hx_mainData[0] = HX_DUT_COMPLETE;
        hx_mainDataIndex = 2;
        hx_state = HX_UC_COMPLETE;
    }
    else
    {
        hx_state = HX_UC_ERROR_PC_CHECKSUM;
        return;
    }
}
