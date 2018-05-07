#include "HX_CommStd.h"
#include "HX_DeviceProperties.h"

/*********************************************************************
 * Function Name: run_SPP
 * 
 * Description: Starts the SPP communication sequence
 * 
 * Parameters: None
 * 
 * Returns: None
 *
 * Notes:
 ********************************************************************/
void run_SPP(uint8_t hx_inData[], uint8_t size);


/*********************************************************************
 * Function Name: hxSPP
 * 
 * Description: sends and receives generic communication packets to the SPP line
 * in little endian format.
 * 
 * Parameters:
 * uint8_t numTX : number of bytes to write to the SPP line
 * uint8_t numRX : number of bytes to readback from SPP line (directly after writing)
 * uint8_t *txBuffer : pointer to initial location of the transmit data array
 * uint8_t *rxBuffer : pointer to initial location of the receive data array
 * 
 * Returns: None
 *
 * Notes:
 ********************************************************************/
void hxSPP(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer);

/*********************************************************************
* Function: void setResistance(uint16_t steps);
*
* Overview: Sets the resistance of the Rheostat in the tri level circuit
*
* PreCondition: None
*
* Input: Number of steps
*
* Output: None
*
********************************************************************/
void setResistance(uint16_t);


/*********************************************************************
* Function: void updateEnable();
*
* Overview: checks the external enable line and updates the global enable value based on it
*
* PreCondition: None
*
* Input: Number of steps
*
* Output: None
*
********************************************************************/
void updateEnable(void);