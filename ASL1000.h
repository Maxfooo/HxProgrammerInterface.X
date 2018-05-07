/* 
 * File:   ASL1000.h
 * Author: maxr
 *
 * Created on May 2, 2017, 11:59 AM
 */

#ifndef ASL1000_H
#define	ASL1000_H

#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/mcc.h"
#include "BORON.h"
#include "Utils.h"
#include "EEPROM.h"
#include "HX_CommInterface.h"
#include "ASL1000_CommStd.h"

/**
 * @Name
 *  run_ASL_stateMachine
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
 *  
 * 
 * 
 **/
void run_ASL_stateMachine(uint8_t hx_inData[], uint8_t size);

/**
 * @Name
 *  ASL_staticStateMachine
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
 *  
 * 
 * 
 **/
void ASL_staticStateMachine();

/**
 * @Name
 *  ASL_resetStateMachine
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
 *  
 * 
 * 
 **/
void ASL_resetStateMachine();

/**
 * @Name
 *  ASL_stateMachine
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
 *  
 * 
 * 
 **/
uint8_t ASL_stateMachine(uint8_t data_in, uint8_t reset);

/**
 * @Name
 *  ASL_handleCommand
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_handleCommand();

/**
 * @Name
 *  ASL_testSetup
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_testSetup();

/**
 * @Name
 *  ASL_testRun
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_testRun();

/**
 * @Name
 *  ASL_configSetup
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_configSetup();

/**
 * @Name
 *  ASL_configRun
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_configRun();

/**
 * @Name
 *  ASL_error
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
 *  
 * 
 * 
 **/
ASL_STATE ASL_error(uint8_t);

/**
 * @Name
 *  ASL_evalChecksum
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
 *  
 * 
 * 
 **/
uint8_t ASL_evalChecksum();

/**
 * @Name
 *  ASL_calcChecksum
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
 *  
 * 
 * 
 **/
void ASL_calcChecksum();

/**
 * @Name
 *  ASL_copyToHxMainData
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
 *  
 * 
 * 
 **/
void ASL_copyToHxMainData();

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
 *  
 * 
 * 
 **/

#endif	/* ASL1000_H */

