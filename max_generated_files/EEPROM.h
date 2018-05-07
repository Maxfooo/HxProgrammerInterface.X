/**
  MEMORY Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    memory.h

  @Summary
    This is the generated header file for the MEMORY driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for MEMORY.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F47K40
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*******************************************************************************/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _EEPROM_H
#define _EEPROM_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

const uint8_t EEPROM_ERASE_BYTE = 0xFF;
const uint16_t EEPROM_MAX_ADDR_RANGE = 0x03FF;
uint16_t eeprom_currAddr = 0x0000;

typedef enum
{
    EEPROM_WRITE_SUCCESS,
    EEPROM_WRITE_FAIL,

    EEPROM_READ_SUCCESS,
    EEPROM_READ_FAIL,

    EEPROM_ERASE_SUCCESS,
    EEPROM_ERASE_FAIL,
            
    EEPROM_OUT_OF_BOUNDS
}EEPROM_STATUS;

/**
  @Summary
    Writes a data byte to Data EEPROM

  @Description
    This routine writes a data byte to given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location to which data to be written
    bData - Data to be written to Data EEPROM location

  @Returns
    EEPROM_STATUS

  @Example
    <code>
    uint16_t dataeeAddr = 0x1010;
    uint8_t dataeeData = 0x55;
    EEPROM_STATUS eepStat;
    eepStat = DATAEE_WriteByte(dataeeAddr, dataeeData);
    </code>
*/
EEPROM_STATUS DATAEE_WriteByte(uint16_t bAdd, uint8_t bData);

/**
  @Summary
    Reads a data byte from Data EEPROM

  @Description
    This routine reads a data byte from given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location from which data has to be read

  @Returns
    Data byte read from given Data EEPROM location

  @Example
    <code>
    uint16_t dataeeAddr = 0x1010;
    uint8_t readData;
    EEPROM_STATUS eepStat;
    eepStat = DATAEE_ReadByte(dataeeAddr, &readData);
    </code>
*/
EEPROM_STATUS DATAEE_ReadByte(uint16_t bAdd);





/**
  @Summary
    Erase a block of memory in the EEPROM

  @Description
    This routine will erase a block of memory from a start address 
    to an end address (inclusive).

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location from which data has to be read

  @Returns
    Data byte indicating a success or a failure.
    EEPROM_STATUS

  @Example
    <code>
    uint16_t startAddr = 0x1010;
    uint16_t endAddr = 0x2020;
    EEPROM_STATUS eepStat;
    eepStat = DATAEE_ReadByte(startAddr, endAddr);
    </code>
*/
EEPROM_STATUS DATAEE_EraseBlock(uint16_t bAddStart, uint16_t bAddEnd);


/**
  @Summary
    Get address of last byte written to eeprom

  @Description
    This routine will fetch the address of the last byte written to the eeprom.

  @Preconditions
    None

  @Param
    None

  @Returns
    EEPROM_STATUS

  @Example
    <code>
 * EEPROM_STATUS eepStat;
    eepStat = EEPROM_Initialize();
    </code>
*/
EEPROM_STATUS EEPROM_Initialize();



/**
  @Summary
    Write address of last byte written to eeprom, to the eeprom.

  @Description
    This routine will write the address of the last byte written to the eeprom,
    to the eeprom.

  @Preconditions
    None

  @Param
    None

  @Returns
    EEPROM_STATUS

  @Example
    <code>
    EEPROM_STATUS eepStat;
    eepStat = EEPROM_Update();
    </code>
*/
EEPROM_STATUS EEPROM_Update();

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _MEMORY_H
/**
 End of File
*/

