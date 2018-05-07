/*********************************************************/
// TESTS TO RUN ON DUT, INITIATED BY ASL1000
/*********************************************************/


// ----- #include -----
#include "ASL1000.h"
#include "EEPROM.h"
// ----- end #include -----

/*
extern uint8_t asl_cmd;
extern uint8_t asl_byte;
extern uint8_t asl_checksum;
extern ASL_MODE asl_mode;
extern uint8_t asl_in_buffer [ASL_DATA_BUFFER_SIZE];
extern uint8_t asl_out_buffer [ASL_DATA_BUFFER_SIZE];
extern uint8_t asl_status_return;
extern int in_buffer_index;
extern int out_buffer_index;
extern int i2c_out_index;
extern int cnfgBytes;
extern int cnfgByteCntr;
*/

void ASL_staticStateMachine()
{
    // If this function is called, then no data was being
    // transmitted, it was just being called in a start/stop
    // state in the I2C state machine
    ASL_stateMachine(0, 0);
    // Decrement in_buffer_index to avoid accessing the
    // asl_in_buffer out of bounds. (the index is incremented
    // each time ASL_stateMachine() is called)
    in_buffer_index--;
}

void ASL_resetStateMachine()
{
    ASL_stateMachine(0, 1);
}

void ASL_stateMachine(uint8_t data_in, char reset)
{
    static ASL_STATE next_state = HANDLE_CMD;
    if(reset)
    {
        next_state = HANDLE_CMD;
        clearArray(asl_in_buffer, ASL_DATA_BUFFER_SIZE);
        return;
    }
    asl_byte = data_in;
    asl_in_buffer[in_buffer_index++] = data_in;
    
    
    switch(next_state)
    {
        case HANDLE_CMD:
            next_state = ASL_handleCommand();
            break;
            
        case RUN_TEST:
            next_state = ASL_testRun();
            break;
            
        case RUN_TEST_READBACK:
            next_state = ASL_testRdbk();
            break;
                    
        case RUN_CNFG:
            next_state = ASL_configSetup();
            // ASL_configRun() is called in the setup function
            break;
            
        case RUN_CNFG_READBACK:
            next_state = ASL_configRdbk();
            break;
            
        case WAIT_FOR_CMD:
            next_state = WAIT_FOR_CMD;
            break;
        
        // ===== ERROR STATES =====
        // SHOULD REPORT ERROR ON OUTBUFFER
        // POSSIBLY WRITE ERROR TO EXTERNAL MEMORY
        // FOR DEBUGGING LATER?
        case ERROR:
            next_state = ERROR;
            
            break;
       
            
        default:  // Unknown Error
            next_state = ASL_error(ASL_RETURN_ERR_SM_UNKNOWN);
            break;
    }
}

ASL_STATE ASL_handleCommand()
{
    asl_cmd = asl_byte; // first byte will always be a command
    
    ASL_STATE next_state;
    
    // find out if the command is readback or not
    char readback = !!(asl_byte & CMD_READBACK_MASK); // 1 = readback
    
    // find out if the command is test or config mode
    char test_or_config = !!(asl_byte & CONFIG_MASK); // 1 = config
    
    // remove readback flag and verify that the command sent
    // actually exists
    char temp_asl_cmd = readback ? (asl_cmd & (~CONFIG_MASK)) : asl_cmd;
    char good_cmd = 0;
    for(int i = 0; i < NUM_OF_TEST_CMDS; i++)
    {
        if(temp_asl_cmd == test_cmd_array[i])
        {
            good_cmd = 1;
        }
    }
    for(int i = 0; i < NUM_OF_CNFG_CMDS; i++)
    {
        if(temp_asl_cmd == cnfg_cmd_array[i])
        {
            good_cmd = 1;
        }
    }
    if(!good_cmd)
    {
        return ASL_error(ASL_RETURN_ERR_BAD_COMMAND);
    }
    
    // set asl_mode
    char mode = (readback << 1) + test_or_config;
    switch(mode) {
        case 0:
            asl_mode = ASL_TEST_MODE;
            next_state = RUN_TEST;
            out_buffer_index = 0; // reset out index only after confirming a RUN mode
            asl_out_buffer[out_buffer_index++] = asl_cmd; // begin response buffer
            break;
        case 1:
            asl_mode = ASL_CNFG_MODE;
            next_state = RUN_CNFG;
            out_buffer_index = 0; // reset out index only after confirming a RUN mode
            asl_out_buffer[out_buffer_index++] = asl_cmd; // begin response buffer
            break;
        case 2:
            asl_mode = ASL_TEST_READBACK_MODE;
            next_state = RUN_TEST_READBACK;
            break;
        case 3:
            asl_mode = ASL_CNFG_READBACK_MODE;
            next_state = RUN_CNFG_READBACK;
            break;
        default: // error
            break;
    }
    
    return next_state;
    
}   

ASL_STATE ASL_testRun()
{
    // the checksum should be the only other byte of information sent
    // after the command to run test
    asl_checksum = asl_byte;
    ASL_STATE next_state;
    DUT_TEST_STATUS asl_return_byte;
    
    if (ASL_evalChecksum())
    {
        // Bring nBusy line LOW (uC is busy then) AND switch mux to make I2C connection
        // to the Boron chip instead of the ASL1000.
        ASL_setupCommToDUT();    
        
        // switch case does not work here
        // https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm
        // FILL IN THE REST OF THE TESTS AS YOU GENERATE THEM
        if (asl_cmd == ASL_RESET)               { asl_return_byte = DUT_test_reset(); }
        else if (asl_cmd == ASL_TEST_WR_RD_RAM) {asl_return_byte = DUT_test_wrRdRAM(); }
        else if (asl_cmd == ASL_TEST_UC_DEBUG)  {asl_return_byte = uC_test_debug(); }
        // else if (asl_cmd == ) { }
        else
        {
            asl_return_byte = ASL_RETURN_ERR_BAD_COMMAND;
            asl_out_buffer[out_buffer_index++] = asl_return_byte;
            return ASL_error(ASL_RETURN_ERR_BAD_COMMAND);
        }

        ASL_takedownCommToDUT();

        asl_out_buffer[out_buffer_index++] = asl_return_byte;
        next_state = WAIT_FOR_CMD;
        next_state = ASL_testRun(); // RUN THE TEST HERE
    }
    else
    {
        return ASL_error(ASL_RETURN_ERR_BAD_CHECKSUM);
    }
    
    return next_state;
}

ASL_STATE ASL_testRdbk()
{
    // output buffer is supposed to be full by now, so this will just
    // handle passing data values to the i2c buffer while the ASL is requesting.
    
    ASL_calcChecksum(); // calculate checksum to send back
    
    static int asl_rdbk_state = 0;
    ASL_STATE next_state;
    
    switch(asl_rdbk_state)
    {
        case 0: // only necessary for the very first call
            i2c_out_index = 0;
            asl_rdbk_state = 1;
            next_state = RUN_TEST_READBACK;
            break;
            
        case 1:
            
            
            if(i2c_out_index >= out_buffer_index)
            {
                i2c_out_index = 0;
                asl_rdbk_state = 1;
                next_state = RUN_TEST_READBACK;
            }
            else
            {
                i2c_out_index++;
                asl_rdbk_state = 1;
                if (i2c_out_index == out_buffer_index - 1)
                {
                    next_state = WAIT_FOR_CMD;
                }
            }
    
            break;
            
        default: // case error
            next_state = WAIT_FOR_CMD;
            break;
    }
    
    return next_state;
    
    
}


ASL_STATE ASL_configSetup()
{
    static CONFIG_STATE cnfg_state = CNFG_BYTES;
    static struct sfr s;
    ASL_STATE next_state;
    
    switch(cnfg_state)
    {
        case CNFG_BYTES:
            cnfgByteCntr = 0;
            cnfgBytes = asl_byte;
            cnfg_state = CNFG_ADDR;
            break;

        case CNFG_ADDR:
            s.addr = asl_byte;
            cnfg_state = CNFG_DATA_HIGH;
            break;

        case CNFG_DATA_HIGH:
            s.dataHigh = asl_byte;
            cnfg_state = CNFG_DATA_LOW;
            break;

        case CNFG_DATA_LOW:
            s.dataLow = asl_byte;
            cnfg_state = CNFG_ADDR;
            sfr_array[cnfgByteCntr++] = s;
            // once cnfgByteCnrt increments to cnfgBytes
            // the if-else statement below will catch
            // it and assign the states appropriately.
            // the sfr_array should never be accessed
            // out of bounds.
            break;
            
        case CNFG_CHECKSUM:
            asl_checksum = asl_byte;
            cnfgByteCntr++;
            break;
            
        default: // error
            break;
    }
    
    if (cnfgByteCntr == cnfgBytes)
    {
        cnfg_state = CNFG_CHECKSUM;
    }
    else if (cnfgByteCntr > cnfgBytes)
    {
        cnfg_state = CNFG_BYTES;
        next_state = ASL_evalChecksum() ? ASL_configRun() : 
            ASL_error(ASL_RETURN_ERR_BAD_CHECKSUM);
    } 
    else
    {
        next_state = RUN_CNFG;
    }
    
    return next_state;
}

ASL_STATE ASL_configRun()
{
    DUT_CNFG_STATUS asl_return_byte;
    ASL_setupCommToDUT();
    
    // Run appropriate test
    // see ASL_testRun as to why I used if-else statements instead of a switch-case
    if (asl_cmd == ASL_CONFIG_OSC_DIV) {asl_return_byte = DUT_cnfg_oscDiv(); }
    else if (asl_cmd == ASL_CONFIG_OTP) {asl_return_byte = DUT_cnfg_OTP(); }
    // else if (asl_cmd == ) { }
    else
    {
        asl_return_byte = ASL_RETURN_ERR_BAD_COMMAND;
        asl_out_buffer[out_buffer_index++] = asl_return_byte;
        return ASL_error(ASL_RETURN_ERR_BAD_COMMAND);
    }
    
    ASL_takedownCommToDUT();
    asl_out_buffer[out_buffer_index++] = asl_return_byte;
    return WAIT_FOR_CMD;
    
}

ASL_STATE ASL_configRdbk()
{
    // the most recent results should have been placed in the
    // asl_out_buffer[] so this function simply builds the checksum
    ASL_calcChecksum();
    
    // DO STUFF HERE
    // set up state machine to return config results.
    
    
    
    
    return WAIT_FOR_CMD;
}

ASL_STATE ASL_error(int error_status)
{
    out_buffer_index = 0;
    asl_mode = ASL_ERROR_MODE;
    asl_status_return = error_status;
    asl_out_buffer[out_buffer_index++] = asl_cmd;
    asl_out_buffer[out_buffer_index++] = asl_status_return;
    ASL_calcChecksum();
    
    
    
    return ERROR;
}

void ASL_setupCommToDUT()
{
    // Pull !busy line low 
    setnBusyLine(SET_LOW);
    
    // DON'T NEED IF USING BOTH I2C MODULES (MASTER IS I2C_2)
    // Switch external mux (using GPIO) to route SDA and SCL lines to the DUT
    // 
    // switchI2CMux(SET_DUT);
    
    
    // Set uC to I2C master mode
    initI2CMaster();
    
}

void ASL_takedownCommToDUT()
{
    // set uC back to I2C slave mode
    initI2CSlave();
    
    // DON'T NEED IF USING BOTH I2C MODULES (MASTER IS I2C_2)
    // Switch external mux (using GPIO) to route SDA and SCL lines back to the ASL1000
    // 
    // switchI2CMux(SET_ASL);
    
    // release !busy line so it can go back high
    // letting the ASL1000 know that it can talk
    // to this uC again
    setnBusyLine(SET_HIGH);
    
}


char ASL_evalChecksum()
{
    char good_checksum;
    ASL_STATE next_state;
    // do checksum calc and verification
    uint8_t temp_checksum = 0;
    
    if(in_buffer_index < 3) // test command sent, just perform two's compliment
    {
        temp_checksum = (~asl_in_buffer[0]) + 1;
        good_checksum = !(temp_checksum ^ asl_checksum);
    }
    else // configure command sent
    {
        for (int i = 0; i < in_buffer_index-1; i++)
        {
            temp_checksum += asl_in_buffer[i];
        }
        temp_checksum = (~temp_checksum) + 1;
        good_checksum = !(temp_checksum ^ asl_checksum);
    }
    
    return good_checksum;
    
    
}

void ASL_calcChecksum()
{
    uint8_t temp_checksum = 0;
    for(int i = 0; i < out_buffer_index; i++)
    {
        temp_checksum += asl_out_buffer[i];
    }
    temp_checksum = (~temp_checksum) + 1;
    asl_out_buffer[out_buffer_index++] = temp_checksum;
    
}



