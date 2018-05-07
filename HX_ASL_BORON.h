
#ifndef HX_ASL_BORON_H
#define	HX_ASL_BORON_H

#include "HX_CommStd.h"
#include "HX_DeviceProperties.h"
#include "Utils.h"
#include <xc.h>
#include "mcc_generated_files/i2c1.h"
#include <string.h>
#include <stdlib.h>



typedef struct 
{
    uint16_t LATCH_OSC0_DATA;
    uint16_t LATCH_OSC1_DATA;
    uint16_t LATCH_OSC2_DATA;
    uint16_t LATCH_ANALOG_DATA;
    uint16_t LATCH_OVEN_DATA;
    uint16_t LATCH_HEAT_DAC_DATA;
    uint16_t LATCH_VARACTOR_DAC_DATA;
    uint16_t LATCH_BIAS_DATA;
    uint16_t LATCH_CMOS_DATA;

    uint16_t LATCH_ADC_DATA;
    uint16_t LATCH_ADC_MUX_DATA;
    uint16_t LATCH_OCXO_DIVISOR_DATA;
    uint16_t LATCH_CLOCK_SELECT_DATA;
} testSpecificLatchData;

extern testSpecificLatchData tsld;

#define CNFG_ARR_SIZE 256

////////////////////
// BORON CHIP VARS
////////////////////
#define BORON_SLAVE_ADDR 0x5A
#define ARDUINO_SLAVE_ADDR 0x2A
#define CMD_READBACK_ARR_SIZE 3
#define BORON_POWER_PIN PORTBbits.RB2


/* READ COMMANDS */
#define BORON_READ_OTP              0x20
#define BORON_READ_DYNAMIC_PARAM    0x21
#define BORON_READ_RAM              0x22
#define BORON_READ_XDATA            0x23
#define BORON_READ_CODE             0x24
#define BORON_READ_STATIC_PARAM     0x25

/* WRITE COMMANDS */
#define BORON_WRITE_OTP             0x30
#define BORON_WRITE_DYNAMIC_PARAM   0x31
#define BORON_WRITE_RAM             0x32
#define BORON_WRITE_XDATA           0x33
#define BORON_WRITE_STATIC_PARAM    0x34

/* EXTENDED COMMANDS */
#define BORON_WRITE_SFR             0x3F
#define BORON_READ_SFR              0x40
#define BORON_WRITE_LATCH           0x50
#define BORON_READ_LATCH            0x51
#define BORON_START_ADC_CONV        0x52
#define BORON_READ_ADC_CONV         0x53
#define BORON_DYN_PARAM_PAGE_LOC    0x41
#define BORON_STA_PARAM_PAGE_LOC    0x42
#define BORON_NEXT_DYN_PARAM_PAGE   0x43
#define BORON_NEXT_STA_PARAM_PAGE   0x44
#define BORON_CURR_DYN_PARAM_PAGE   0x45
#define BORON_CURR_STA_PARAM_PAGE   0x46
#define BORON_SOFTWARE_RESET        0x47
#define BORON_TASK_ENABLE           0x48
#define BORON_TASK_DISABLE          0x49
#define BORON_RTOS_TOGGLE           0x4A

/* BORON ERRORS */
#define BORON_ERROR_DATA_BUFFER_BUSY        0x11
#define BORON_ERROR_COMM_NUM_BYTES_OVERFLOW 0x12
#define BORON_ERROR_CMD                     0x13
#define BORON_ERROR_INCOMPLETE_OP           0x14
#define BORON_ERROR_CHECKSUM                0x15
#define BORON_ERROR_BYTE_COUNT              0x16
#define BORON_ERROR_PARAM_REWRITE           0x21
#define BORON_ERROR_SINGLE_WIRE_STOP        0x22



/* TASK COMMANDS */

/* BORON COMM */
#define BORON_PKT_SIZE 16
uint8_t boronTxPktIndex = 0;
uint8_t boronRxPktIndex = 0;
uint8_t boronTxPkt[BORON_PKT_SIZE];
uint8_t boronRxPkt[BORON_PKT_SIZE];


////////////////////
// PROTOTYPES
////////////////////
void run_ASLBoron(uint8_t hx_inData[], uint8_t size);
void write_ASLBoron_test(uint8_t run_test, uint8_t* run_rpt, uint8_t *rxBuffer);
void write_ASLBoron_cnfg(uint8_t run_cnfg, uint8_t *rxBuffer);
void write_ASLBoron_cstm_cnfg(uint8_t* cnfg_arr, uint16_t cnfg_arr_size, uint8_t* rxBuffer);
void write_ASLBoron_debug(uint8_t run_test, uint8_t run_cnfg, uint8_t* rxBuffer);

////////////////////
// TESTS
////////////////////
void ASLB_ContinuityESD(uint8_t* rxBuffer);
void ASLB_SheetResistance(uint8_t* rxBuffer);
void ASLB_HeaterFunctionality(uint8_t* rxBuffer);
void ASLB_AnalogMuxReadingSetup(uint8_t* rxBuffer);
void ASLB_CTHERM(uint8_t* rxBuffer);
void ASLB_ADCReferenceVoltage(uint8_t* rxBuffer);
void ASLB_HeaterFBSignals(uint8_t* rxBuffer);
void ASLB_InternalTempSensor(uint8_t* rxBuffer);
void ASLB_MuxReadingVDD(uint8_t* rxBuffer);
void ASLB_DACReferenceVoltage(uint8_t* rxBuffer);
void ASLB_VaricapDACVoltage(uint8_t* rxBuffer);
void ASLB_TempSetDACVoltage(uint8_t* rxBuffer);
void ASLB_CurrentLimitDACVoltage(uint8_t* rxBuffer);
void ASLB_DigitalRegulatorVoltage(uint8_t* rxBuffer);
void ASLB_ZTCReferenceCurrent(uint8_t* rxBuffer);
void ASLB_MuxOpenCircuit(uint8_t* rxBuffer);
void ASLB_OscillatorRegulatorVoltage(uint8_t* rxBuffer);
void ASLB_ExternalReferenceVoltage(uint8_t* rxBuffer);
void ASLB_VaricapBiasSteering(uint8_t* rxBuffer);
void ASLB_ColpittsOscillator(uint8_t* rxBuffer);
void ASLB_PierceOscillator(uint8_t* rxBuffer);
void ASLB_RDResistor(uint8_t* rxBuffer);
void ASLB_CFCVVaricap(uint8_t* rxBuffer);
void ASLB_CommonGate(uint8_t* rxBuffer);
void ASLB_ADC(uint8_t* rxBuffer);

////////////////////
// CONFIG
////////////////////
void ASLB_cnfg0(uint8_t* rxBuffer);
void ASLB_cnfg1(uint8_t* rxBuffer);

////////////////////
// DEBUG TESTS
////////////////////
void ASLB_debug0(uint8_t* rxBuffer);
void ASLB_debug1(uint8_t* rxBuffer);
void ASLB_debug2(uint8_t* rxBuffer);
void ASLB_debug3(uint8_t dataByte, uint8_t * rxBuffer);
void setCommSingleWire();
void setCommI2C();

////////////////////
// UTILS
////////////////////
uint8_t adjustI2CBaudRate(uint8_t bd);
void resetSFRData();
void resetLATCHData();
void cycleBoronPower();
uint8_t setTempCtrlDAC(uint16_t value12bit);
uint8_t setCurrLimitDAC(uint16_t value7bit);
void setupLatchPkt(txRxPkt* pkt, uint16_t latch_data, uint8_t latch_num);
void setupCmdReadbackPkt(txRxPkt* pkt);
uint8_t checkAndSendLatchData(uint8_t* rxBuffer, uint8_t* rx_index);
uint8_t filterLatchPacketResponses(uint8_t* rxBuffer, uint8_t* rx_index, 
                                    txRxPkt** readbackPkts, uint8_t count);
void findMaxI2CSpeed(uint8_t * rxBuffer);
uint8_t setupLrgWrPkt(txRxPkt* pkt, uint16_t addr, uint8_t boronCmd, 
        uint8_t* data, uint8_t size);
uint16_t calcBaudRate(uint8_t bd);
////////////////////
// SFR
////////////////////

#define SFR_P0 0x00
#define SFR_P1 0x01
#define SFR_OSC0 0x02
#define SFR_OSC1 0x03
#define SFR_OSC2 0x04
#define SFR_ANALOG 0x05
#define SFR_OVEN 0x06
#define SFR_HEAT 0x07
#define SFR_VAR 0x08
#define SFR_BIAS 0x09
#define SFR_EXT 0x0A
#define SFR_CMOS_OUT 0x0B
#define SFR_OCXO_DIV 0x0C
#define SFR_CLK_SEL 0x0D
#define SFR_ADC_DIV 0x0E
#define SFR_ADC_MUX 0x0F
#define SFR_ASIC_REV 0x10
#define SFR_OTP_MR 0x11
#define SFR_OTP_MPP 0x12
#define SFR_OTP_MRR_LO 0x13
#define SFR_OTP_MRR_HI 0x14
#define SFR_OTP_WR_ADDR_HIGH 0x15
#define SFR_OTP_WR_ADDR_LOW 0x16
#define SFR_OTP_WR_DATA 0x17
#define SFR_OTP_DIV_100US_HIGH 0x18
#define SFR_OTP_DIV_100US_LOW 0x19
#define SFR_OTP_DIV_400US_HIGH 0x1A
#define SFR_OTP_DIV_400US_LOW 0x1B
#define SFR_OTP_ST_TIMING 0x1C
#define SFR_SW_100US_LOW 0x1D
#define SFR_SW_100US_HIGH 0x1E
#define SFR_SW_ST_DET 0x1F
#define SFR_SW_WR_BIT 0x20
#define SFR_SW_BIT_INT 0x21 
#define SFR_SW_WR_BYT 0x22
#define SFR_SW_BYT_INT 0x23
#define SFR_SW_DET_BIT 0x24
#define SFR_SW_BIT_DATA 0x25
#define SFR_SW_BIT_DATA_ST 0x26
#define SFR_SW_STOP_DET 0x27
#define SFR_SW_STOP_DET_ST 0x28

////////////////////
// SFR DATA
////////////////////

uint8_t SFR_P0_DATA = 0x00;
uint8_t SFR_P1_DATA = 0x00;
uint8_t SFR_OSC0_DATA = 0x00;
uint8_t SFR_OSC1_DATA = 0x00;
uint8_t SFR_OSC2_DATA = 0x00;
uint8_t SFR_ANALOG_DATA = 0x00;
uint8_t SFR_OVEN_DATA = 0x00;
uint8_t SFR_HEAT_DATA = 0x00;
uint8_t SFR_VAR_DATA = 0x00;
uint8_t SFR_BIAS_DATA = 0x00;
uint8_t SFR_EXT_DATA = 0x00;
uint8_t SFR_CMOS_OUT_DATA = 0x00;
uint8_t SFR_OCXO_DIV_DATA = 0x00;
uint8_t SFR_CLK_SEL_DATA = 0x00;
uint8_t SFR_ADC_DIV_DATA = 0x00;
uint8_t SFR_ADC_MUX_DATA = 0x00;
uint8_t SFR_ASIC_REV_DATA = 0x00;
uint8_t SFR_OTP_MR_DATA = 0x00;
uint8_t SFR_OTP_MPP_DATA = 0x00;
uint8_t SFR_OTP_MRR_LO_DATA = 0x00;
uint8_t SFR_OTP_MRR_HI_DATA = 0x00;
uint8_t SFR_OTP_WR_ADDR_HIGH_DATA = 0x00;
uint8_t SFR_OTP_WR_ADDR_LOW_DATA = 0x00;
uint8_t SFR_OTP_WR_DATA_DATA = 0x00;
uint8_t SFR_OTP_DIV_100US_HIGH_DATA = 0x00;
uint8_t SFR_OTP_DIV_100US_LOW_DATA = 0x00;
uint8_t SFR_OTP_DIV_400US_HIGH_DATA = 0x00;
uint8_t SFR_OTP_DIV_400US_LOW_DATA = 0x00;
uint8_t SFR_OTP_ST_TIMING_DATA = 0x00;
uint8_t SFR_SW_100US_LOW_DATA = 0x00;
uint8_t SFR_SW_100US_HIGH_DATA = 0x00;
uint8_t SFR_SW_ST_DET_DATA = 0x00;
uint8_t SFR_SW_WR_BIT_DATA = 0x00;
uint8_t SFR_SW_BIT_INT_DATA = 0x00; 
uint8_t SFR_SW_WR_BYT_DATA = 0x00;
uint8_t SFR_SW_BYT_INT_DATA = 0x00;
uint8_t SFR_SW_DET_BIT_DATA = 0x00;
uint8_t SFR_SW_BIT_DATA_DATA = 0x00;
uint8_t SFR_SW_BIT_DATA_ST_DATA = 0x00;
uint8_t SFR_SW_STOP_DET_DATA = 0x00;
uint8_t SFR_SW_STOP_DET_ST_DATA = 0x00;

////////////////////
// LATCHES
////////////////////

#define LATCH_OSC0 0x0
#define LATCH_OSC1 0x1
#define LATCH_OSC2 0x2
#define LATCH_ANALOG 0x3
#define LATCH_OVEN 0x4
#define LATCH_HEAT_DAC 0x5
#define LATCH_VARACTOR_DAC 0x6
#define LATCH_BIAS 0x7
#define LATCH_CMOS 0x8
#define LATCH_ADC 0x9
#define LATCH_ADC_MUX 0xA
#define LATCH_OCXO_DIVISOR 0xB
#define LATCH_CLOCK_SELECT 0xC

#define _LATCH_COUNT 12
#define _LATCH_STREAM_SIZE (2*_LATCH_COUNT)
////////////////////
// LATCH DATA
////////////////////

uint32_t LATCH_OSC0_DATA = 0x000B;//
uint32_t LATCH_OSC1_DATA = 0x0000;//
uint32_t LATCH_OSC2_DATA = 0x0000;//
uint32_t LATCH_ANALOG_DATA = 0x0000;//
uint32_t LATCH_OVEN_DATA = 0x0000;//
uint32_t LATCH_HEAT_DAC_DATA = 0x0000;//
uint32_t LATCH_VARACTOR_DAC_DATA = 0x0000;//
uint32_t LATCH_BIAS_DATA = 0x0000;//
uint32_t LATCH_CMOS_DATA = 0x0000;//

uint32_t LATCH_ADC_DATA = 0x0000;//
uint32_t LATCH_ADC_MUX_DATA = 0x0000;
uint32_t LATCH_OCXO_DIVISOR_DATA = 0x0000;
uint32_t LATCH_CLOCK_SELECT_DATA = 0x0000;

////////////////////
// ADC CHANNEL
////////////////////

/*
#define CHNL_CTHERM 0x0
#define CHNL_OPEN 0x1
#define CHNL_EXT_OVEN_FB 0x2
#define CHNL_INT_OVEN_FB1 0x3
#define CHNL_INT_OVEN_FB2 0x4
#define CHNL_TEMP_SENSE_PTAT 0x5
#define CHNL_OVEN_AMP_OUT 0x6
#define CHNL_EFC 0x7
#define CHNL_VDD3P3 0x8
#define CHNL_VREF_DAC 0x9
#define CHNL_VCAP_DAC 0xA
#define CHNL_TEMP_SET_DAC 0xB
#define CHNL_CURRENT_LIMIT_DAC 0xC
#define CHNL_VDD1P8 0xD
#define CHNL_ZTC_1UA 0xE
#define CHNL_OPEN 0xF
*/

////////////////////
// TASK NUMBER
////////////////////

#define TASK_INIT 0x0
#define TASK_FT 0x1
#define TASK_EXOVEN 0x2
#define TASK_INTOVEN 0x3
#define TASK_TEMPSEN 0x4
#define TASK_HEATDRI 0x5
#define TASK_EFC 0x7
#define TASK_FV 0x8
#define TASK_CS 0x9

/////////////////////////////
// INTERFACING WITH HX_USB
/////////////////////////////
/* <Assumes user is using the auto packet builder>
 CMD Byte:
 *  0x31 (HX_PKT_ASL_BORON)
 DATA Packet
 *  MODE  : 0xhh,
 *  TEST  : 0xhh,
 *  CONFIG{/NUM_OF_CONFIGS} :  0xhh,
 *  {CNFG_ADDR : 0xhh,
 *  CNFG_MSB   : 0xhh,
 *  CNFG_LSB   : 0xhh, ...}
 *  REPEAT     : 0xhh, 0xhh, // multiplier, n-repetitions (TEST ONLY)
 *  CHCKSUM    : 0xhh
 EXAMPLES
 * DEBUG: (0xAA is debug mode "MODE" byte)
 * 0x31 0xAA 0xhh(which debug function?) 0x00 0x00 0x00 0xhh(checksum)
 */



#endif	/* HX_ASL_BORON_H */

