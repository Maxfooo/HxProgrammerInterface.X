/* 
 * File:   HX_I2C.h
 * Author: MaxR
 *
 * Created on October 16, 2017, 3:07 PM
 */

#ifndef HX_I2C_H
#define	HX_I2C_H

#include "HX_CommStd.h"
#include "HX_DeviceProperties.h"
#include "Utils.h"

#define TX_ARR_SIZE 256
#define I2C_FAIL 1

////////////////////
// HX_I2C PACKET STRUCTURE
// <Mode>
// <Slave Address>
// <# of Tx Bytes>
// <# of Rx Bytes>
// <Byte0, Byte1, Byte2, ...>
// <Checksum>
////////////////////

// I2C Modes
// HX_I2C_WRITE_MODE 0x00
// HX_I2C_WR_RD_MODE 0x10
// HX_I2C_READ_MODE  0x20
// HX_I2C_DEBUG_MODE0 0x30
// HX_I2C_DEBUG_MODE1 0x31
// HX_I2C_DEBUG_MODE2 0x32

////////////////////
// PROTOTYPES
////////////////////
void run_I2C(uint8_t hx_inData[], uint8_t size);



#endif	/* HX_I2C_H */

