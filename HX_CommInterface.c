
#include "HX_CommInterface.h"


/*
 * FROM PC: PACKET STRUCTURE
 * 
 * PC_CMD, PktSize, [Data], Checksum
 * -PktSize does NOT include itself
 * -Checksum includes everything before it
 * 
 * TO PC: PACKET STRUCTURE
 * PKT1 = PC_CMD, NumOfData, Data, Checksum
 * PKT2 = PC_CMD, NumOfData, [Data, ...], Checksum
 * 
 */

void HX_Initialize()
{
    hx_state = HX_UC_COMPLETE;
    //LATA = HX_ENABLE;
    LATA = HX_ENABLE_TEST;
    hxFlushInputBuffer();
    hxFlushOutputBuffer();
    hxFlushMainDataBuffer();
}

void hxISRHandle(uint8_t data)
{
    static uint8_t currentByteCounter = 0;
    static uint8_t rxPktSizeFlag = 0;
    static uint8_t rxHxCmdFlag = 0;
    static uint8_t pktSize = 0;
    static uint8_t rxHxPktFlag = 0;
    
    hxStartTimer(HX_TIMER_PC_RST_VAL);
    
    if (0 == rxHxCmdFlag) // check first byte
    {
        hx_pcCmd = data;
        if (hx_pcCmd == HX_PC_DATA_WRITE)
        {
            hxFlushInputBuffer();
            rxHxPktFlag = 0; //First read byte will be command byte
        }
        else
        {
            rxHxPktFlag = 1; //Added in case a new routine is made
        }
        hx_checksum = data;
        rxHxCmdFlag = 1;
    }
    else if (0 == rxPktSizeFlag) // check second byte
    {
        pktSize = data;
        hx_checksum += data;
        rxPktSizeFlag = 1;
    }
    else if (currentByteCounter >= pktSize - 1) // check last byte
    {
        // validate checksum
        hx_checksum += data; // add sent checksum to hx_checksum
        if (hx_checksum == 0x00)
        {
            hx_pktState = HX_UC_ACK;
        }
        else
        {
            hx_pktState = HX_UC_ERROR_PC_CHECKSUM;
        }
        
        // reset
        rxHxCmdFlag = 0;
        rxPktSizeFlag = 0;
        currentByteCounter = 0;
        rxHxPktFlag = 0;
        
        // set and return
        hx_rxFullPktFlag = 1;
        
        // Stop Timer and reset to default value
        hxStopTimer();
        
        // filter command
        hxCmdFilter();
        
    }
    else // check intermediate bytes
    {
        if (0 == rxHxPktFlag)
        {
            hx_pktCmd = data;
            rxHxPktFlag = 1;
        }
        else
        {
            hx_inData[hx_inDataIndex++] = data;
        }
//        hx_inData[hx_inDataIndex++] = data;
        hx_checksum += data;
        currentByteCounter++;
    }
}

void hxCmdFilter()
{
    // Any timer set in this function continue until
    // either stopped in the "hxHandleTxUART" functions
    // or the ISR hits for a timeout.    
    
    if (hx_pktState == HX_UC_NACK) // packet error
    {
        hxSetupPacket1(hx_pcCmd, hx_pktState);
        hx_txPacketFlag = 1;
        return;
    }
    
    if (hx_pcCmd != HX_PC_INITIATE_MAIN_CODE)
    {
        hxStartTimer(HX_TIMER_PC_RST_VAL);
        
        switch (hx_pcCmd)
        {
            case HX_PC_DATA_WRITE:
                hxSetupPacket1(hx_pcCmd, hx_pktState);
                break;
                
            case HX_PC_STATUS_REQUEST:
                // status is updated globally
                // write status back
                hxSetupPacket1(hx_pcCmd, hx_state);
                break;
                
            case HX_PC_ACK_COMPLETE:
                hxSetupPacket2(HX_PC_ACK_COMPLETE);
                break;
                
            case HX_PC_FLUSH_INPUT_BUFFER:
                hxFlushInputBuffer();
                hxSetupPacket1(hx_pcCmd, HX_UC_ACK);
                break;
                
            case HX_PC_FLUSH_OUTPUT_BUFFER:
                // hxSetPacket1() already flushes the output buffer
                hxSetupPacket1(hx_pcCmd, HX_UC_ACK);
                hxFlushMainDataBuffer();
                break;
                
            case HX_PC_REQUEST_CHIP_ID:
//                hxSetupPacket1(hx_pcCmd, HX_CHIP_ID);
//                hxFlushMainDataBuffer();
                hxFlushMainDataBuffer();
                hxFlushOutputBuffer();
                hx_mainData[hx_mainDataIndex++] = HX_CHIP_ID;
                hx_mainData[hx_mainDataIndex++] = HX_CHIP_REVISION;
                hxSetupPacket2(hx_pcCmd);
                hxFlushMainDataBuffer();
                break;
                
            case HX_PC_REQUEST_INPUT_BUFFER:
                hxFlushOutputBuffer();
                hxFlushMainDataBuffer();
                for (uint8_t i = 0; i < hx_inDataIndex; i++)
                {
                    hx_mainData[hx_mainDataIndex++] = hx_inData[i];
                }
                hxSetupPacket2(hx_pcCmd);
                hxFlushMainDataBuffer();
                break;
            
            case HX_PC_REQUEST_DEBUG_VAR:
                hxSetupPacket1(hx_pcCmd, hx_debugVar);
                break;
                
            case HX_PC_REQUEST_PREVIOUS_PKT:
                // do nothing, packet is already setup
                break;
                
            default:
                hxSetupPacket1(hx_pcCmd, HX_UC_CMD_UNRECOGNIZED);
                break;
        }
    }
    else // HX_PC_INITIATE_MAIN_CODE
    {
        hxFlushMainDataBuffer();
        // Running the code in main allows for a 200ms timeout limit
        hxStartTimer(HX_TIMER_MAIN_CODE_RST_VAL);
        
        if (hx_rxFullPktFlag)
        {
            hxSetupPacket1(HX_PC_INITIATE_MAIN_CODE, HX_UC_ACK);
            hx_txPacketFlag = 1;
            hx_startMain = 1;
            hx_state = HX_UC_BUSY;
        }
        else
        {
            hx_startMain = 0;
            hxSetupPacket1(HX_PC_INITIATE_MAIN_CODE, HX_UC_NACK);
            hx_state = HX_UC_ERROR_UNKNOWN;
        }
    }
    hx_txPacketFlag = 1;
}

void hxSetupPacket1(uint8_t pc_cmd, uint8_t data)
{
    hxFlushOutputBuffer();
    hx_outData[HX_PC_CMD_INDEX] = pc_cmd;
    hx_outData[HX_NUM_BYTES_INDEX] = HX_PACKET_1_NUM_BYTES;
    hx_outData[hx_outDataIndex++] = data;
    hx_outData[hx_outDataIndex++] = hxCalcChecksum(hx_outData, hx_outDataIndex);
}

void hxSetupPacket2(uint8_t pc_cmd)
{
    hx_outData[HX_PC_CMD_INDEX] = pc_cmd;
    for (uint8_t i = 0; i < hx_mainDataIndex; i++)
    {
        hx_outData[hx_outDataIndex++] = hx_mainData[i];
    }
    // remove the offset (PC_CMD, NUM_BYTES) and add 1 for checksum
    hx_outData[HX_NUM_BYTES_INDEX] = hx_outDataIndex - HX_DATA_OUT_ORIGIN + 1;
    hx_outData[hx_outDataIndex++] = hxCalcChecksum(hx_outData, hx_outDataIndex);
    
}

void hxHandleTxUART()
{
    if (hx_txPacketFlag)
    {
        LED_COMM_TX  = 1;
        for (int i = 0; i < hx_outDataIndex; i++)
        {
            EUSART1_Write(hx_outData[i]);
        }
        while (PIE3bits.TX1IE == 1);
        LED_COMM_TX = 0;
        
        hxStopTimer();
        hx_txPacketFlag = 0;
        hxFlushOutputBuffer();
    }
    
}

void hxHandleTmr0()
{
    hxFlushOutputBuffer();
    hxFlushInputBuffer();
//    hxFlushMainDataBuffer();
    // Micro running main code or call and response command?
    // set state appropriately
    hx_state = hx_state == HX_UC_BUSY ? HX_UC_ERROR_MAIN_TIMEOUT:
                                        HX_UC_ERROR_PC_TIMEOUT;
    hxSetupPacket1(HX_PC_STATUS_REQUEST, hx_state);
    
    hx_startMain = 0;
    hx_txPacketFlag = 1;
    hx_abort = 1;
}

void hxStartTimer(uint16_t timerVal)
{
    TMR0_Write16bitTimer(timerVal);
    TMR0_StartTimer();
}

void hxStopTimer()
{
    TMR0_StopTimer();
    TMR0_Write16bitTimer(HX_TIMER_PC_RST_VAL);
}

void hxFlushInputBuffer()
{
    hxClearArray(hx_inData, HX_IN_DATA_SIZE);
    hx_inDataIndex = 0;
    hx_pktCmd = 0;
}

void hxFlushOutputBuffer()
{
    hxClearArray(hx_outData, HX_OUT_DATA_SIZE);
    hx_outDataIndex = HX_DATA_OUT_ORIGIN;
}

void hxFlushMainDataBuffer()
{
    hxClearArray(hx_mainData, HX_MAIN_DATA_SIZE);
    hx_mainDataIndex = 0;
}

uint8_t hxCalcChecksum(uint8_t data [], uint8_t size)
{
    uint8_t temp_checksum = 0;
    for(uint8_t i = 0; i < size; i++)
    {
        temp_checksum += data[i];
    }
    temp_checksum = (~temp_checksum) + 1;
    return temp_checksum;
}

void hxClearArray(uint8_t arr [], uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}