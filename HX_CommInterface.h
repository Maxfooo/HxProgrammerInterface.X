/* 
 * File:   HX_CommInterface.h
 * Author: maxr
 *
 * Created on June 30, 2017, 11:56 AM
 */

#ifndef HX_COMMINTERFACE_H
#define	HX_COMMINTERFACE_H

#include "mcc_generated_files/eusart1.h"
#include "HX_CommStd.h"
#include "HX_DeviceProperties.h"
#include "mcc_generated_files/tmr0.h"
#include "HX_SPP_CARBON.h"



/**
 * @Name
 *  HX_Initialize
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Initialize variables used in communicating with the PC host.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void HX_Initialize();

/**
 * @Name
 *  hxISRHandle
 * 
 * @Param
 *  data - byte read in from the UART receive ISR
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Read in the bytes from the UART receive ISR and determine what to do with
 *  it. Data byte should follow the incoming packet structure:
 *  PC_CMD, PktSize, [Data], Checksum
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxISRHandle(uint8_t data);

/**
 * @Name
 *  hxCmdFilter
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Based on the value of hx_pcCmd (the command sent from the PC host), initiate
 *  tasks and set up the return packet, appropriately.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxCmdFilter();

/**
 * @Name
 *  hxSetupPacket1
 * 
 * @Param
 *  pc_cmd - The command of which the PC host sent to the micro
 *  data   - The data byte that should be sent back to the PC host
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Fill in the packet details and data for a quick response to the PC host. 
 *  This is usually just a status report or an ack/nack.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 *  The output buffer is flushed before the packet is setup.
 * 
 * 
 **/
void hxSetupPacket1(uint8_t pc_cmd, uint8_t data);

/**
 * @Name
 *  hxSetupPacket2
 * 
 * @Param
 *  pc_cmd - The command of which the PC host sent to the micro
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Fill in the remaining packet details of pc_cmd, number of bytes, and checksum.
 *  Data should have already been placed into the output buffer in the code
 *  being run in main (if any).
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * The output buffer is NOT flushed before the packet is setup.
 * 
 * 
 **/
void hxSetupPacket2(uint8_t pc_cmd);

/**
 * @Name
 *  hxHandleTxUART
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Write the data in the "hx_outData" buffer to the UART TX buffer and transmit.
 *  Only write number of bytes from the buffer up to the index "hx_outDataIndex".
 *  The function is called in main and checks for the flag "hx_txPacketFlag".
 *  After all the data is written, the flag "hx_txPacketFlag" is cleared.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxHandleTxUART();

/**
 * @Name
 *  hxHandleTmr0
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  If the timer interrupt goes off, it calls this handle function.
 *  This function will flush both buffers, set the hx_status to
 *  the appropriate timeout error status and setup a packet for reading.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxHandleTmr0();

/**
 * @Name
 *  hxStartTimer
 * 
 * @Param
 *  uint16_t - timer0's timeout value
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Initially set the timer's timeout value, then start timer0.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxStartTimer(uint16_t timerVal);

/**
 * @Name
 *  hxStopTimer
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Stop timer0 and then set the timeout value to the default 
 *  value of HX_TIMER_PC_RST_VAL
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxStopTimer();

/**
 * @Name
 *  hxFlushInputBuffer
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Set all indices of the array "hx_inData" to zero and reset the global index 
 *  variable "hx_inDataIndex" to 0
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxFlushInputBuffer();

/**
 * @Name
 *  hxFlushOutputBuffer
 * 
 * @Param
 *  
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Set all indices of the array "hx_outData" to zero and reset the global index
 *  variable "hx_outDataIndex" to "HX_DATA_OUT_ORIGIN" (== 2)
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxFlushOutputBuffer();

/**
 * @Name
 *  
 * 
 * @Param
 *  
 * 
 * @Return
 *  
 * 
 * @Description
 *  
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxFlushMainDataBuffer();

/**
 * @Name
 *  hxCalcChecksum
 * 
 * @Param
 *  data - Array of data bytes
 *  size - Size of the array of data bytes
 * 
 * @Return
 *  uint8_t - value of checksum
 * 
 * @Description
 *  Sum all data bytes in the array and take the two's compliment of the result. 
 *  Return this value as the checksum.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
uint8_t hxCalcChecksum(uint8_t data [], uint8_t size);

/**
 * @Name
 *  hxClearArray
 * 
 * @Param
 *  arr  - Array with arbitrary data in it
 *  size - Size of the array
 * 
 * @Return
 *  void
 * 
 * @Description
 *  Set all the data in the array (up to size) to 0.
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/
void hxClearArray(uint8_t arr [], uint8_t size);




/**
 * @Name
 *  
 * 
 * @Param
 *  
 * 
 * @Return
 *  
 * 
 * @Description
 *  
 * 
 * @Notes
 *  Must have included "HX_CommStd.h"
 * 
 * 
 **/

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* HX_COMMINTERFACE_H */

