/* 
 * File:   Utils.h
 * Author: maxr
 *
 * Created on May 8, 2017, 11:48 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include "../mcc_generated_files/i2c1.h"

// ----- #defines -----
#define MICRO_I2C_SLAVE_MODE 0x00
#define MICRO_I2C_MASTER_MODE 0x01

#define INPUT   1
#define OUTPUT  0

#define ANALOG      1
#define DIGITAL     0

#define SET_HIGH 1
#define SET_LOW 0

#define SET_ASL 1
#define SET_DUT 0

// ===== SFR ALIASES =====
#define PORTA_LATCH LATA
#define PORTA_ANLG_SEL ANSELA
#define PORTA_DATA_DIR TRISA


// -----  GLOBALS -----
int uC_i2c_mode = MICRO_I2C_SLAVE_MODE;



// ----- PROTOTYPES -----
void initPortA(char, char);
void setnBusyLine(int);
void switchI2CMux(int);
void initI2CMaster();
void initI2CSlave();
void MICRO_Initialize();
uint8_t calcChecksum(uint8_t [], int);
void clearArray(uint8_t [], int);













#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

