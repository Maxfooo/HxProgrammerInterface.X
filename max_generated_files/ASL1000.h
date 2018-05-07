/* 
 * File:   ASL1000.h
 * Author: maxr
 *
 * Created on May 2, 2017, 11:59 AM
 */

#ifndef ASL1000_H
#define	ASL1000_H

// ----- #include -----
#include "../mcc_generated_files/i2c1.h"
#include "../mcc_generated_files/mcc.h"
#include "DUT.h"
#include "Utils.h"

// ----- end #include -----


typedef enum
{
    NONE,
    HANDLE_CMD,
            
    RUN_TEST,
    RUN_TEST_READBACK,
            
    RUN_CNFG,
    RUN_CNFG_READBACK,
            
    WAIT_FOR_CMD,
    
    ERROR,
} ASL_STATE;

typedef enum
{
    CNFG_BYTES,
    CNFG_ADDR,
    CNFG_DATA_HIGH,
    CNFG_DATA_LOW,
    CNFG_CHECKSUM,
} CONFIG_STATE;

typedef enum
{
    ASL_TEST_MODE,
    ASL_TEST_READBACK_MODE,
    ASL_CNFG_MODE,
    ASL_CNFG_READBACK_MODE,
    ASL_ERROR_MODE,
} ASL_MODE;




// ----- #define -----
#define ASL_DATA_BUFFER_SIZE 32
#define SFR_ARRAY_SIZE 32


// ----- CONSTANTS -----
#define CONFIG_MASK 0x40
#define CMD_READBACK_MASK 0x80

// ====TEST COMMANDS =====
#define ASL_RESET 0x00
#define ASL_TEST_WR_RD_RAM  0x01
#define ASL_TEST_UC_DEBUG 0x1F

#define NUM_OF_TEST_CMDS 3
const uint8_t test_cmd_array[NUM_OF_TEST_CMDS] = {ASL_RESET, 
                                                  ASL_TEST_WR_RD_RAM, 
                                                  ASL_TEST_UC_DEBUG};

// ===== CONFIG COMMANDS =====
#define ASL_CONFIG_OSC_DIV  (CONFIG_MASK | 0x01)
#define ASL_CONFIG_OTP      (CONFIG_MASK | 0x02)
#define NUM_OF_CNFG_CMDS 2
const uint8_t cnfg_cmd_array[NUM_OF_CNFG_CMDS] = {ASL_CONFIG_OSC_DIV, 
                                                  ASL_CONFIG_OTP};

// ===== RETURN STATUSES =====
#define ASL_RETURN_ERR_COMM_LOW 0x00
#define ASL_RETURN_FAIL 0x01
#define ASL_RETURN_PASS 0x02
#define ASL_RETURN_ERR_BAD_COMMAND 0x03
#define ASL_RETURN_ERR_BAD_CHECKSUM 0x04
#define ASL_RETURN_ERR_DUT_COMM 0x05
#define ASL_RETURN_ERR_SM_UNKNOWN 0xAA
#define ASL_RETURN_ERR_COMM_HIGH 0xFF

struct sfr {uint8_t addr; uint8_t dataHigh; uint8_t dataLow;};
struct sfr sfr_array[SFR_ARRAY_SIZE];




// ----- globals -----
uint8_t asl_cmd = 0;
uint8_t asl_byte = 0;
uint8_t asl_checksum = 0;
ASL_MODE asl_mode = 0;
uint8_t asl_in_buffer [ASL_DATA_BUFFER_SIZE];
uint8_t asl_out_buffer [ASL_DATA_BUFFER_SIZE];
uint8_t asl_status_return = 0;
int in_buffer_index = 0;
int out_buffer_index = 0;
int i2c_out_index = 0;
int cnfgBytes = 0;
int cnfgByteCntr = 0;

// ----- PROTOTYPES -----

void ASL_staticStateMachine();
void ASL_resetStateMachine();
void ASL_stateMachine(uint8_t, char);

ASL_STATE ASL_handleCommand();

ASL_STATE ASL_testSetup();
ASL_STATE ASL_testRun();
ASL_STATE ASL_testSetupRdbk();
ASL_STATE ASL_testRdbk();

ASL_STATE ASL_configSetup();
ASL_STATE ASL_configRun();
ASL_STATE ASL_configSetupRdbk();
ASL_STATE ASL_configRdbk();

ASL_STATE ASL_error(int);

char ASL_evalChecksum();
void ASL_calcChecksum();

void ASL_setupCommToDUT();
void ASL_takedownCommToDUT();





#endif	/* ASL1000_H */

