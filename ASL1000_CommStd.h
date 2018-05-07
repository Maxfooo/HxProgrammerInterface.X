/*
 * File:   ASL1000_CommStd.h
 * Author: maxr
 *
 * Created on July 13, 2017, 1:31 PM
 */

#ifndef ASL1000_COMMSTD_H
#define	ASL1000_COMMSTD_H

/*====================================================
NOMENCLATURE FOR COMMAND BYTES

-- if TEST COMMAND
#define ASL_TEST_<TEST_NAME> 0xhh
where 0xhh < 0x80

-- if CONFIG COMMAND
CONFIG_MASK = 0x80;
#define ASL_CONFIG_<CONFIG_NAME> (CONFIG_MASK | 0xhh)
====================================================*/

//////////////////////////////////////////////
// MICRO AND PC INTERFACE COMMON
//////////////////////////////////////////////

//========== TEST COMMANDS ==========//
#define ASL_RESET           0x00
#define ASL_TEST_WR_RD_RAM  0x01
#define ASL_TEST_ARD_DEBUG  0x1E
#define ASL_TEST_UC_DEBUG   0x1F
#define NUM_OF_TEST_CMDS    4
//const uint8_t test_cmd_array[NUM_OF_TEST_CMDS] = {ASL_RESET,
//                                                ASL_TEST_WR_RD_RAM,
//                                                ASL_TEST_ARD_DEBUG,
//                                                ASL_TEST_UC_DEBUG};
uint8_t test_cmd_array[NUM_OF_TEST_CMDS];


//========== CONFIG COMMANDS ==========//
#define CONFIG_MASK         0x80 // MSB
#define ASL_CONFIG_OSC_DIV  (CONFIG_MASK | 0x01)
#define ASL_CONFIG_OTP      (CONFIG_MASK | 0x02)
#define ASL_CONFIG_DEBUG    (CONFIG_MASK | 0x1F)
#define NUM_OF_CNFG_CMDS    3
//const uint8_t cnfg_cmd_array[NUM_OF_CNFG_CMDS] = {ASL_CONFIG_OSC_DIV,
//                                                ASL_CONFIG_OTP,
//                                                ASL_CONFIG_DEBUG};
uint8_t cnfg_cmd_array[NUM_OF_CNFG_CMDS];

//========== RETURN STATUSES ==========//
#define ASL_RETURN_ERR_COMM_LOW     0x00
#define ASL_RETURN_FAIL             0x01
#define ASL_RETURN_PASS             0x02
#define ASL_RETURN_ERR_BAD_COMMAND  0x03
#define ASL_RETURN_ERR_BAD_CHECKSUM 0x04
#define ASL_RETURN_ERR_BORON_COMM   0x05
#define ASL_RETURN_ERR_SM_UNKNOWN   0xAA
#define ASL_RETURN_ERR_PREAMBLE     0xEE
#define ASL_RETURN_ERR_COMM_HIGH    0xFF

////------------------------------------------

//////////////////////////////////////////////
// MICROCONTROLLER C ONLY
//////////////////////////////////////////////

typedef enum
{
    NONE,
    HANDLE_CMD,
    RUN_TEST,
    RUN_CNFG,
    FINISHED,
    ERROR
} ASL_STATE;

typedef enum
{
    CNFG_BYTES,
    CNFG_ADDR,
    CNFG_DATA_HIGH,
    CNFG_DATA_LOW,
    CNFG_CHECKSUM
} CONFIG_STATE;

typedef enum
{
    ASL_TEST_MODE,
    ASL_CNFG_MODE,
    ASL_ERROR_MODE
} ASL_MODE;

//========== STATE MACHINE ==========//
#define ASL_SM_FINISHED     1
#define ASL_SM_NOT_FINISHED 0

//========== globals ==========//
#define ASL_DATA_BUFFER_SIZE    64
#define SFR_ARRAY_SIZE          64
uint8_t asl_cmd             =   0;
uint8_t asl_byte            =   0;
uint8_t asl_checksum        =   0;
ASL_MODE asl_mode           =   0;
uint8_t asl_status_return   =   0;
uint8_t in_buffer_index     =   0;
uint8_t out_buffer_index    =   0;
uint8_t i2c_out_index       =   0;
uint8_t cnfgBytes           =   0;
uint8_t cnfgByteCntr        =   0;
uint8_t asl_in_buffer   [ASL_DATA_BUFFER_SIZE];
uint8_t asl_out_buffer  [ASL_DATA_BUFFER_SIZE];
struct sfr {uint8_t addr; uint8_t dataHigh; uint8_t dataLow;};
struct sfr sfr_array[SFR_ARRAY_SIZE];


////------------------------------------------


//////////////////////////////////////////////
// PC C++ (QT5) ONLY
/*/////////////////////////////////////////////

const unsigned char READBACK_BUFFER_SIZE = 32;
struct sfr {QString name; unsigned char relAddr; unsigned char absAddr; unsigned char msByte; unsigned char lsByte;};
struct ASL_cmd {unsigned char cmdByte; unsigned char cmdRequestByte; int cmdRuns; std::vector<sfr> cmdConfig; QString cmdName; QString cmdDesc;};
extern bool debug_mode;

*///------------------------------------------

#endif	/* ASL1000_COMMSTD_H */

