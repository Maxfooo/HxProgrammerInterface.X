/* 
 * File:   Utils.h
 * Author: maxr
 *
 * Created on May 8, 2017, 11:48 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include "mcc_generated_files/i2c1.h"


// ----- #defines -----
#define SLAVE_I2C_GENERIC_RETRY_MAX     100
#define I2C_COMM_SUCCESS 0
#define I2C_COMM_FAILURE 0
#define I2C_COMM_TIMEOUT 0
#define TX_RX_PACKET_SIZE 64
#define TX_PACKET_TYPE 0
#define RX_PACKET_TYPE 1
#define I2C_TR_BLOCK_SIZE 32
#define BORON_COMM_I2C 0
#define BORON_COMM_SW 1
#define DEFAULT_I2C_SPEED_VAL 0x9F

// globals

// ----- STRUCTS -----
typedef struct TxRxPacket {
    uint8_t txOrRx;
    uint16_t slaveAddr;
    uint8_t data[TX_RX_PACKET_SIZE];
    uint8_t numOfBytes;
    I2C1_MESSAGE_STATUS* status;
} txRxPkt;


// ----- PROTOTYPES -----
uint8_t calcChecksum(uint8_t [], uint8_t);
void clearArray(uint8_t [], uint8_t);
uint8_t writeI2C(uint8_t txData[], uint8_t size, uint16_t slaveAddr);
uint8_t readI2C(uint8_t rxData[], uint8_t numOfRx, uint16_t slaveAddr);
uint8_t copyUint8_tArr(uint8_t* toArr, uint8_t* fromArr, uint8_t size);
uint8_t streamBoronComm(uint8_t boron_comm_type, txRxPkt** pktArr, uint8_t pktSize);
uint8_t streamI2C(txRxPkt** pktArr, uint8_t pktSize);
uint8_t streamSW(txRxPkt** pktArr, uint8_t pktSize);


// -- Notes --

// delay in milliseconds macro
// __delay_ms(value);













#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

