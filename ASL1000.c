
/*********************************************************/
// TESTS TO RUN ON BORON, INITIATED BY ASL1000
/*********************************************************/


#include "ASL1000.h"

void ASL_Initialize()
{
    
}

void run_ASL_stateMachine(uint8_t hx_inData[], uint8_t size)
{
    ASL_resetStateMachine();
    
    for(uint8_t i = 0; i < size; i++)
    {
        if(ASL_stateMachine(hx_inData[i], 0))
        {
            break;
        }
    }
    ASL_copyToHxMainData();
    // hx_state is handled within the state machine
     
}

uint8_t ASL_stateMachine(uint8_t data_in, uint8_t reset)
{
    
    if (hx_abort)
    {
        return ASL_SM_FINISHED;
    }
    
    static ASL_STATE next_state = HANDLE_CMD;
    static uint8_t sm_state = ASL_SM_NOT_FINISHED;
    if (reset)
    {
        next_state = HANDLE_CMD;
        clearArray(asl_in_buffer, ASL_DATA_BUFFER_SIZE);
        clearArray(asl_out_buffer, ASL_DATA_BUFFER_SIZE);
        in_buffer_index = 0;
        out_buffer_index = 0;
        sm_state = ASL_SM_NOT_FINISHED;
        return ASL_SM_FINISHED;
    }
    
    asl_byte = data_in;
    asl_in_buffer[in_buffer_index++] = data_in;
    
    switch(next_state)
    {
        case HANDLE_CMD:
            
            next_state = ASL_handleCommand();
            
            if (next_state == ERROR)
            {
                sm_state = ASL_SM_FINISHED;
            }
            else
            {
                sm_state = ASL_SM_NOT_FINISHED;
            }
            break;
            
        case RUN_TEST:
            
            // debugging
            //asl_out_buffer[out_buffer_index++] = asl_cmd;
            //ASL_calcChecksum();
            //hx_debugVar = data_in;
            //return ASL_SM_FINISHED;
            // end debugging
            
            next_state = ASL_testRun();
            if (next_state == ERROR)
            {
                sm_state = ASL_SM_FINISHED;
            }
            else
            {
                sm_state = ASL_SM_FINISHED;
                hx_state = HX_UC_COMPLETE;
            }
            break;
                    
        case RUN_CNFG:
            // ASL_configRun() is called in the setup function
            next_state = ASL_configSetup();
            if (next_state == FINISHED)
            {
                sm_state = ASL_SM_FINISHED;
                hx_state = HX_UC_COMPLETE;
            }
            else if (next_state == ERROR)
            {
                sm_state = ASL_SM_FINISHED;
            }
            else
            {
                sm_state = ASL_SM_NOT_FINISHED;
            }
            break;
            
            
            
            
        // mainly for RUN_CNGF (and random calls to the state machine after it's
        // already finished. Also for error returns after the HANDLE_CMD state.
        case FINISHED:
            next_state = FINISHED;
            sm_state = ASL_SM_FINISHED;
            hx_state = HX_UC_COMPLETE;
            break;
            
        case ERROR:
            next_state = ERROR;
            sm_state = ASL_SM_FINISHED;
            break;
       
        default:  // Unknown Error
            next_state = ASL_error(ASL_RETURN_ERR_SM_UNKNOWN);
            sm_state = ASL_SM_FINISHED;
            break;
    }
    
    return sm_state;
}



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

ASL_STATE ASL_handleCommand()
{
    asl_cmd = asl_byte; // first byte will always be a command
    
    ASL_STATE next_state;
    
    // find out if the command is test or config mode
    uint8_t test_or_config = !!(asl_byte & CONFIG_MASK); // 1 = config
    
    // set asl_mode
    // 0 = Test 
    // 1 = Config 
    if (test_or_config)
    {
        asl_mode = ASL_CNFG_MODE;
        next_state = RUN_CNFG;
        out_buffer_index = 0; // reset out index only after confirming a RUN mode
        asl_out_buffer[out_buffer_index++] = asl_cmd; // begin response buffer
    }
    else
    {
        asl_mode = ASL_TEST_MODE;
        next_state = RUN_TEST;
        out_buffer_index = 0; // reset out index only after confirming a RUN mode
        asl_out_buffer[out_buffer_index++] = asl_cmd; // begin response buffer
    }
    
    
    return next_state;
    
}   

ASL_STATE ASL_testRun()
{
    // the checksum should be the only other byte of information sent
    // after the command to run test
    asl_checksum = asl_byte;
    ASL_STATE next_state;
    uint8_t asl_return_byte;
    
    if (ASL_evalChecksum())
    {       
        switch (asl_cmd)
        {
            case ASL_RESET: 
                asl_return_byte = BORON_test_reset();
                break;
                
            case ASL_TEST_WR_RD_RAM:
                asl_return_byte = BORON_test_wrRdRAM();
                break;
                
                
                
            case ASL_TEST_ARD_DEBUG:
                asl_return_byte = ARDUINO_test_debug();
                break;
                
            case ASL_TEST_UC_DEBUG:
                asl_return_byte = uC_test_debug();
                break;
                
            default: // unknown command
                return ASL_error(ASL_RETURN_ERR_BAD_COMMAND);
        }

        asl_out_buffer[out_buffer_index++] = asl_return_byte;
        ASL_calcChecksum();
        next_state = FINISHED;
    }
    else
    {
        return ASL_error(ASL_RETURN_ERR_BAD_CHECKSUM);
    }
    
    return next_state;
}

ASL_STATE ASL_configSetup()
{
    static CONFIG_STATE cnfg_state = CNFG_BYTES;
    static struct sfr s;
    ASL_STATE next_state;
    
    switch (cnfg_state)
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
        if (ASL_evalChecksum())
        {
            next_state = ASL_configRun();
        }
        else
        {
            next_state = ASL_error(ASL_RETURN_ERR_BAD_CHECKSUM);
        }
    } 
    else
    {
        next_state = RUN_CNFG;
    }
    
    return next_state;
}

ASL_STATE ASL_configRun()
{
    uint8_t asl_return_byte = 0;
    
    // Run appropriate test
    switch (asl_cmd)
    {
        case ASL_CONFIG_OSC_DIV:
            asl_return_byte = BORON_cnfg_oscDiv();
            break;
            
        case ASL_CONFIG_OTP:
            asl_return_byte = BORON_cnfg_OTP();
            break;
            
            
        case ASL_CONFIG_DEBUG:
            asl_return_byte = BORON_cnfg_debug();
            break;
            
        default:
            return ASL_error(ASL_RETURN_ERR_BAD_COMMAND);
    }   
    
    asl_out_buffer[out_buffer_index++] = asl_return_byte;
    ASL_calcChecksum();
    return FINISHED;
    
}

ASL_STATE ASL_error(uint8_t error_status)
{
    out_buffer_index = 0;
    asl_mode = ASL_ERROR_MODE;
    asl_status_return = error_status;
    asl_out_buffer[out_buffer_index++] = ASL_RETURN_ERR_PREAMBLE;
    asl_out_buffer[out_buffer_index++] = asl_cmd;
    asl_out_buffer[out_buffer_index++] = asl_status_return;
    ASL_calcChecksum();
    
    switch (error_status)
    {
        case ASL_RETURN_ERR_COMM_LOW:
            hx_state = HX_UC_ERROR_PC_COMM;
            break;
            
        case ASL_RETURN_ERR_BAD_COMMAND:
            hx_state = HX_UC_ERROR_INTERNAL_PKT;
            break;
            
        case ASL_RETURN_ERR_BAD_CHECKSUM:
            hx_state = HX_UC_ERROR_PC_CHECKSUM;
            break;
            
        case ASL_RETURN_ERR_BORON_COMM:
            hx_state = HX_UC_ERROR_DUT_COMM;
            break;
            
        case ASL_RETURN_ERR_SM_UNKNOWN:
            hx_state = HX_UC_ERROR_UNKNOWN;
            break;
            
        case ASL_RETURN_ERR_COMM_HIGH:
            hx_state = HX_UC_ERROR_PC_COMM;
            break;
    }
    
    // write command and errors to external memory here
    // 
    
    return ERROR;
}

uint8_t ASL_evalChecksum()
{
    // compute checksum and verify against asl_checksum
    
    uint8_t good_checksum = 0;
    uint8_t temp_checksum = 0;
    
    if (in_buffer_index < 3) // test command sent, just perform two's compliment
    {
        temp_checksum = (~asl_cmd) + 1;
        good_checksum = temp_checksum == asl_checksum ? 1 : 0;
    }
    else // configure command sent
    {
        temp_checksum = calcChecksum(asl_in_buffer, in_buffer_index-1);
        good_checksum = temp_checksum == asl_checksum ? 1 : 0;
    }
    
    return good_checksum;
}

void ASL_calcChecksum()
{
    uint8_t temp_checksum = 0;
    for (uint8_t i = 0; i < out_buffer_index; i++)
    {
        temp_checksum += asl_out_buffer[i];
    }
    temp_checksum = (temp_checksum ^ 0xFF) + 1;
    asl_out_buffer[out_buffer_index++] = temp_checksum;
    
}


void ASL_copyToHxMainData()
{
    for (uint8_t i = 0; i < out_buffer_index; i++)
    {
        hx_mainData[hx_mainDataIndex++] = asl_out_buffer[i];
    }
}
