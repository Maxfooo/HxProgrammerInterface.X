#include "HX_CommStd.h"
#include "HX_DeviceProperties.h"
#define HX_LATA_MID_TEST 0x01
#define HX_LATA_HIGH_TEST 0x03
#define HX_LATA_LOW_TEST 0x00
#define HX_ENABLE_TEST HX_LATA_HIGH_TEST

// VALUE    | TIME  | PASS/FAIL
// 70       |         | PASS
// 60       |         | PASS
// 55       |         | PASS
// 50       |         | PASS
// 40       | 69-78us | PASS
// 30       |         | FAIL
// 35       |         | FAIL
// note: these times represent the time between readback pulses
// note: the time also include if statements
#define SPP_READBACK_TIMER_DEBUG 40
//#define SPP_READBACK_TIMER_DEBUG 70



void run_SPPCarbon_test(uint8_t hx_inData[], uint8_t size);
void hxSPPCarbonRead_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer);
void hxSPPCarbonWriteAndVerity_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer);
//void hxSPPCarbonWrite_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer);
uint8_t hxSPPCarbonReadReg_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer);
uint8_t hxSPPCarbonWriteReg_test(uint8_t numTX, uint8_t *txBuffer);
void hxSPPCarbonProgram_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer);
void hxSPPCarbonCheckBusy_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer);
void hxSPPCarbonWrite_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer);
uint8_t hxOddParityCalc(uint8_t *buff, uint8_t size);