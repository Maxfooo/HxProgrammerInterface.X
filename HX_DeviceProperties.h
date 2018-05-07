#ifndef HX_DEVICEPROPERTIES_H
#define HX_DEVICEPROPERTIES_H
#include "mcc_generated_files/mcc.h"

/********** CHIP INFO **********/
#define HX_CHIP_ID 0x01
#define HX_CHIP_REVISION 0x01
#define HX_CHIP_FREQUENCY 64000000 // 64MHz

/********** Timer **********/
#define HX_TIMER_PC_RST_VAL         0xF63C // 10ms (default)
#define HX_TIMER_MAIN_CODE_RST_VAL  0x3CB0 // 200ms

/********** Comm TO PC **********/
// Position 0 and 1 are reserved for PC_CMD and numOfDataBytes
#define HX_PC_CMD_INDEX         0
#define HX_NUM_BYTES_INDEX      1
#define HX_DATA_OUT_ORIGIN      2
#define HX_PACKET_1_NUM_BYTES   2
#define HX_OUT_DATA_SIZE        64
#define HX_MAIN_DATA_SIZE       61
uint8_t hx_outDataIndex     =   0;
uint8_t hx_outData[HX_OUT_DATA_SIZE];
uint8_t hx_mainDataIndex    =   0;
uint8_t hx_mainData[HX_MAIN_DATA_SIZE];


/********** Comm FROM PC **********/
// only loading in data to be sent to the developer's code later
#define HX_IN_DATA_SIZE     64
uint8_t hx_inDataIndex  =   0;
uint8_t hx_inData[HX_IN_DATA_SIZE];


/********** Comm processing **********/
uint8_t hx_checksum = 0;
uint8_t hx_pcCmd    = 0;
uint8_t hx_pktCmd   = 0;
uint8_t hx_state    = 0x00;
uint8_t hx_pktState = 0x00;

/********** Flags **********/
uint8_t hx_rxFullPktFlag    = 0;
uint8_t hx_startMain        = 0;
uint8_t hx_rxOverflowFlag   = 0;
uint8_t hx_txPacketFlag     = 1;

/********** main code vars **********/
uint8_t hx_debugVar = 0;
uint8_t hx_abort    = 0;


/***** Single Pin Programming Definitions *****/
#define HX_LATA_HIGH        0x0e;
#define HX_LATA_LOW         0x0a;
#define HX_LATA_MID_START   0x09;  // initial transistor only drive to mid
#define HX_LATA_MID         0x01;
#define HX_LATA_HIZ         0x0b;
#define HX_ENABLE           HX_LATA_HIGH;
#define SPP_READBACK_TIMER  50;     //Roughly 60us of delay once the mid to high transition occurs


/********** LED PIN LOCATIONS *********/
#define LED_COMM_TX LATBbits.LB5





#endif