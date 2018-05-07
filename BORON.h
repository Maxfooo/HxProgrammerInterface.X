/* 
 * File:   BORON.h
 * Author: maxr
 *
 * Created on May 5, 2017, 11:16 AM
 */

#ifndef BORON_H
#define	BORON_H

#include "mcc_generated_files/i2c1.h"
#include "Utils.h"
#include "ASL1000.h"
#include "BORON_CommStd.h"



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
uint8_t BORON_test_reset();

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
uint8_t BORON_test_wrRdRAM();

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
uint8_t uC_test_debug();

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
uint8_t ARDUINO_test_debug();

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
uint8_t BORON_cnfg_oscDiv();

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
uint8_t BORON_cnfg_OTP();

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
uint8_t BORON_cnfg_debug();


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* BORON_H */

