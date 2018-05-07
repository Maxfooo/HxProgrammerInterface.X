/* 
 * File:   BORON_CommStd.h
 * Author: maxr
 *
 * Created on July 14, 2017, 11:50 AM
 */

#ifndef BORON_COMMSTD_H
#define	BORON_COMMSTD_H

/********** GLOBALS **********/
#define SLAVE_I2C_GENERIC_RETRY_MAX     100
#define BORON_SLAVE_ADDR            =   0x5A;
uint8_t ardSlvAddr = 0x2A;
uint8_t boronSlvAddr = 0x5A;

/********** BORON READ COMMANDS **********/
#define BORON_READ_OTP              0x20
#define BORON_READ_DYNAMIC_PARAM    0x21
#define BORON_READ_RAM              0x22
#define BORON_READ_XDATA            0x23
#define BORON_READ_CODE             0x24
#define BORON_READ_STATIC_PARAM     0x25
#define BORON_READ_CMD_COUNT        6
const uint8_t BORON_READ_CMDS[BORON_READ_CMD_COUNT] = { BORON_READ_OTP,
                                                    BORON_READ_DYNAMIC_PARAM,
                                                    BORON_READ_RAM,
                                                    BORON_READ_XDATA,
                                                    BORON_READ_CODE,
                                                    BORON_READ_STATIC_PARAM};

/********** BORON WRITE COMMANDS **********/
#define BORON_WRITE_OTP             0x30
#define BORON_WRITE_DYNAMIC_PARAM   0x31
#define BORON_WRITE_RAM             0x32
#define BORON_WRITE_XDATA           0x33
#define BORON_WRITE_STATIC_PARAM    0x34
#define BORON_WRITE_CMD_COUNT       5
const uint8_t BORON_WRITE_CMDS[BORON_WRITE_CMD_COUNT] = {   BORON_WRITE_OTP,
                                                        BORON_WRITE_DYNAMIC_PARAM,
                                                        BORON_WRITE_RAM,
                                                        BORON_WRITE_XDATA,
                                                        BORON_WRITE_STATIC_PARAM};

/********** BORON ERRORS **********/
// ----- BORON errors ----- (Boron errors)
#define BORON_ERR_DATA_BUFFER_BUSY          0x11
#define BORON_ERR_COMM_NUM_BYTES_OVERFLOW   0x12
#define BORON_ERR_COMMAND                   0x13
#define BORON_ERR_INCOMPLETE_OP             0x14
#define BORON_ERR_CHECKSUM                  0x15
#define BORON_ERR_BYTE_COUNT                0x16
#define BORON_ERR_PARAM_REWRITE             0x17
#define BORON_ERR_SINGLE_WIRE               0x18
#define BORON_ERR_COUNT                     8
const uint8_t BORON_ERRS[BORON_ERR_COUNT] = {   BORON_ERR_DATA_BUFFER_BUSY,
                                            BORON_ERR_COMM_NUM_BYTES_OVERFLOW,
                                            BORON_ERR_COMMAND,
                                            BORON_ERR_INCOMPLETE_OP,
                                            BORON_ERR_CHECKSUM,
                                            BORON_ERR_BYTE_COUNT,
                                            BORON_ERR_PARAM_REWRITE,
                                            BORON_ERR_SINGLE_WIRE};

/********** BORON COMM **********/
#define BORON_PKT_SIZE 16
uint8_t boronTxPktIndex = 0;
uint8_t boronRxPktIndex = 0;
uint8_t boronTxPkt[BORON_PKT_SIZE];
uint8_t boronRxPkt[BORON_PKT_SIZE];


/********** BORON DEBUG **********/
uint8_t boronDebugTxData[1];








#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* BORON_COMMSTD_H */

