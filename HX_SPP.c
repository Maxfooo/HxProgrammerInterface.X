#include <pic18f47k40.h>

#include "HX_SPP.h"
void run_SPP(uint8_t hx_inData[], uint8_t size)
{
    uint8_t numTxBytes;
    uint8_t numRxBytes;
    
    INTERRUPT_GlobalInterruptDisable();
    if (size < 3)
    {
        hx_state = HX_UC_ERROR_PC_COMM;
    }
    else if (hx_inData[0] != (size - 2))
    {
        hx_state = HX_UC_ERROR_PC_COMM;
    }
    else
    {
        numTxBytes = hx_inData[0];
        numRxBytes = hx_inData[1];
        hx_mainDataIndex = numRxBytes;
        hxSPP(numTxBytes, numRxBytes, &hx_inData[2], hx_mainData); //Sets hx_state depending on whether communicatino is successful
    }
    INTERRUPT_GlobalInterruptEnable();
}

void hxSPP(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
	uint8_t tempByte;
    uint8_t readbackCommState;
    uint8_t bitVal;
    uint16_t readbackTimer;
    
	//Setup initial Conditions
	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
	
	//Check if the packet is valid
	
	//Send TX data across SPP line
	for (uint8_t i = 0; i < numTX; i++){
		tempByte = txBuffer[i];
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_MID;
            __delay_us(1);
            LATA = HX_LATA_HIGH;
            __delay_us(1);
            LATA = HX_LATA_MID;
        }
        else{
            LATA = HX_LATA_MID;
            __delay_us(1);
            LATA = HX_LATA_LOW;
            __delay_us(1);
            LATA = HX_LATA_MID;
        }
		for (uint8_t j = 1; j < 8; j++){
			tempByte = tempByte << 1;
			if (tempByte & 0x80){
                //Set all non zero values to be equal to 1
                LATA = HX_LATA_HIGH;
            __delay_us(1);
                LATA = HX_LATA_MID;
			}
			else{
                LATA = HX_LATA_LOW;
            __delay_us(1);
                LATA = HX_LATA_MID;
			}
		}
	}
	
	//Read RX data across SPP Line
	for (uint8_t i = 0; i < numRX; i++){
		tempByte = 0;
        LATA = HX_LATA_MID;
        __delay_us(1);
        LATA = HX_LATA_HIGH;
        __delay_us(1);
        LATA = HX_LATA_LOW;
        __delay_us(1);
        LATA = HX_LATA_HIZ;
        __delay_us(1);
        for(uint8_t j = 0; j < 8; j++)
        {
            tempByte = tempByte << 1;
            readbackCommState = 0;
            bitVal = 2;
            readbackTimer = SPP_READBACK_TIMER;
            while(readbackTimer--)
            {
                if (readbackCommState == 0)         // ____(case 0)_____|---------|_________|---------|_______
                {
                    if (PORTAbits.RA4 == 1)
                    {
                        readbackCommState++;
                        bitVal = 2;
                    }
                }
                else if (readbackCommState == 1)    // _________|----(case 1)-----|_________|---------|_______
                {
                    if (PORTAbits.RA4 == 0)
                    {
                        readbackCommState++;
                        bitVal = 0;
                    }
                }
                else if (readbackCommState == 2)    // _________|---------|____(case 2)_____|---------|_______
                {
                    if (PORTAbits.RA4 == 1)
                    {
                        readbackCommState++;
                        bitVal = 2;
                    }
                }
                else if (readbackCommState == 3)    // _________|---------|_________|----(case 3)-----|_______
                {
                    if (PORTAbits.RA4 == 0)
                    {
                        readbackTimer = 0;
                        __delay_us(12);
                        readbackCommState++;
                        bitVal = 1; 
                    }
                }
            }
            if(bitVal == 1)
            {
                tempByte = tempByte++;
            }
            else if (bitVal == 2)
            {
                hx_state = HX_UC_ERROR_DUT_COMM;
                break;
            }
        }
        if (hx_state == HX_UC_ERROR_DUT_COMM)
        {
            break;
        }
		rxBuffer[i] = tempByte;
	}
	
	//Clean Up
    if(hx_state != HX_UC_ERROR_DUT_COMM)
    {
        hx_state = HX_UC_COMPLETE;
    }
	LATA = HX_ENABLE; /* reset the chip to be enabled */
}

void setResistance(uint16_t steps)
{
    uint16_t tempSteps = 64;
    LATAbits.LA5 = 0x1;
    LATAbits.LA4 = 0x0;
    while (tempSteps != 0)
       {
       LATAbits.LA5 = 0x0;
       LATAbits.LA5 = 0x1;
       tempSteps--;
       }
    LATAbits.LA4 = 0x1;

    LATAbits.LA5 = 0x0;
    LATAbits.LA4 = 0x0;
    while (steps != 0)
       {
       LATAbits.LA5 = 0x1;
       LATAbits.LA5 = 0x0;
       steps--;
       }
    LATAbits.LA4 = 0x1;
}

void updateEnable()
{
    if (PORTAbits.RA5 == 0x1)
    {
        LATA = HX_LATA_HIGH;
    }
    else LATA = HX_LATA_LOW;
}