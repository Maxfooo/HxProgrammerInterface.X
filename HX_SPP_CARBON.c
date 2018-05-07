#include "HX_SPP_CARBON.h"

void run_SPPCarbon_test(uint8_t hx_inData[], uint8_t size)
{
    uint8_t numTxBytes;
    uint8_t numRxBytes;
    uint8_t returnValue;
    
    INTERRUPT_GlobalInterruptDisable();
    if (size != 3)
    {
        hx_state = HX_UC_ERROR_PC_COMM;
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_WRITE_TO_REG)
    {
        numTxBytes = 3;
        hx_mainDataIndex = 1;
        //hxSPPCarbonWriteAndVerity_test(numTxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
        hxSPPCarbonWrite_test(numTxBytes, &hx_inData[0], hx_mainData);
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_READ_FROM_REG)
    {
        numTxBytes = 3;
        numRxBytes = 2;
        hx_mainDataIndex = numRxBytes+1;
        hxSPPCarbonRead_test(numTxBytes, numRxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_WRITE_TO_OTP)
    {
        numTxBytes = 3;
        hx_mainDataIndex = 1;
        hxSPPCarbonProgram_test(numTxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_READ_FROM_OTP)
    {
        numTxBytes = 3;
        numRxBytes = 2;
        hx_mainDataIndex = numRxBytes+1;
        hxSPPCarbonRead_test(numTxBytes, numRxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_READ_CHIP_ID)
    {
        numTxBytes = 3;
        numRxBytes = 2;
        hx_mainDataIndex = numRxBytes+1;
        hxSPPCarbonRead_test(numTxBytes, numRxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_SELECT_ACT)
    {
        numTxBytes = 3;
        hx_mainDataIndex = 1;
        returnValue = hxSPPCarbonWriteReg_test(numTxBytes, &hx_inData[0]); //Sets hx_state depending on whether communication is successful
        if (returnValue != HX_DUT_COMPLETE)
        {
            hx_state = HX_UC_ERROR_DUT_COMM;
        }
        else
        {
            hx_state = HX_UC_COMPLETE;
            hx_mainData[0] = HX_DUT_COMPLETE;
        }
    }
    else if((hx_inData[0] & SPP_MODE_MASK) == SPP_CHECK_STATE)
    {
        numTxBytes = 3;
        numRxBytes = 2;
        hx_mainDataIndex = numRxBytes+1;
        hxSPPCarbonCheckBusy_test(numTxBytes, numRxBytes, &hx_inData[0], hx_mainData); //Sets hx_state depending on whether communication is successful
    }
    //////////////////
    // HARDWARE DEBUGGING - TRYING TO BREAK CarbonSPP DEVICE
    else if ( (hx_inData[0] & SPP_MODE_MASK) == SPP_SEND_HALF_DATA)
    {
        numTxBytes = 2;
        // change the command to write reg, so that the block recognizes the command
        hx_inData[0] = SPP_MODE_MASK & SPP_WRITE_TO_REG;
        hx_mainDataIndex = 1;
        hxSPPCarbonWriteAndVerity_test(numTxBytes, &hx_inData[0], hx_mainData);
    }
    else if ( (hx_inData[0] & SPP_MODE_MASK) == SPP_REQUEST_HALF_DATA)
    {
        numTxBytes = 3;
        numRxBytes = 1;
        hx_inData[0] = SPP_MODE_MASK & SPP_READ_FROM_REG;
        hx_mainDataIndex = numRxBytes+1;
        hxSPPCarbonRead_test(numTxBytes, numRxBytes, &hx_inData[0], hx_mainData);
    }
    else if ( (hx_inData[0] & SPP_MODE_MASK) == SPP_WRITE_READ_RAPID)
    {
        numTxBytes = 3;
        hx_mainDataIndex = 1;
        hx_inData[0] = SPP_MODE_MASK & SPP_WRITE_TO_REG;
        hxSPPCarbonWriteAndVerity_test(numTxBytes, &hx_inData[0], hx_mainData);
        
    }
    //////////////////
    else
    {
        hx_state = HX_UC_ERROR_PC_COMM;
        
        // allowing sending bad commands, at the moment, to test the response
        // from the CarbonSPP device on hardware
        numTxBytes = 3;
        hx_mainDataIndex = 1;
        hxSPPCarbonWriteAndVerity_test(numTxBytes, &hx_inData[0], hx_mainData);
         
    }
    INTERRUPT_GlobalInterruptEnable();
}

//****************************************************************************************************************
void hxSPPCarbonWriteAndVerity_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
    uint8_t returnValue;
    uint8_t tempValue;
    returnValue = hxSPPCarbonWriteReg_test(numTX, txBuffer);
    if (returnValue != HX_DUT_COMPLETE)
    {
        hx_state = HX_UC_ERROR_DUT_COMM;
        return;
    }
    
    tempValue = txBuffer[0];
    tempValue = tempValue & SPP_INDEX_MASK;
    tempValue = tempValue | SPP_READ_FROM_REG;
    txBuffer[0] = tempValue;
    returnValue = hxSPPCarbonReadReg_test(numTX, 2, txBuffer, &rxBuffer[1]);
    if (returnValue != HX_DUT_COMPLETE)
    {
        hx_state = HX_UC_ERROR_DUT_COMM;
        return;
    }
    for (uint8_t i = 1; i < numTX; i++)
    {
        if(rxBuffer[i] != txBuffer[i])
        {
            hx_state = HX_UC_ERROR_DUT_COMM;
            return;
        }
    }
    rxBuffer[0] = HX_DUT_COMPLETE;
    hx_state = HX_UC_COMPLETE;
}

//****************************************************************************************************************
void hxSPPCarbonWrite_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
    uint8_t returnValue;
    returnValue = hxSPPCarbonWriteReg_test(numTX, txBuffer);
    if (returnValue != HX_DUT_COMPLETE)
    {
        hx_state = HX_UC_ERROR_DUT_COMM;
        return;
    }
    
    rxBuffer[0] = HX_DUT_COMPLETE;
    hx_state = HX_UC_COMPLETE;
}

//****************************************************************************************************************
void hxSPPCarbonRead_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
    uint8_t returnValue;
    returnValue = hxSPPCarbonReadReg_test(numTX, numRX, txBuffer, &rxBuffer[1]);
    if (returnValue != HX_DUT_COMPLETE)
    {
        hx_state = HX_UC_ERROR_DUT_COMM;
        return;
    }
    rxBuffer[0] = HX_DUT_COMPLETE;
    hx_state = HX_UC_COMPLETE;
}

//****************************************************************************************************************
uint8_t hxSPPCarbonReadReg_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
	uint8_t tempByte;
    uint8_t readbackCommState;
    uint8_t bitVal;
    uint16_t readbackTimer;
    uint8_t oddParityCalc = 0;
    uint8_t portAVal = 0;
    uint8_t tempHxState = HX_DUT_COMPLETE;
    
    oddParityCalc = hxOddParityCalc(txBuffer, 3);
    
	//Setup initial Conditions
//	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
	
	//Check if the packet is valid
	
	//Send TX data across SPP line
    
    //Send Preamble Nibble 
    tempByte = 0xA0;
    if (tempByte & 0x80){
        //Set all non zero values to be equal to 1
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else{
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    for (uint8_t j = 1; j < 4; j++){
        tempByte = tempByte << 1;
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
    }
    
	for (uint8_t i = 0; i < numTX; i++){
        
		tempByte = txBuffer[i];
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
		for (uint8_t j = 1; j < 8; j++){
			tempByte = tempByte << 1;
			if (tempByte & 0x80){
                //Set all non zero values to be equal to 1
                LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
			else{
                LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
		}
	}
    
    // Tx odd parity of mode+MSByte+LSByte
    if (oddParityCalc)
    {
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else
    {
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    
    // Tx mandatory stop bit == 0
    __delay_us(1);
    LATA = HX_LATA_LOW_TEST;
    __delay_us(1);
    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
    // THING THE SPP DEVICE SEES.
	
	//Read RX data across SPP Line
    for (uint8_t i = 0; i < numRX; i++)
    {
        tempByte = 0;
        
        for(uint8_t j = 0; j < 8; j++)
        {
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
    //        LATA = HX_LATA_HIZ;
//            __delay_us(1);
            
            tempByte = tempByte << 1;
            readbackCommState = 0;
            bitVal = 2;
            //readbackTimer = SPP_READBACK_TIMER;
            readbackTimer = SPP_READBACK_TIMER_DEBUG;
            while(readbackTimer--)
            {
                portAVal = PORTA;
                if (readbackCommState == 0)         // ____(case 0)_____|---------|_________|---------|_______
                {
                    // looking for a one to get to the next state
                    if (PORTAbits.RA4 == 1)
                    //if (1 == portAVal & 0x01)
                    {
                        readbackCommState = 1;
                        bitVal = 3;
                    }
                }
                else if (readbackCommState == 1)    // _________|----(case 1)-----|_________|---------|_______
                {
                    if (PORTAbits.RA4 == 0)
                    //if (0 == portAVal & 0x01)
                    {
                        readbackCommState = 2;
                        bitVal = 0;
                    }
                }
                else if (readbackCommState == 2)    // _________|---------|____(case 2)_____|---------|_______
                {
                    if (PORTAbits.RA4 == 1)
                    //if (1 == portAVal & 0x01)
                    {
                        readbackCommState = 3;
                        bitVal = 4;
                    }
                }
                else if (readbackCommState == 3)    // _________|---------|_________|----(case 3)-----|_______
                {
                    if (PORTAbits.RA4 == 0)
                    //if (0 == portAVal & 0x01)
                    {
                        readbackCommState = 3;
                        bitVal = 1; 
                    }
                }
            }
            if(bitVal == 0)
            {
               //Do nothing
            }
            else if(bitVal == 1)
            {
                tempByte = tempByte++;
            }
            else
            {
                tempHxState = HX_UC_ERROR_DUT_COMM;
                break;
            }
        }
        if (tempHxState == HX_UC_ERROR_DUT_COMM)
        {
            break;
        }
        rxBuffer[i] = tempByte;
    }
    if(tempHxState == HX_UC_ERROR_DUT_COMM)
    {
        tempHxState = HX_UC_ERROR_DUT_COMM;
    }
    else
    {
        tempHxState = HX_DUT_COMPLETE;
    }
    return tempHxState;
}

//****************************************************************************************************************
uint8_t hxSPPCarbonWriteReg_test(uint8_t numTX, uint8_t *txBuffer)
{
	uint8_t tempByte;
    uint8_t oddParityCalc = 0;
    uint8_t tempHxState = HX_DUT_COMPLETE;
    oddParityCalc = hxOddParityCalc(txBuffer, 3);
    
    //-------------------WRITE TO REGISTER------------------------------
    
	//Setup initial Conditions
//	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
	
	//Send TX data across SPP line
    //Send Preamble Nibble
    tempByte = 0xA0;
    if (tempByte & 0x80){
        //Set all non zero values to be equal to 1
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else{
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    for (uint8_t j = 1; j < 4; j++){
        tempByte = tempByte << 1;
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
    }
    
	for (uint8_t i = 0; i < numTX; i++){
        
		tempByte = txBuffer[i];
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
		for (uint8_t j = 1; j < 8; j++){
			tempByte = tempByte << 1;
			if (tempByte & 0x80){
                //Set all non zero values to be equal to 1
                LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
			else{
                LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
		}
	}
    
    // Tx odd parity of mode+MSByte+LSByte
    if (oddParityCalc)
    {
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else
    {
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    
    // Tx mandatory stop bit == 0
    __delay_us(1);
    LATA = HX_LATA_LOW_TEST;
    __delay_us(1);
    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
    // THING THE SPP DEVICE SEES.
    return tempHxState;
}


//****************************************************************************************************************
void hxSPPCarbonProgram_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
	uint8_t tempByte;
    uint8_t oddParityCalc = 0;
    
    oddParityCalc = hxOddParityCalc(txBuffer, 3);
    
	//Setup initial Conditions
//	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
	
	//Send TX data across SPP line
    //Send Preamble Nibble
    tempByte = 0xA0;
    if (tempByte & 0x80){
        //Set all non zero values to be equal to 1
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else{
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    for (uint8_t j = 1; j < 4; j++){
        tempByte = tempByte << 1;
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
    }
    
	for (uint8_t i = 0; i < numTX; i++){
        
		tempByte = txBuffer[i];
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
		for (uint8_t j = 1; j < 8; j++){
			tempByte = tempByte << 1;
			if (tempByte & 0x80){
                //Set all non zero values to be equal to 1
                LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
			else{
                LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
		}
	}
    
    // Tx odd parity of mode+MSByte+LSByte
    if (oddParityCalc)
    {
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else
    {
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    
    // Tx mandatory stop bit == 0
    __delay_us(1);
    LATA = HX_LATA_LOW_TEST;
    __delay_us(1);
    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
    // THING THE SPP DEVICE SEES.
    rxBuffer[0] = HX_DUT_BUSY;
    
	//Clean Up
    if(hx_state != HX_UC_ERROR_DUT_COMM)
    {
        hx_state = HX_DUT_COMPLETE;
    }
	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
}

//****************************************************************************************************************
void hxSPPCarbonCheckBusy_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
{
	uint8_t tempByte;
    uint8_t readbackCommState;
    uint8_t bitVal;
    uint16_t readbackTimer;
    uint8_t oddParityCalc = 0;
    uint8_t portAVal = 0;
    
    oddParityCalc = hxOddParityCalc(txBuffer, 3);
    
	//Setup initial Conditions
//	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
	
	//Check if the packet is valid
	
	//Send TX data across SPP line
    
    
    
    //Send Preamble Nibble
    tempByte = 0xA0;
    if (tempByte & 0x80){
        //Set all non zero values to be equal to 1
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else{
        LATA = HX_LATA_MID_TEST;
        __delay_us(1);
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    for (uint8_t j = 1; j < 4; j++){
        tempByte = tempByte << 1;
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
    }
    
	for (uint8_t i = 0; i < numTX; i++){
        
		tempByte = txBuffer[i];
        if (tempByte & 0x80){
            //Set all non zero values to be equal to 1
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
        else{
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
            LATA = HX_LATA_MID_TEST;
        }
		for (uint8_t j = 1; j < 8; j++){
			tempByte = tempByte << 1;
			if (tempByte & 0x80){
                //Set all non zero values to be equal to 1
                LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
			else{
                LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
                LATA = HX_LATA_MID_TEST;
			}
		}
	}
    
    // Tx odd parity of mode+MSByte+LSByte
    if (oddParityCalc)
    {
        LATA = HX_LATA_HIGH_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    else
    {
        LATA = HX_LATA_LOW_TEST;
        __delay_us(1);
        LATA = HX_LATA_MID_TEST;
    }
    
    // Tx mandatory stop bit == 0
    __delay_us(1);
    LATA = HX_LATA_LOW_TEST;
    __delay_us(1);
    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
    // THING THE SPP DEVICE SEES.
	
    hx_state = HX_UC_COMPLETE;
    rxBuffer[0] = HX_DUT_COMPLETE;
    rxBuffer[1] = 0x00;
	//Read RX data across SPP Line
    for (uint8_t i = 0; i < numRX; i++)
    {
        tempByte = 0;
        
        for(uint8_t j = 0; j < 8; j++)
        {
            LATA = HX_LATA_MID_TEST;
            __delay_us(1);
            LATA = HX_LATA_HIGH_TEST;
            __delay_us(1);
            LATA = HX_LATA_LOW_TEST;
            __delay_us(1);
    //        LATA = HX_LATA_HIZ;
//            __delay_us(1);
            
            tempByte = tempByte << 1;
            readbackCommState = 0;
            bitVal = 2;
            //readbackTimer = SPP_READBACK_TIMER;
            readbackTimer = SPP_READBACK_TIMER_DEBUG;
            while(readbackTimer--)
            {
                portAVal = PORTA;
                if (readbackCommState == 0)         // ____(case 0)_____|---------|_________|---------|_______
                {
                    // looking for a one to get to the next state
                    //if (PORTAbits.RA4 == 1)
                    if (1 == portAVal & 0x01)
                    {
                        readbackCommState = 1;
                        bitVal = 2;
                    }
                }
                else if (readbackCommState == 1)    // _________|----(case 1)-----|_________|---------|_______
                {
                    //if (PORTAbits.RA4 == 0)
                    if (0 == portAVal & 0x01)
                    {
                        readbackCommState = 2;
                        bitVal = 0;
                    }
                }
                else if (readbackCommState == 2)    // _________|---------|____(case 2)_____|---------|_______
                {
                    //if (PORTAbits.RA4 == 1)
                    if (1 == portAVal & 0x01)
                    {
                        readbackCommState = 3;
                        bitVal = 4;
                    }
                }
                else if (readbackCommState == 3)    // _________|---------|_________|----(case 3)-----|_______
                {
                    //if (PORTAbits.RA4 == 0)
                    if (0 == portAVal & 0x01)
                    {
                        readbackCommState = 3;
                        bitVal = 1; 
                    }
                }
            }
            if(bitVal == 0)
            {
               //Do nothing
            }
            else if(bitVal == 1)
            {
                tempByte = tempByte++;
            }
            else
            {
                rxBuffer[0] = HX_DUT_BUSY;
                rxBuffer[1] = 0x00;
                break;
            }
        }
        if (rxBuffer[0] == HX_DUT_BUSY)
        {
            break;
        }
        rxBuffer[i+1] = tempByte;
    }
	
	//Clean Up
    hx_state = HX_UC_COMPLETE;
	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
}

//****************************************************************************************************************
uint8_t hxOddParityCalc(uint8_t *buff, uint8_t size)
{
    uint8_t parity = 1;
    uint8_t temp = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        temp = buff[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (temp & 0x80)
            {
                parity++;
            }
            temp = temp << 1;
        }
        
    }
    parity = parity & 0x01;
    
    return parity;
}






//
//void hxSPPCarbonWrite_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer)
//{
//	uint8_t tempByte;
//    uint8_t oddParityCalc = 0;
//    oddParityCalc = hxOddParityCalc(txBuffer, 3);
//    
//    //-------------------WRITE TO REGISTER------------------------------
//    
//	//Setup initial Conditions
////	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
//	
//	//Send TX data across SPP line
//    //Send Preamble Nibble
//    tempByte = 0xA0;
//    if (tempByte & 0x80){
//        //Set all non zero values to be equal to 1
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else{
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    for (uint8_t j = 1; j < 4; j++){
//        tempByte = tempByte << 1;
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//    }
//    
//	for (uint8_t i = 0; i < numTX; i++){
//        
//		tempByte = txBuffer[i];
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//		for (uint8_t j = 1; j < 8; j++){
//			tempByte = tempByte << 1;
//			if (tempByte & 0x80){
//                //Set all non zero values to be equal to 1
//                LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//			else{
//                LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//		}
//	}
//    
//    // Tx odd parity of mode+MSByte+LSByte
//    if (oddParityCalc)
//    {
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else
//    {
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    
//    // Tx mandatory stop bit == 0
//    __delay_us(1);
//    LATA = HX_LATA_LOW_TEST;
//    __delay_us(1);
//    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
//    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
//    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
//    // THING THE SPP DEVICE SEES.
//    
//    //-------------------READBACK FROM REGISTER TO VERIFY------------------------------
//    
//    rxBuffer[0] = HX_DUT_COMPLETE;
//    
//	//Clean Up
//    if(hx_state != HX_UC_ERROR_DUT_COMM)
//    {
//        hx_state = HX_DUT_COMPLETE;
//    }
//	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
//}
//
//
//
//void hxSPPCarbonRead_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
//{
//	uint8_t tempByte;
//    uint8_t readbackCommState;
//    uint8_t bitVal;
//    uint16_t readbackTimer;
//    uint8_t oddParityCalc = 0;
//    uint8_t portAVal = 0;
//    
//    oddParityCalc = hxOddParityCalc(txBuffer, 3);
//    
//	//Setup initial Conditions
////	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
//	
//	//Check if the packet is valid
//	
//	//Send TX data across SPP line
//    
//    
//    
//    //Send Preamble Nibble
//    tempByte = 0xA0;
//    if (tempByte & 0x80){
//        //Set all non zero values to be equal to 1
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else{
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    for (uint8_t j = 1; j < 4; j++){
//        tempByte = tempByte << 1;
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//    }
//    
//	for (uint8_t i = 0; i < numTX; i++){
//        
//		tempByte = txBuffer[i];
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//		for (uint8_t j = 1; j < 8; j++){
//			tempByte = tempByte << 1;
//			if (tempByte & 0x80){
//                //Set all non zero values to be equal to 1
//                LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//			else{
//                LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//		}
//	}
//    
//    // Tx odd parity of mode+MSByte+LSByte
//    if (oddParityCalc)
//    {
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else
//    {
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    
//    // Tx mandatory stop bit == 0
//    __delay_us(1);
//    LATA = HX_LATA_LOW_TEST;
//    __delay_us(1);
//    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
//    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
//    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
//    // THING THE SPP DEVICE SEES.
//	
//	//Read RX data across SPP Line
//    for (uint8_t i = 0; i < numRX; i++)
//    {
//        tempByte = 0;
//        
//        for(uint8_t j = 0; j < 8; j++)
//        {
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//    //        LATA = HX_LATA_HIZ;
////            __delay_us(1);
//            
//            tempByte = tempByte << 1;
//            readbackCommState = 0;
//            bitVal = 2;
//            //readbackTimer = SPP_READBACK_TIMER;
//            readbackTimer = SPP_READBACK_TIMER_DEBUG;
//            while(readbackTimer--)
//            {
//                portAVal = PORTA;
//                if (readbackCommState == 0)         // ____(case 0)_____|---------|_________|---------|_______
//                {
//                    // looking for a one to get to the next state
//                    //if (PORTAbits.RA4 == 1)
//                    if (1 == portAVal & 0x01)
//                    {
//                        readbackCommState = 1;
//                        bitVal = 3;
//                    }
//                }
//                else if (readbackCommState == 1)    // _________|----(case 1)-----|_________|---------|_______
//                {
//                    //if (PORTAbits.RA4 == 0)
//                    if (0 == portAVal & 0x01)
//                    {
//                        readbackCommState = 2;
//                        bitVal = 0;
//                    }
//                }
//                else if (readbackCommState == 2)    // _________|---------|____(case 2)_____|---------|_______
//                {
//                    //if (PORTAbits.RA4 == 1)
//                    if (1 == portAVal & 0x01)
//                    {
//                        readbackCommState = 3;
//                        bitVal = 4;
//                    }
//                }
//                else if (readbackCommState == 3)    // _________|---------|_________|----(case 3)-----|_______
//                {
//                    //if (PORTAbits.RA4 == 0)
//                    if (0 == portAVal & 0x01)
//                    {
//                        readbackCommState = 3;
//                        bitVal = 1; 
//                    }
//                }
//            }
//            if(bitVal == 0)
//            {
//               //Do nothing
//            }
//            else if(bitVal == 1)
//            {
//                tempByte = tempByte++;
//            }
//            else
//            {
//                hx_state = HX_UC_ERROR_DUT_COMM;
//                break;
//            }
//        }
//        if (hx_state == HX_UC_ERROR_DUT_COMM)
//        {
//            break;
//        }
//        rxBuffer[i+1] = tempByte;
//    }
//    rxBuffer[0] = HX_DUT_COMPLETE;
//    
//	
//	//Clean Up
//    if(hx_state != HX_UC_ERROR_DUT_COMM)
//    {
//        hx_state = HX_DUT_COMPLETE;
//    }
//	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
//}
//
//
//void hxSPPCarbonProgram_test(uint8_t numTX, uint8_t *txBuffer, uint8_t *rxBuffer)
//{
//	uint8_t tempByte;
//    uint8_t oddParityCalc = 0;
//    
//    oddParityCalc = hxOddParityCalc(txBuffer, 3);
//    
//	//Setup initial Conditions
////	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
//	
//	//Send TX data across SPP line
//    //Send Preamble Nibble
//    tempByte = 0xA0;
//    if (tempByte & 0x80){
//        //Set all non zero values to be equal to 1
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else{
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    for (uint8_t j = 1; j < 4; j++){
//        tempByte = tempByte << 1;
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//    }
//    
//	for (uint8_t i = 0; i < numTX; i++){
//        
//		tempByte = txBuffer[i];
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//		for (uint8_t j = 1; j < 8; j++){
//			tempByte = tempByte << 1;
//			if (tempByte & 0x80){
//                //Set all non zero values to be equal to 1
//                LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//			else{
//                LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//		}
//	}
//    
//    // Tx odd parity of mode+MSByte+LSByte
//    if (oddParityCalc)
//    {
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else
//    {
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    
//    // Tx mandatory stop bit == 0
//    __delay_us(1);
//    LATA = HX_LATA_LOW_TEST;
//    __delay_us(1);
//    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
//    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
//    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
//    // THING THE SPP DEVICE SEES.
//    rxBuffer[0] = HX_DUT_BUSY;
//    
//	//Clean Up
//    if(hx_state != HX_UC_ERROR_DUT_COMM)
//    {
//        hx_state = HX_DUT_COMPLETE;
//    }
//	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
//}
//
//void hxSPPCarbonCheckBusy_test(uint8_t numTX, uint8_t numRX, uint8_t *txBuffer, uint8_t *rxBuffer)
//{
//	uint8_t tempByte;
//    uint8_t readbackCommState;
//    uint8_t bitVal;
//    uint16_t readbackTimer;
//    uint8_t oddParityCalc = 0;
//    uint8_t portAVal = 0;
//    
//    oddParityCalc = hxOddParityCalc(txBuffer, 3);
//    
//	//Setup initial Conditions
////	LATA = HX_LATA_HIZ; /* make sure chip is in h1-z */
//	
//	//Check if the packet is valid
//	
//	//Send TX data across SPP line
//    
//    
//    
//    //Send Preamble Nibble
//    tempByte = 0xA0;
//    if (tempByte & 0x80){
//        //Set all non zero values to be equal to 1
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else{
//        LATA = HX_LATA_MID_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    for (uint8_t j = 1; j < 4; j++){
//        tempByte = tempByte << 1;
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//    }
//    
//	for (uint8_t i = 0; i < numTX; i++){
//        
//		tempByte = txBuffer[i];
//        if (tempByte & 0x80){
//            //Set all non zero values to be equal to 1
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//        else{
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_MID_TEST;
//        }
//		for (uint8_t j = 1; j < 8; j++){
//			tempByte = tempByte << 1;
//			if (tempByte & 0x80){
//                //Set all non zero values to be equal to 1
//                LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//			else{
//                LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//                LATA = HX_LATA_MID_TEST;
//			}
//		}
//	}
//    
//    // Tx odd parity of mode+MSByte+LSByte
//    if (oddParityCalc)
//    {
//        LATA = HX_LATA_HIGH_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    else
//    {
//        LATA = HX_LATA_LOW_TEST;
//        __delay_us(1);
//        LATA = HX_LATA_MID_TEST;
//    }
//    
//    // Tx mandatory stop bit == 0
//    __delay_us(1);
//    LATA = HX_LATA_LOW_TEST;
//    __delay_us(1);
//    // DO NOT RETURN TO MID STATE AFTER STOP BIT.
//    // READBACK SHOULD TAKE CARE OF ITSELF, BUT WHEN
//    // YOU ARE ONLY TX, THE STOP BIT HAS TO BE THE LAST
//    // THING THE SPP DEVICE SEES.
//	
//    rxBuffer[0] = HX_DUT_COMPLETE;
//	//Read RX data across SPP Line
//    for (uint8_t i = 0; i < numRX; i++)
//    {
//        tempByte = 0;
//        
//        for(uint8_t j = 0; j < 8; j++)
//        {
//            LATA = HX_LATA_MID_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_HIGH_TEST;
//            __delay_us(1);
//            LATA = HX_LATA_LOW_TEST;
//            __delay_us(1);
//    //        LATA = HX_LATA_HIZ;
////            __delay_us(1);
//            
//            tempByte = tempByte << 1;
//            readbackCommState = 0;
//            bitVal = 2;
//            //readbackTimer = SPP_READBACK_TIMER;
//            readbackTimer = SPP_READBACK_TIMER_DEBUG;
//            while(readbackTimer--)
//            {
//                portAVal = PORTA;
//                if (readbackCommState == 0)         // ____(case 0)_____|---------|_________|---------|_______
//                {
//                    // looking for a one to get to the next state
//                    //if (PORTAbits.RA4 == 1)
//                    if (1 == portAVal & 0x01)
//                    {
//                        readbackCommState = 1;
//                        bitVal = 3;
//                    }
//                }
//                else if (readbackCommState == 1)    // _________|----(case 1)-----|_________|---------|_______
//                {
//                    //if (PORTAbits.RA4 == 0)
//                    if (0 == portAVal & 0x01)
//                    {
//                        readbackCommState = 2;
//                        bitVal = 0;
//                    }
//                }
//                else if (readbackCommState == 2)    // _________|---------|____(case 2)_____|---------|_______
//                {
//                    //if (PORTAbits.RA4 == 1)
//                    if (1 == portAVal & 0x01)
//                    {
//                        readbackCommState = 3;
//                        bitVal = 4;
//                    }
//                }
//                else if (readbackCommState == 3)    // _________|---------|_________|----(case 3)-----|_______
//                {
//                    //if (PORTAbits.RA4 == 0)
//                    if (0 == portAVal & 0x01)
//                    {
//                        readbackCommState = 3;
//                        bitVal = 1; 
//                    }
//                }
//            }
//            if(bitVal == 0)
//            {
//               //Do nothing
//            }
//            else if(bitVal == 1)
//            {
//                tempByte = tempByte++;
//            }
//            else if (bitVal == 2)
//            {
//                rxBuffer[0] = HX_DUT_COMPLETE;
//                break;
//            }
//            else
//            {
//                hx_state = HX_UC_ERROR_DUT_COMM;
//                break;
//            }
//        }
//        if (bitVal == 2)
//        {
//            break;
//        }
//        else if (hx_state == HX_UC_ERROR_DUT_COMM)
//        {
//            break;
//        }
//        rxBuffer[i+1] = tempByte;
//    }
//	
//	//Clean Up
//    if(hx_state != HX_UC_ERROR_DUT_COMM)
//    {
//        hx_state = HX_DUT_COMPLETE;
//    }
//	LATA = HX_ENABLE_TEST; /* reset the chip to be enabled */
//}
//uint8_t hxOddParityCalc(uint8_t *buff, uint8_t size)
//{
//    uint8_t parity = 1;
//    uint8_t temp = 0;
//    for (uint8_t i = 0; i < size; i++)
//    {
//        temp = buff[i];
//        for (uint8_t j = 0; j < 8; j++)
//        {
//            if (temp & 0x80)
//            {
//                parity++;
//            }
//            temp = temp << 1;
//        }
//        
//    }
//    parity = parity & 0x01;
//    
//    return parity;
//}