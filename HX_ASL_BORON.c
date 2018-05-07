
#include "HX_ASL_BORON.h"

// STRUCTURE
// MODE   : [0xhh,
// TEST   :  0xhh,
// CONFIG{/NUM_OF_CONFIGS} :  0xhh,
// {CNFG_ADDR : 0xhh,
//  CNFG_MSB  : 0xhh,
//  CNFG_LSB  : 0xhh,}
// REPEAT :  0xhh, 0xhh, // multiplier, n-repetitions (TEST ONLY)
// CHCKSUM:  0xhh]

testSpecificLatchData tsld;
uint8_t BORON_COMM_TYPE = BORON_COMM_I2C;

void run_ASLBoron(uint8_t hx_inData[], uint8_t size)
{    
    uint8_t mode_run        = 0;
    uint8_t test_run        = 0;
    uint8_t cnfg_run        = 0;
    uint8_t cnfg_cnt        = 0;
    uint8_t cnfg_arr[CNFG_ARR_SIZE]; // arbitrary size
    uint16_t data_index     = 0;
    uint8_t rpt_run[2];
    uint8_t checksum        = 0;
    uint8_t calc_checksum   = 0;
    
    mode_run = hx_inData[data_index++];
    if (!(mode_run == ASL_B_TEST_MODE ||
        mode_run == ASL_B_CNFG_MODE   ||
        mode_run == ASL_B_CCNFG_MODE  ||
        mode_run == ASL_B_DEBUG_MODE))
    {
        hx_state = HX_UC_ERROR_PC_COMM;
        return;
    }
    else
    {
        test_run = hx_inData[data_index++];
        if (mode_run == ASL_B_CCNFG_MODE)
        {
            cnfg_cnt = hx_inData[data_index++];
            if ((3*cnfg_cnt > CNFG_ARR_SIZE) || (cnfg_cnt < 1))
            {
                hx_state = HX_UC_ERROR_INTERNAL_PKT;
                return;
            }
            for (uint8_t i = 0; i < 3*cnfg_cnt; i++)
            {
                cnfg_arr[i] = hx_inData[data_index++];
            }
        }
        else
        {
            cnfg_run = hx_inData[data_index++];
        }
        rpt_run[0] = hx_inData[data_index] == 0 ? 1 : hx_inData[data_index];
        data_index++;
        rpt_run[1] = hx_inData[data_index] == 0 ? 1 : hx_inData[data_index];
        data_index++;
        checksum = hx_inData[data_index++];
        calc_checksum = calcChecksum(hx_inData, data_index-1);
        if (calc_checksum != checksum)
        {
            hx_state = HX_UC_ERROR_PC_CHECKSUM;
            return;
        }
        else
        {
            switch(mode_run)
            {
                case ASL_B_TEST_MODE:
                    hx_mainDataIndex = 1;
                    write_ASLBoron_test(test_run, rpt_run, hx_mainData);
                    hx_state = HX_UC_COMPLETE;
                    break;
                    
                case ASL_B_CNFG_MODE:
                    hx_mainDataIndex = 1;
                    write_ASLBoron_cnfg(cnfg_run, hx_mainData);
                    hx_state = HX_UC_COMPLETE;
                    break;
                    
                case ASL_B_CCNFG_MODE:
                    hx_mainDataIndex = 1;
                    write_ASLBoron_cstm_cnfg(cnfg_arr, 3*cnfg_cnt, hx_mainData);
                    hx_state = HX_UC_COMPLETE;
                    break;
                
                case ASL_B_DEBUG_MODE:
                    hx_mainDataIndex = 1;
                    write_ASLBoron_debug(test_run, cnfg_run, hx_mainData);
                    hx_state = HX_UC_COMPLETE;
                    break;
                    
                default:
                    hx_state = HX_UC_ERROR_INTERNAL_PKT;
                    break;
            }
        }
    }
}

void write_ASLBoron_test(uint8_t run_test, uint8_t* run_rpt, uint8_t* rxBuffer)
{
    int i,j;
    
    for (i = 0; i < run_rpt[0]; i++)
    {
        for (j = 0; j < run_rpt[1]; j++)
        {
            switch (run_test)
            {
                case 0x00:
                    ASLB_ContinuityESD(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;

                case 0x01:
                    ASLB_SheetResistance(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x02:
                    ASLB_HeaterFunctionality(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x03:
                    ASLB_AnalogMuxReadingSetup(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x04:
                    ASLB_CTHERM(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x05:
                    ASLB_ADCReferenceVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x06:
                    ASLB_HeaterFBSignals(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x07:
                    ASLB_InternalTempSensor(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x08:
                    ASLB_MuxReadingVDD(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x09:
                    ASLB_DACReferenceVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0A:
                    ASLB_VaricapDACVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0B:
                    ASLB_TempSetDACVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0C:
                    ASLB_CurrentLimitDACVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0D:
                    ASLB_DigitalRegulatorVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0E:
                    ASLB_ZTCReferenceCurrent(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x0F:
                    ASLB_MuxOpenCircuit(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x10:
                    ASLB_OscillatorRegulatorVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x11:
                    ASLB_ExternalReferenceVoltage(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x12:
                    ASLB_VaricapBiasSteering(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x13:
                    ASLB_ColpittsOscillator(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x14:
                    ASLB_PierceOscillator(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x15:
                    ASLB_RDResistor(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x16:
                    ASLB_CFCVVaricap(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x17:
                    ASLB_CommonGate(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                case 0x18:
                    ASLB_ADC(&rxBuffer[1]);
                    rxBuffer[0] = HX_DUT_COMPLETE;
                    break;
                    
                    
                default:
                    hx_state = HX_UC_ERROR_INTERNAL_PKT;
                    rxBuffer[0] = HX_DUT_NACK;
                    break;
            } // end switch-case
        } // end for-j
    } // end for-i
}


void write_ASLBoron_cnfg(uint8_t run_cnfg, uint8_t* rxBuffer)
{
    switch (run_cnfg)
    {
        case 0x00:
            ASLB_cnfg0(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;

        case 0x01:
            ASLB_cnfg1(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;

        default:
            hx_state = HX_UC_ERROR_INTERNAL_PKT;
            rxBuffer[0] = HX_DUT_NACK;
            break;
    } // end switch-case
}

void write_ASLBoron_cstm_cnfg(uint8_t* cnfg_arr, uint16_t cnfg_arr_size, uint8_t* rxBuffer)
{
    // WRITE STATE MACHINE FOR BUILDING THE PACKETS TO CONFIGURE THE BORON CHIP
}

void write_ASLBoron_debug(uint8_t run_test, uint8_t run_cnfg, uint8_t* rxBuffer)
{
    switch(run_test)
    {
        case 0x00:
            cycleBoronPower();
            break;
            
        case 0x01:
            ASLB_debug0(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
        
        case 0x02:
            ASLB_debug1(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x03:
            ASLB_debug2(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x04:
            ASLB_debug3(run_cnfg, &rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x05:
            resetSFRData();
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x06:
            resetLATCHData();
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x07:
            setCommSingleWire();
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x08:
            setCommI2C();
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
        case 0x09:
            findMaxI2CSpeed(&rxBuffer[1]);
            rxBuffer[0] = HX_DUT_COMPLETE;
            break;
            
//        case 0xBB:
//            ARDUINO_test_debug(&rxBuffer[1]);
//            rxBuffer[0] = HX_DUT_COMPLETE;
//            break;
            
        default:
            hx_state = HX_UC_ERROR_INTERNAL_PKT;
            rxBuffer[0] = HX_DUT_NACK;
            break;
    }
}

void ASLB_ContinuityESD(uint8_t* rxBuffer)
{
    
}

void ASLB_SheetResistance(uint8_t* rxBuffer)
{
    /*
     * Meaure through HTRFB. Sheet resistance stored in the OTP for Vectron to read
     * 
    1. Disable internal heater opamp
    2. Select internal heater
    3. Enable read sheet resistance may need to ensure ADC mux not fed to pin)
    4. Resistance to ground on HTRFB pin is 10K (plus FET). Select reasonable voltage range and measure resistance.
    5. Measure and save resistance for writing to OTP
    6. de-select Resistance read in commands
     * 
     * // Disable Temperature control DAC OPAMP
    LATCH_OVEN_DATA &= 0xFEFF; // bit 8 => 0
    
    // Enable internal Heater
    LATCH_OVEN_DATA |= 0x0080; // bit 7 => 1
     */
    
    uint8_t rx_index = 0;
    
    // Fill in the latch data to the **test specific latch data** (tsld) struct
    tsld.LATCH_OSC0_DATA         = 0x0000;
    tsld.LATCH_OSC1_DATA         = 0x0000;
    tsld.LATCH_OSC2_DATA         = 0x0000;
    tsld.LATCH_ANALOG_DATA       = 0x0000;
    tsld.LATCH_OVEN_DATA         = 0x0000;
    tsld.LATCH_HEAT_DAC_DATA     = 0x0000;
    tsld.LATCH_VARACTOR_DAC_DATA = 0x0000;
    tsld.LATCH_BIAS_DATA         = 0x0000;
    tsld.LATCH_CMOS_DATA         = 0x0000;
    tsld.LATCH_ADC_DATA          = 0x0000;
    tsld.LATCH_ADC_MUX_DATA      = 0x0000;
    tsld.LATCH_OCXO_DIVISOR_DATA = 0x0000;
    tsld.LATCH_CLOCK_SELECT_DATA = 0x0000;
    
    if (!checkAndSendLatchData(rxBuffer, &rx_index))
    {
        // error occured (and was loaded into rxBuffer)
        return;
    }
    
    // Write the rest of the test code here
    
    // final index addition
    hx_mainDataIndex += rx_index;
    
}

void ASLB_HeaterFunctionality(uint8_t* rxBuffer)
{
    /*
     * Measure supply current of heaters. Need to trick oven controller into 
     * driving the drive node to 3.3V - drive therm_int
    1. Enable internal heater
    2. Configure current limiter (DAC1)
    3. Enable & configure oven controller
    4. Configure temp set DAC (DAC0)
    5. Measure current through VDDH(1&2) & voltage at HTRFB
     */
    
    uint8_t rx_index = 0;
    
    // Enable internal Heater
    LATCH_OVEN_DATA |= 0x0040; // bit 7 => 1
    
    // Configure current limiter DAC1
    LATCH_OVEN_DATA |= 0xA000; // set 1's
    LATCH_OVEN_DATA &= 0xAFFF; // set 0's
    
    // Enable oven controller
    LATCH_OVEN_DATA |= 0x0004; // bit 3 => 1
    
    // Config oven controller (feedback gain)
    LATCH_OVEN_DATA |= 0x0003; // set 1's
    LATCH_OVEN_DATA &= 0xFFF3; // set 0's
    
    // Config temp set DAC0
    LATCH_HEAT_DAC_DATA = 0xFAAA; // 12 bit dac, use the lower 12 bits
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
    txRxPkt ovenWrPkt;
    setupLatchPkt(&ovenWrPkt, LATCH_OVEN_DATA, LATCH_OVEN);
    txRxPkt heatDacWrPkt;
    setupLatchPkt(&heatDacWrPkt, LATCH_HEAT_DAC_DATA, LATCH_HEAT_DAC);
    
    // BUILD TRANSMISSION REQUEST BLOCK AND I2C STREAM
    I2C1_TRANSACTION_REQUEST_BLOCK trBlock[2];
    txRxPkt* stream[2];
    uint8_t stream_status = 0;
    stream[0] = &ovenWrPkt;
    stream[1] = &cmdReadbackPkt;
    stream_status = streamBoronComm(BORON_COMM_TYPE, stream, 2);
    
    /*
    if (stream_status != I2C_COMM_FAILURE)
    {
        memcpy(&rxBuffer[rx_index], cmdReadbackPkt.data, CMD_READBACK_ARR_SIZE);
        rx_index += CMD_READBACK_ARR_SIZE;
        hx_mainDataIndex += rx_index;
    }
    else
    {
        rxBuffer[rx_index++] = HX_UC_ERROR_DUT_COMM;
        rxBuffer[rx_index++] = 0xEE;
        hx_mainDataIndex += rx_index;
    }
    */
}

void ASLB_AnalogMuxReadingSetup(uint8_t* rxBuffer)
{
    /*
     * Not a test. Common setup function
    1. Disable internal heater opamp
    2. Select internal heater
    3. Disable read sheet resistance (may not be needed, already in that state?)
     */
    
    uint8_t rx_index = 0;
    
    // Disable Temperature control DAC OPAMP
    LATCH_OVEN_DATA &= 0xF7FF; // bit 8 => 0
    
    // Enable internal Heater
    LATCH_OVEN_DATA |= 0x0040; // bit 7 => 1
    
    // Disable read sheet resistance
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    txRxPkt ovenWrPkt;
    setupLatchPkt(&ovenWrPkt, LATCH_OVEN_DATA, LATCH_OVEN);
    
    // BUILD TRANSMISSION REQUEST BLOCK AND I2C STREAM
    txRxPkt* stream[2];
    uint8_t stream_status = 0;
    stream[0] = &ovenWrPkt;
    stream[1] = &cmdReadbackPkt;   
    stream_status = streamBoronComm(BORON_COMM_TYPE, stream, 2);
}

void ASLB_CTHERM(uint8_t* rxBuffer)
{
    /*
     * Measure through HTRFB
     1. Select mux channel for CTHERM
    2. Set VDD/2 on CTHERM
    3. Measure voltage on HTRFB (Vdd/2)
    4. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for CTHERM
    LATCH_ADC_DATA |= 0x0000; // CTHERM
    LATCH_ADC_DATA &= 0xFFF0; // CTHERM
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
}

void ASLB_ADCReferenceVoltage(uint8_t* rxBuffer)
{
    /*
     * Measure through HTRFB
     1. Select mux channel for ADC Vref - make sure ADC reference is on
    2. Read voltage on HTRFB (2.9V)
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for ADC Vref
    LATCH_ADC_DATA |= 0x0001; // ADC Vref
    LATCH_ADC_DATA &= 0xFFF1; // ADC Vref
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
    
}

void ASLB_HeaterFBSignals(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for Heater 1 Internal Oven Feedback 
    // (Main feedback from Heater 1 sense resistor)
    LATCH_ADC_DATA |= 0x0003; // Heater 1 FB
    LATCH_ADC_DATA &= 0xFFF3; // Heater 1 FB
    
    // Select mux channel for Heater 2 Internal Oven Feedback 
    // (Matching terminal of Heater 2 sense resistor)
    LATCH_ADC_DATA |= 0x0004; // Heater 2 FB
    LATCH_ADC_DATA &= 0xFFF4; // Heater 2 FB
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_InternalTempSensor(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for Internal Temp Sensor
    2. Read voltage on HTRFB
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for Internal Temp Sensor
    LATCH_ADC_DATA |= 0x0005; // Internal Temp Sensor
    LATCH_ADC_DATA &= 0xFFF5; // Internal Temp Sensor
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
    
}

void ASLB_MuxReadingVDD(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for Vdd
    2. Read voltage on HTRFB (Vdd/2)
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for VDD (VDD/2)
    LATCH_ADC_DATA |= 0x0008; // VDD/2
    LATCH_ADC_DATA &= 0xFFF8; // VDD/2
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_DACReferenceVoltage(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for DAC Vref - make sure DAC refenence is on
    2. Read voltage on HTRFB (2.9V/2)
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for DAC VREF
    LATCH_ADC_DATA |= 0x0009; // DAC VREF
    LATCH_ADC_DATA &= 0xFFF9; // DAC VREF
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_VaricapDACVoltage(uint8_t* rxBuffer)
{
    /*
     1. Setup Varicap DAC (routine/function?), output level TBD - make sure DAC reference & driver amp is on
    2. Select mux channel for Varicap DAC
    3. Read voltage on HTRFB
    4. Repeat for a few different DAC codes
    5. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for DAC VREF
    LATCH_ADC_DATA |= 0x0009; // DAC VREF
    LATCH_ADC_DATA &= 0xFFF9; // DAC VREF
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_TempSetDACVoltage(uint8_t* rxBuffer)
{
    /*
     1. Setup Temperature DAC (routine/function?), output level TBD - make sure DAC reference & driver amp is on
    2. Select mux channel for Temperature DAC
    3. Read voltage on HTRFB
    4. Repeat for a few different DAC codes
    5. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
    // 819 = 4095 / 5; 5 was chosen completely arbitrarily.
    for (int i = 0; i < 4096; i += 819)
    {
        if (!setTempCtrlDAC(i))
        {
            // error in parameter given
            break;
        }
        
        // Select mux channel for Temp Set DAC Voltage
        LATCH_ADC_DATA |= 0x000B; // Temp Set DAC Voltage
        LATCH_ADC_DATA &= 0xFFFB; // Temp set DAC Voltage
        
        // read the voltage
    }
    
    
    
    
}

void ASLB_CurrentLimitDACVoltage(uint8_t* rxBuffer)
{
    /*
     1. Setup Current Limit DAC (routine/function?), output level TBD - make sure DAC reference is on
    2. Select mux channel for Temperature DAC 
    3. Read voltage on HTRFB
    4. Repeat for a few different DAC codes
    5. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
    // 25.4 = 127/5
    for (int i = 0; i < 128; i += 25)
    {
        if (!setCurrLimitDAC(i))
        {
            // error with parameter given
            break;
        }
        
        // Select mux channel for Current Limit DAC
        LATCH_ADC_DATA |= 0x000B; // Current Limit DAC
        LATCH_ADC_DATA &= 0xFFFB; // Current Limit DAC
        
    }
    
}

void ASLB_DigitalRegulatorVoltage(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for digital regulator
    2. Read voltage on HTRFB (Digital Voltage
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for digital regulator (1.8V)
    LATCH_ADC_DATA |= 0x000D; // digital regulator 
    LATCH_ADC_DATA &= 0xFFFD; // digital regulator 
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_ZTCReferenceCurrent(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for ZTC Reference
    2. Read 1 ua out of HTRFB pin or place resistor to ground to give an effective voltage at 1 ua current
    3. Don't change any settings wait for another mux channel select
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for ZTC
    LATCH_ADC_DATA |= 0x000E; // ZTC
    LATCH_ADC_DATA &= 0xFFFE; // ZTC
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
    
}

void ASLB_MuxOpenCircuit(uint8_t* rxBuffer)
{
    /*
     1. Select mux channel for open circuit
    2. Read "0" V at HTRFB pin
    3. Finish mux select routines as needed.
     */
    
    uint8_t rx_index = 0;
    
    // Select mux channel for OPEN
    LATCH_ADC_DATA |= 0x000F; // OPEN
    LATCH_ADC_DATA &= 0xFFFF; // OPEN
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_OscillatorRegulatorVoltage(uint8_t* rxBuffer)
{
    /*
     1. Turn on oscillator Regulator
    2. Measure voltage (2.9V) on BYPASS pin
     */
    
    uint8_t rx_index = 0;
    
    // enable oscillator (2.9v OSC/BYPASS regulator)
    LATCH_BIAS_DATA |= 0x0004; // bit 4
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_ExternalReferenceVoltage(uint8_t* rxBuffer)
{
    /*
     1. Turn on external regulator
    2. Measure voltage (2.8V for 3.3V version, 4.0V for 5.0V version)
     */
    
    uint8_t rx_index = 0;
    
    // enable external regulator
    LATCH_BIAS_DATA |= 0x0010; // bit 5
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_VaricapBiasSteering(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
    txRxPkt cmdReadbackPkt;
    setupCmdReadbackPkt(&cmdReadbackPkt);
}

void ASLB_ColpittsOscillator(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_PierceOscillator(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_RDResistor(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_CFCVVaricap(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_CommonGate(uint8_t* rxBuffer)
{
    /*
     
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_ADC(uint8_t* rxBuffer)
{
    /*
     1. Disable internal heater opamp
    2. Select internal heater
    3. Disable read sheet resistance (may not be needed, already in that state?)
    4. Select mux channel 0Fh so mux is in a de-selected state
    5. Set HTRFB to the desired test voltage
    6. Start A/D convert, wait for interrupt
    7. Read data from Latches
    8. Repeat from step 5 on as needed
     */
    
    uint8_t rx_index = 0;
    
    
}

void ASLB_cnfg0(uint8_t* rxBuffer)
{
    
}

void ASLB_cnfg1(uint8_t* rxBuffer)
{
    
}

void ASLB_debug0(uint8_t* rxBuffer)
{
    ////////////////////////////////////////////////////////////////////
    // just make sure that data can be sent back from this state machine
    // nothing is being sent to the DUT
    ////////////////////////////////////////////////////////////////////
    
    uint8_t rx_index = 0;
    rxBuffer[rx_index++] = 0xAA;
    hx_mainDataIndex += rx_index;
}

void ASLB_debug1(uint8_t* rxBuffer)
{
    //////////////////////////////////////////////////
    // Write and readback data to Boron's RAM register
    //////////////////////////////////////////////////
    
    uint8_t rx_index = 0;
    uint8_t stream_status = 0;
    uint8_t ramAddrHi = 0x00;
    uint8_t ramAddrLo = 0x31;
    uint8_t ramData = 0xBB;
    
    txRxPkt ramRdPkt;
    txRxPkt cmdReadbackPkt;
    txRxPkt ramReadbackPkt;
    
    // RAM WRITE PACKET
    txRxPkt ramWrPkt;
    ramWrPkt.txOrRx = TX_PACKET_TYPE;
    ramWrPkt.slaveAddr = BORON_SLAVE_ADDR;
    ramWrPkt.numOfBytes = 6;
    ramWrPkt.data[0] = BORON_WRITE_RAM;
    ramWrPkt.data[1] = ramAddrLo;
    ramWrPkt.data[2] = ramAddrHi; 
    ramWrPkt.data[3] = 1;
    ramWrPkt.data[4] = ramData;
    ramWrPkt.data[5] = calcChecksum(ramWrPkt.data, ramWrPkt.numOfBytes-1);
    
    // RAM WRITE READBACK PACKET 
    cmdReadbackPkt.txOrRx = RX_PACKET_TYPE;
    cmdReadbackPkt.slaveAddr = BORON_SLAVE_ADDR;
    cmdReadbackPkt.numOfBytes = 3;
    
    // RAM READ PACKET
    ramRdPkt.txOrRx = TX_PACKET_TYPE;
    ramRdPkt.slaveAddr = BORON_SLAVE_ADDR;
    ramRdPkt.numOfBytes = 5;
    ramRdPkt.data[0] = BORON_READ_RAM;
    ramRdPkt.data[1] = ramAddrLo;
    ramRdPkt.data[2] = ramAddrHi;
    ramRdPkt.data[3] = 1;
    ramRdPkt.data[4] = calcChecksum(ramRdPkt.data, ramRdPkt.numOfBytes-1);
    
    // RAM READ READBACK PACKET
    ramReadbackPkt.txOrRx = RX_PACKET_TYPE;
    ramReadbackPkt.slaveAddr = BORON_SLAVE_ADDR;
    ramReadbackPkt.numOfBytes = CMD_READBACK_ARR_SIZE;
    
    // BUILD I2C STREAM
    txRxPkt* stream[4];
    stream[0] = &ramWrPkt;
    stream[1] = &cmdReadbackPkt;
    stream[2] = &ramRdPkt;
    stream[3] = &ramReadbackPkt;    
    
    stream_status = streamBoronComm(BORON_COMM_TYPE ,stream, 4);
    
    if (stream_status != I2C_COMM_FAILURE)
    {
        memcpy(&rxBuffer[rx_index], cmdReadbackPkt.data, CMD_READBACK_ARR_SIZE);
        rx_index += CMD_READBACK_ARR_SIZE;
        memcpy(&rxBuffer[rx_index], ramReadbackPkt.data, CMD_READBACK_ARR_SIZE);
        rx_index += CMD_READBACK_ARR_SIZE;
        hx_mainDataIndex += rx_index;
        
//        rxBuffer[rx_index++] = cmdReadbackPkt.data[1];
//        rxBuffer[rx_index++] = ramReadbackPkt.data[1];
//        hx_mainDataIndex += rx_index;
        
//        rxBuffer[rx_index++] = 0xEE;
//        hx_mainDataIndex += rx_index;
    }
    else
    {
        rxBuffer[rx_index++] = HX_UC_ERROR_DUT_COMM;
        rxBuffer[rx_index++] = 0xEE;
        hx_mainDataIndex += rx_index;
    }
}

void ASLB_debug2(uint8_t* rxBuffer)
{
    /////////////////////////////////////////
    // write a bunch of zeros to the i2c line
    /////////////////////////////////////////
    
    uint8_t rx_index = 0;
    uint8_t wr_status;
    uint8_t rd_status;
    uint8_t zeros[10] = {0};
    wr_status = writeI2C(zeros, 10, 0x00);  
}

void ASLB_debug3(uint8_t dataByte, uint8_t * rxBuffer)
{
    ////////////////////////////////////////
    // Adjust the baud rate of the I2C block
    ////////////////////////////////////////
    
    uint8_t rx_index = 0;
    if (!adjustI2CBaudRate(dataByte)) // did not write, baud out of range
    {
        rxBuffer[rx_index++] = HX_DUT_NACK;
        rxBuffer[rx_index++] = dataByte;
    }
    else
    {
        rxBuffer[rx_index++] = HX_DUT_ACK;
        rxBuffer[rx_index++] = dataByte;
    }
    hx_mainDataIndex += rx_index;
    
}

void setCommSingleWire()
{
    BORON_COMM_TYPE = BORON_COMM_SW;
}

void setCommI2C()
{
    BORON_COMM_TYPE = BORON_COMM_I2C;
}

void findMaxI2CSpeed(uint8_t * rxBuffer)
{
    uint8_t rx_index = 0;
    
    uint8_t ramAddrHi = 0x00;
    uint8_t ramAddrLo = 0x31;
    uint8_t ramData = 0xBB;
    
    uint16_t bRate = 0;
    // RAM WRITE PACKET
    txRxPkt ramWrPkt;
    ramWrPkt.txOrRx = TX_PACKET_TYPE;
    ramWrPkt.slaveAddr = BORON_SLAVE_ADDR;
    ramWrPkt.numOfBytes = 6;
    ramWrPkt.data[0] = BORON_WRITE_RAM;
    ramWrPkt.data[1] = ramAddrLo;
    ramWrPkt.data[2] = ramAddrHi; 
    ramWrPkt.data[3] = 1;
    ramWrPkt.data[4] = ramData;
    ramWrPkt.data[5] = calcChecksum(ramWrPkt.data, ramWrPkt.numOfBytes-1);
    
    // RAM WRITE READBACK PACKET 
    txRxPkt cmdReadbackPkt;
    cmdReadbackPkt.txOrRx = RX_PACKET_TYPE;
    cmdReadbackPkt.slaveAddr = BORON_SLAVE_ADDR;
    cmdReadbackPkt.numOfBytes = 3;
    
    txRxPkt* stream[2];
    stream[0] = &ramWrPkt;
    stream[1] = &cmdReadbackPkt;
    
    
    for (uint8_t i = 0xFF; i >= 0x03; i--)
    {
        if (!adjustI2CBaudRate(i))
        {
            break;
        }
        
        // stream the packet (check for error in transmission)
        if (streamI2C(stream, 2) == I2C1_MESSAGE_FAIL)
        {
            // failed communication
            bRate = calcBaudRate(i);
            rxBuffer[rx_index++] = HX_UC_ERROR_DUT_COMM;
            rxBuffer[rx_index++] = i;
            rxBuffer[rx_index++] = (bRate >> 8) & 0xFF;
            rxBuffer[rx_index++] = bRate & 0xFF;
            adjustI2CBaudRate(DEFAULT_I2C_SPEED_VAL);
            break;
        }
        
        if (ramData == cmdReadbackPkt.data[1])
        {
            // Good
            continue;
        }
        else
        {
            // Bad - still communicating, but mangled the return data
            rxBuffer[rx_index++] = HX_UC_ACK;
            rxBuffer[rx_index++] = i;
        }
    }
    
    hx_mainDataIndex += rx_index;
}

uint16_t calcBaudRate(uint8_t bd)
{
    uint16_t bRate = HX_CHIP_FREQUENCY / (4 * (bd + 1));
    return bRate;    
}

// send and receive data to and from an arduino via i2c
//void ARDUINO_test_debug(uint8_t * rxBuffer)
//{
//    uint8_t * ardDebugVal;
//    *ardDebugVal = 0x30;
//    /* ***************************************
//    * Test I2C Master transmit
//    * ***************************************/
//    if (*ardDebugVal < 0x30 || *ardDebugVal >= 0x39)
//    {
//        *ardDebugVal = 0x30;
//    }
//    writeI2C(ardDebugVal, 1, 0x2A);
//    //__delay_ms(500);
//    __delay_ms(50);
//    /* ***************************************
//    * Test I2C Master request
//    * ***************************************/
//    readI2C(ardDebugVal, 1, 0x2A);
//    //__delay_ms(500);
//    __delay_ms(50);
//    
//    rxBuffer[0] = *ardDebugVal;
//    hx_mainDataIndex++;
//}

void cycleBoronPower()
{
    BORON_POWER_PIN = 1;
    __delay_ms(1);
    BORON_POWER_PIN = 0;
}

uint8_t adjustI2CBaudRate(uint8_t bd)
{
    // SSP1ADD
    // Change baud rate from 0x03 to 0xFF
    // Currently at 0x9F (159) (with 64MHz clock to get 100KHz baud)
    // The number is a divider, as you increase the value
    // the baud rate goes DOWN
    if (bd < 0x03)
    {
        SSP1ADD = DEFAULT_I2C_SPEED_VAL;
        return 0;
    }
    
    SSP1ADD = bd;
    return 1;
    
}

uint8_t setTempCtrlDAC(uint16_t value12bit)
{
    if (value12bit > 4095)
    {
        return 0;
    }
    
    
    // make sure DAC reference & driver amp is on
    LATCH_OVEN_DATA |= 0x0040; // enable internal oven controller
    LATCH_OVEN_DATA |= 0x0080; // enable driver amp
    
    // Enable DAC bias
    LATCH_BIAS_DATA |= 0x0001;
    
    
    LATCH_HEAT_DAC_DATA = value12bit;
    
    // FINISH ACTUALLY SETTING THE DAC VALUE ON THE BORON CHIP
    
    return 1;
}

uint8_t setCurrLimitDAC(uint16_t value7bit)
{
    if (value7bit > 127)
    {
        return 0;
    }
    
    // Enable DAC bias
    LATCH_BIAS_DATA |= 0x0001;
    
    // Set the current limiter DAC value (top seven bits on oven_latch)
    LATCH_OVEN_DATA |= (value7bit << 9);
    LATCH_OVEN_DATA &= (value7bit << 9) | 0x01FF;
    
    // FINISH ACTUALLY SETTING THE DAC VALUE ON THE BORON CHIP
    
    return 1;
}

void setupLatchPkt(txRxPkt* pkt, uint16_t latch_data, uint8_t latch_num)
{
    // The following is the procedure for every latch write operation
    
    pkt->txOrRx = TX_PACKET_TYPE;
    pkt->slaveAddr = BORON_SLAVE_ADDR;
    pkt->numOfBytes = 5;
    pkt->data[0] = BORON_WRITE_LATCH;
    pkt->data[1] = (latch_data >> 8) & 0xFF;
    pkt->data[2] = latch_data & 0xFF;
    pkt->data[3] = latch_num;
    pkt->data[4] = calcChecksum(pkt->data, pkt->numOfBytes-1);
}

void setupCmdReadbackPkt(txRxPkt* pkt)
{
    // The following is the procedure for every command readback operation
    pkt->txOrRx = RX_PACKET_TYPE;
    pkt->slaveAddr = BORON_SLAVE_ADDR;
    pkt->numOfBytes = CMD_READBACK_ARR_SIZE;
}

uint8_t setupLrgWrPkt(txRxPkt* pkt, uint16_t addr, uint8_t boronCmd, 
        uint8_t* data, uint8_t size)
{
    // 0=error; 
    // 1=good; 
    // n=index of remaining data that couldn't be TX'd
    uint8_t status = 0;
    uint8_t pktIndex = 0;
    
    if (size > TX_RX_PACKET_SIZE)
    {
        status = TX_RX_PACKET_SIZE;
        size = TX_RX_PACKET_SIZE;
    }
    else
    {
        status = 1;
    }
    
    pkt->txOrRx = TX_PACKET_TYPE;
    pkt->slaveAddr = BORON_SLAVE_ADDR;
    pkt->data[pktIndex++] = boronCmd;
    pkt->data[pktIndex++] = addr & 0xFF; //(latch_data >> 8) & 0xFF;
    pkt->data[pktIndex++] = (addr >> 8) & 0xFF;
    for (uint8_t i=0; i < size; i++)
    {
        pkt->data[pktIndex++] = data[i];
    }
    pkt->data[pktIndex++] = calcChecksum(pkt->data, pktIndex);
    pkt->numOfBytes = pktIndex;
    
    return status;
}

uint8_t checkAndSendLatchData(uint8_t* rxBuffer, uint8_t* rx_index)
{
    // This function will check each global latch data variable
    // against the data filled into the 
    // **test specific latch data** (tsld) struct
    // If the data is different, then a packet will be created and
    // streamed to BORON, otherwise the latch data is left alone     
    
    uint8_t noError = 0;
    uint8_t latchSendCount = 0;
    uint8_t readbackSendCount = 0;
    
    txRxPkt* stream[_LATCH_STREAM_SIZE];
    txRxPkt* readbackPkts[_LATCH_COUNT];
    uint8_t stream_status;
    
    
    
    // Compare global latch data variable to that in the tsld-struct
    if (!(LATCH_OSC0_DATA & tsld.LATCH_OSC0_DATA))
    {
        // set the global data equal to the new data (from the struct)
        LATCH_OSC0_DATA = tsld.LATCH_OSC0_DATA;
        
        // create a corresponding latch packet
        txRxPkt osc0_pkt;
        
        // setup the latch
        setupLatchPkt(&osc0_pkt, LATCH_OSC0_DATA, LATCH_OSC0);
        
        // add this latch packet to the stream to be transmitted
        stream[latchSendCount++] = &osc0_pkt;
        
        // do the same as above for the readback packet
        txRxPkt readback_osc0_pkt;
        setupCmdReadbackPkt(&readback_osc0_pkt);
        stream[latchSendCount++] = &readback_osc0_pkt;
        
        // add readback packet to this array to filter responses later
        readbackPkts[readbackSendCount++] = &readback_osc0_pkt;
    }
    
    if (!(LATCH_OSC1_DATA & tsld.LATCH_OSC1_DATA))
    {
        LATCH_OSC1_DATA = tsld.LATCH_OSC1_DATA;
        txRxPkt osc1_pkt;
        setupLatchPkt(&osc1_pkt, LATCH_OSC1_DATA, LATCH_OSC1);
        stream[latchSendCount++] = &osc1_pkt;
        txRxPkt readback_osc1_pkt;
        setupCmdReadbackPkt(&readback_osc1_pkt);
        stream[latchSendCount++] = &readback_osc1_pkt;
        readbackPkts[readbackSendCount++] = &readback_osc1_pkt;
    }
    
    if (!(LATCH_OSC2_DATA & tsld.LATCH_OSC2_DATA))
    {
        LATCH_OSC2_DATA = tsld.LATCH_OSC2_DATA;
        txRxPkt osc2_pkt;
        setupLatchPkt(&osc2_pkt, LATCH_OSC2_DATA, LATCH_OSC2);
        stream[latchSendCount++] = &osc2_pkt;
        txRxPkt readback_osc2_pkt;
        setupCmdReadbackPkt(&readback_osc2_pkt);
        stream[latchSendCount++] = &readback_osc2_pkt;
        readbackPkts[readbackSendCount++] = &readback_osc2_pkt;
    }
    
    if (!(LATCH_ANALOG_DATA & tsld.LATCH_ANALOG_DATA))
    {
        LATCH_ANALOG_DATA = tsld.LATCH_ANALOG_DATA;
        txRxPkt analog_pkt;
        setupLatchPkt(&analog_pkt, LATCH_ANALOG_DATA, LATCH_ANALOG);
        stream[latchSendCount++] = &analog_pkt;
        txRxPkt readback_analog_pkt;
        setupCmdReadbackPkt(&readback_analog_pkt);
        stream[latchSendCount++] = &readback_analog_pkt;
        readbackPkts[readbackSendCount++] = &readback_analog_pkt;
    }
    
    if (!(LATCH_OVEN_DATA & tsld.LATCH_OVEN_DATA))
    {
        LATCH_OVEN_DATA = tsld.LATCH_OVEN_DATA;
        txRxPkt oven_pkt;
        setupLatchPkt(&oven_pkt, LATCH_OVEN_DATA, LATCH_OVEN);
        stream[latchSendCount++] = &oven_pkt;
        txRxPkt readback_oven_pkt;
        setupCmdReadbackPkt(&readback_oven_pkt);
        stream[latchSendCount++] = &readback_oven_pkt;
        readbackPkts[readbackSendCount++] = &readback_oven_pkt;
    }
    
    if (!(LATCH_HEAT_DAC_DATA & tsld.LATCH_HEAT_DAC_DATA))
    {
        LATCH_HEAT_DAC_DATA = tsld.LATCH_HEAT_DAC_DATA;
        txRxPkt heat_dac_pkt;
        setupLatchPkt(&heat_dac_pkt, LATCH_HEAT_DAC_DATA, LATCH_HEAT_DAC);
        stream[latchSendCount++] = &heat_dac_pkt;
        txRxPkt readback_heat_dac_pkt;
        setupCmdReadbackPkt(&readback_heat_dac_pkt);
        stream[latchSendCount++] = &readback_heat_dac_pkt;
        readbackPkts[readbackSendCount++] = &readback_heat_dac_pkt;
    }
    
    if (!(LATCH_VARACTOR_DAC_DATA & tsld.LATCH_VARACTOR_DAC_DATA))
    {
        LATCH_VARACTOR_DAC_DATA = tsld.LATCH_VARACTOR_DAC_DATA;
        txRxPkt varactor_dac_pkt;
        setupLatchPkt(&varactor_dac_pkt, LATCH_VARACTOR_DAC_DATA, LATCH_VARACTOR_DAC);
        stream[latchSendCount++] = &varactor_dac_pkt;
        txRxPkt readback_varactor_dac_pkt;
        setupCmdReadbackPkt(&readback_varactor_dac_pkt);
        stream[latchSendCount++] = &readback_varactor_dac_pkt;
        readbackPkts[readbackSendCount++] = &readback_varactor_dac_pkt;
    }
    
    if (!(LATCH_BIAS_DATA & tsld.LATCH_BIAS_DATA))
    {
        LATCH_BIAS_DATA = tsld.LATCH_BIAS_DATA;
        txRxPkt bias_pkt;
        setupLatchPkt(&bias_pkt, LATCH_BIAS_DATA, LATCH_BIAS);
        stream[latchSendCount++] = &bias_pkt;
        txRxPkt readback_bias_pkt;
        setupCmdReadbackPkt(&readback_bias_pkt);
        stream[latchSendCount++] = &readback_bias_pkt;
        readbackPkts[readbackSendCount++] = &readback_bias_pkt;
    }
    
    if (!(LATCH_CMOS_DATA & tsld.LATCH_CMOS_DATA))
    {
        LATCH_CMOS_DATA = tsld.LATCH_CMOS_DATA;
        txRxPkt cmos_pkt;
        setupLatchPkt(&cmos_pkt, LATCH_CMOS_DATA, LATCH_CMOS);
        stream[latchSendCount++] = &cmos_pkt;
        txRxPkt readback_cmos_pkt;
        setupCmdReadbackPkt(&readback_cmos_pkt);
        stream[latchSendCount++] = &readback_cmos_pkt;
        readbackPkts[readbackSendCount++] = &readback_cmos_pkt;
    }
    
    if (!(LATCH_ADC_DATA & tsld.LATCH_ADC_DATA))
    {
        LATCH_ADC_DATA = tsld.LATCH_ADC_DATA;
        txRxPkt adc_pkt;
        setupLatchPkt(&adc_pkt, LATCH_ADC_DATA, LATCH_ADC);
        stream[latchSendCount++] = &adc_pkt;
        txRxPkt readback_adc_pkt;
        setupCmdReadbackPkt(&readback_adc_pkt);
        stream[latchSendCount++] = &readback_adc_pkt;
        readbackPkts[readbackSendCount++] = &readback_adc_pkt;
    }
    
    if (!(LATCH_ADC_MUX_DATA & tsld.LATCH_ADC_MUX_DATA))
    {
        LATCH_ADC_MUX_DATA = tsld.LATCH_ADC_MUX_DATA;
        txRxPkt adc_mux_pkt;
        setupLatchPkt(&adc_mux_pkt, LATCH_ADC_MUX_DATA, LATCH_ADC_MUX);
        stream[latchSendCount++] = &adc_mux_pkt;
        txRxPkt readback_adc_mux_pkt;
        setupCmdReadbackPkt(&readback_adc_mux_pkt);
        stream[latchSendCount++] = &readback_adc_mux_pkt;
        readbackPkts[readbackSendCount++] = &readback_adc_mux_pkt;
    }
    
    if (!(LATCH_OCXO_DIVISOR_DATA & tsld.LATCH_OCXO_DIVISOR_DATA))
    {
        LATCH_OCXO_DIVISOR_DATA = tsld.LATCH_OCXO_DIVISOR_DATA;
        txRxPkt ocxo_divisor_pkt;
        setupLatchPkt(&ocxo_divisor_pkt, LATCH_OCXO_DIVISOR_DATA, LATCH_OCXO_DIVISOR);
        stream[latchSendCount++] = &ocxo_divisor_pkt;
        txRxPkt readback_ocxo_divisor_pkt;
        setupCmdReadbackPkt(&readback_ocxo_divisor_pkt);
        stream[latchSendCount++] = &readback_ocxo_divisor_pkt;
        readbackPkts[readbackSendCount++] = &readback_ocxo_divisor_pkt;
    }
    
    if (!(LATCH_CLOCK_SELECT_DATA & tsld.LATCH_CLOCK_SELECT_DATA))
    {
        LATCH_CLOCK_SELECT_DATA = tsld.LATCH_CLOCK_SELECT_DATA;
        txRxPkt clock_select_pkt;
        setupLatchPkt(&clock_select_pkt, LATCH_CLOCK_SELECT_DATA, LATCH_CLOCK_SELECT_DATA);
        stream[latchSendCount++] = &clock_select_pkt;
        txRxPkt readback_clock_select_pkt;
        setupCmdReadbackPkt(&readback_clock_select_pkt);
        stream[latchSendCount++] = &readback_clock_select_pkt;
        readbackPkts[readbackSendCount++] = &readback_clock_select_pkt;
    }
    
    // send every latch packet and its corresponding readback packet
    stream_status = streamBoronComm(BORON_COMM_TYPE, stream, latchSendCount);
    
    // check for stream failure and fill in appropriate readback data or error
    if (stream_status != I2C_COMM_FAILURE)
    {
        //memcpy(&rxBuffer[*rx_index], cmdReadbackPkt.data, CMD_READBACK_ARR_SIZE);
        //*rx_index += CMD_READBACK_ARR_SIZE;
        noError = 1;
    }
    else
    {
        rxBuffer[*rx_index++] = HX_UC_ERROR_DUT_COMM;
        rxBuffer[*rx_index++] = 0xEE;
        noError = 0;
    }
    
    noError = filterLatchPacketResponses(rxBuffer, rx_index, readbackPkts, readbackSendCount);
    
    
    return noError;
    
}

uint8_t filterLatchPacketResponses(uint8_t* rxBuffer, uint8_t* rx_index, 
                                    txRxPkt** readbackPkts, uint8_t count)
{
    uint8_t possibleError = 0;
    for (int i = 0; i < count; i++)
    {
        possibleError = readbackPkts[i]->data[1];
        // If the value in the return packet (from boron) at position [1]
        // is an error, fill in the error into the rxBuffer
        if (possibleError == BORON_ERROR_DATA_BUFFER_BUSY ||
            possibleError == BORON_ERROR_COMM_NUM_BYTES_OVERFLOW ||
            possibleError == BORON_ERROR_CMD ||
            possibleError == BORON_ERROR_INCOMPLETE_OP ||
            possibleError == BORON_ERROR_CHECKSUM ||
            possibleError == BORON_ERROR_BYTE_COUNT ||
            possibleError == BORON_ERROR_PARAM_REWRITE ||
            possibleError == BORON_ERROR_PARAM_REWRITE)
        {
            rxBuffer[*rx_index++] = readbackPkts[i]->data[0];
            rxBuffer[*rx_index++] = readbackPkts[i]->data[1];
            rxBuffer[*rx_index++] = readbackPkts[i]->data[2];
            return 0;
        }
        
    }
    return 1;
}

void resetSFRData()
{
    SFR_P0_DATA = 0x00;
    SFR_P1_DATA = 0x00;
    SFR_OSC0_DATA = 0x00;
    SFR_OSC1_DATA = 0x00;
    SFR_OSC2_DATA = 0x00;
    SFR_ANALOG_DATA = 0x00;
    SFR_OVEN_DATA = 0x00;
    SFR_HEAT_DATA = 0x00;
    SFR_VAR_DATA = 0x00;
    SFR_BIAS_DATA = 0x00;
    SFR_EXT_DATA = 0x00;
    SFR_CMOS_OUT_DATA = 0x00;
    SFR_OCXO_DIV_DATA = 0x00;
    SFR_CLK_SEL_DATA = 0x00;
    SFR_ADC_DIV_DATA = 0x00;
    SFR_ADC_MUX_DATA = 0x00;
    SFR_ASIC_REV_DATA = 0x00;
    SFR_OTP_MR_DATA = 0x00;
    SFR_OTP_MPP_DATA = 0x00;
    SFR_OTP_MRR_LO_DATA = 0x00;
    SFR_OTP_MRR_HI_DATA = 0x00;
    SFR_OTP_WR_ADDR_HIGH_DATA = 0x00;
    SFR_OTP_WR_ADDR_LOW_DATA = 0x00;
    SFR_OTP_WR_DATA_DATA = 0x00;
    SFR_OTP_DIV_100US_HIGH_DATA = 0x00;
    SFR_OTP_DIV_100US_LOW_DATA = 0x00;
    SFR_OTP_DIV_400US_HIGH_DATA = 0x00;
    SFR_OTP_DIV_400US_LOW_DATA = 0x00;
    SFR_OTP_ST_TIMING_DATA = 0x00;
    SFR_SW_100US_LOW_DATA = 0x00;
    SFR_SW_100US_HIGH_DATA = 0x00;
    SFR_SW_ST_DET_DATA = 0x00;
    SFR_SW_WR_BIT_DATA = 0x00;
    SFR_SW_BIT_INT_DATA = 0x00; 
    SFR_SW_WR_BYT_DATA = 0x00;
    SFR_SW_BYT_INT_DATA = 0x00;
    SFR_SW_DET_BIT_DATA = 0x00;
    SFR_SW_BIT_DATA_DATA = 0x00;
    SFR_SW_BIT_DATA_ST_DATA = 0x00;
    SFR_SW_STOP_DET_DATA = 0x00;
    SFR_SW_STOP_DET_ST_DATA = 0x00;
}

void resetLATCHData()
{
    LATCH_OSC0_DATA = 0x000B;
    LATCH_OSC1_DATA = 0x0000;
    LATCH_OSC2_DATA = 0x0000;
    LATCH_ANALOG_DATA = 0x0000;
    LATCH_OVEN_DATA = 0x0000;
    LATCH_HEAT_DAC_DATA = 0x0000;
    LATCH_VARACTOR_DAC_DATA = 0x0000;
    LATCH_BIAS_DATA = 0x0000;
    LATCH_CMOS_DATA = 0x0000;
    LATCH_ADC_DATA = 0x0000;
    LATCH_ADC_MUX_DATA = 0x0000;
    LATCH_OCXO_DIVISOR_DATA = 0x0000;
    LATCH_CLOCK_SELECT_DATA = 0x0000;
}

