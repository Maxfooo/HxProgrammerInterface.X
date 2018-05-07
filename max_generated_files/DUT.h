/* 
 * File:   DUT.h
 * Author: maxr
 *
 * Created on May 5, 2017, 11:16 AM
 */

#ifndef DUT_H
#define	DUT_H

// ----- #include -----
#include "../mcc_generated_files/i2c1.h"
#include "Utils.h"
#include "ASL1000.h"

// ----- #define -----
#define SLAVE_I2C_GENERIC_RETRY_MAX     100


// ----- constants -----
const uint16_t DUT_SLAVE_ADDR = 0x5A;

typedef enum
{
    DUT_WRITE_FAILED,
    DUT_WRITE_SUCCESSFUL,
    DUT_WRITE_TIMEOUT,
         
    DUT_READ_FAILED,
    DUT_READ_SUCCESSFUL,
    DUT_READ_TIMEOUT,
} DUT_TEST_STATUS;

typedef enum
{
    DUT_CNFG_FAIL,
    DUT_CNGF_PASS,
} DUT_CNFG_STATUS;

// ----- DUT cmds ----- (Boron commands)
#define DUT_READ_OTP 0x20
#define DUT_READ_DYNAMIC_PARAM 0x21
#define DUT_READ_RAM 0x22
#define DUT_READ_XDATA 0x23
#define DUT_READ_CODE 0x24
#define DUT_READ_STATIC_PARAM 0x25
#define DUT_READ_CMD_COUNT 6
const uint8_t DUT_READ_CMDS[DUT_READ_CMD_COUNT] = { DUT_READ_OTP,
                                                    DUT_READ_DYNAMIC_PARAM,
                                                    DUT_READ_RAM,
                                                    DUT_READ_XDATA,
                                                    DUT_READ_CODE,
                                                    DUT_READ_STATIC_PARAM};


#define DUT_WRITE_OTP 0x30
#define DUT_WRITE_DYNAMIC_PARAM 0x31
#define DUT_WRITE_RAM 0x32
#define DUT_WRITE_XDATA 0x33
#define DUT_WRITE_STATIC_PARAM 0x34
#define DUT_WRITE_CMD_COUNT 5
const uint8_t DUT_WRITE_CMDS[DUT_WRITE_CMD_COUNT] = {   DUT_WRITE_OTP,
                                                        DUT_WRITE_DYNAMIC_PARAM,
                                                        DUT_WRITE_RAM,
                                                        DUT_WRITE_XDATA,
                                                        DUT_WRITE_STATIC_PARAM};

// ----- DUT errors ----- (Boron errors)
#define DUT_ERR_DATA_BUFFER_BUSY 0x11
#define DUT_ERR_COMM_NUM_BYTES_OVERFLOW 0x12
#define DUT_ERR_COMMAND 0x13
#define DUT_ERR_INCOMPLETE_OP 0x14
#define DUT_ERR_CHECKSUM 0x15
#define DUT_ERR_BYTE_COUNT 0x16
#define DUT_ERR_PARAM_REWRITE 0x17
#define DUT_ERR_SINGLE_WIRE 0x18
#define DUT_ERR_COUNT 8
const uint8_t DUT_ERRS[DUT_ERR_COUNT] = {   DUT_ERR_DATA_BUFFER_BUSY,
                                            DUT_ERR_COMM_NUM_BYTES_OVERFLOW,
                                            DUT_ERR_COMMAND,
                                            DUT_ERR_INCOMPLETE_OP,
                                            DUT_ERR_CHECKSUM,
                                            DUT_ERR_BYTE_COUNT,
                                            DUT_ERR_PARAM_REWRITE,
                                            DUT_ERR_SINGLE_WIRE};



// ----- PROTOTYPES -----
DUT_TEST_STATUS DUT_writeBytes(uint8_t [], int);
DUT_TEST_STATUS DUT_readBytes(uint8_t *, uint16_t);

// ----- tests -----
uint8_t DUT_test_reset();
uint8_t DUT_test_wrRdRAM();
uint8_t uC_test_debug();

// ----- configurations -----
uint8_t DUT_cnfg_oscDiv();
uint8_t DUT_cnfg_OTP();









#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* DUT_H */

