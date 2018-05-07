#include "HX_CommInterface.h"
#include "HX_CommStd.h"
#define HX_LATA_MID_TEST 0x01
#define HX_LATA_HIGH_TEST 0x03
#define HX_LATA_LOW_TEST 0x00
#define HX_ENABLE_TEST HX_LATA_HIGH_TEST

//General Comm Test Routines
void runTest(uint8_t hx_inData[], uint8_t size);

//SPP Comm Test Routines
void run_SPP_test(uint8_t hx_inData[], uint8_t size);
void hxSPP_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer);