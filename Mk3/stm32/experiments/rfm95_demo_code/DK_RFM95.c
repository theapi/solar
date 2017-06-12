// ========================================================
/// @file       DK_RFM95.c
/// @brief      RFM95 basic application
/// @version    V2.1
/// @date       2013/12/25
/// @company    HOPE MICROELECTRONICS Co., Ltd.
/// @website    http://www.hoperf.com
/// @author     Geman Deng
/// @mobile     +86-013244720618
/// @tel        0755-82973805 Ext:846
// ========================================================
#include "DK_RFM.h"

/************************Description************************
                      ________________
                     /                \
                    /      HOPERF      \
                   /____________________\
                  |       DK_RFM95       |
    (MCU)DIO1 --- | DIO4             GND | --- GND
    (MCU)DIO0 --- | DIO3              A7 | --- 
  (MCU)RFData --- | DIO2              A6 | ---  
   (MCU)nIRQ1 --- | DIO1              A5 | --- 
   (MCU)nIRQ0 --- | DIO0              A4 | --- 
    (MCU)MISO --- | MISO              A3 | --- 
    (MCU)MOSI --- | MOSI              A2 | --- 
     (MCU)SCK --- | SCK               A1 | --- 
     (MCU)nCS --- | NSS               A0 | --- 
  (MCU)PORChk --- | RESET            VCC | --- VCC             
                  |______________________|
                  
                  

//  RF module:           RFM95
//  FSK:
//  Carry Frequency:     868MHz/915MHz
//  Bit Rate:            1.2Kbps/2.4Kbps/4.8Kbps/9.6Kbps
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Frequency Deviation: +/-35KHz
//  Receive Bandwidth:   83KHz
//  Coding:              NRZ
//  Packet Format:       0x5555555555+0xAA2DD4+"HopeRF RFM COBRFM95-S" (total: 29 bytes)
  
//  LoRa:
//  Carry Frequency:     868MHz/915MHz
//  Spreading Factor:    6/7/8/9/10/11/12
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Receive Bandwidth:   7.8KHz/10.4KHz/15.6KHz/20.8KHz/31.2KHz/41.7KHz/62.5KHz/125KHz/250KHz/500KHz
//  Coding:              NRZ
//  Packet Format:       "HopeRF RFM COBRFM95-S" (total: 21 bytes)

//  Tx Current:          about 120mA  (RFOP=+20dBm,typ.)
//  Rx Current:          about 11.5mA  (typ.)       
**********************************************************/


/**********************************************************
**Parameter table define
**********************************************************/
#define RFM95_PORchk_In  TRISD0=1   
#define RFM95_PORchk_Out TRISD0=0  
#define RFM95_PORchkin   RD0  
#define RFM95_PORchkout  LATD0      
/**********************************************************
**Parameter table define
**********************************************************/
const u16 RFM95FreqTbl[2][3] = 
{ 
  {0x06D9, 0x0700, 0x0800}, //868MHz
  {0x06E4, 0x07C0, 0x0800}, //915MHz
};

const u16 RFM95PowerTbl[4] =
{ 
  0x09FF,                   //20dbm  
  0x09FC,                   //17dbm
  0x09F9,                   //14dbm
  0x09F6,                   //11dbm 
};

const u8 RFM95SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
};

const u8 RFM95LoRaBwTbl[10] =
{
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

const u8  RFM95Data[] = {"HopeRF RFM COBRFM95-S"};

/**********************************************************
**Variable define
**********************************************************/
u8 gb_WaitStableFlag=0;                                    //State stable flag

void RFM95_Config(u8 mode);

///////////////////////////////////////////////// LoRa mode //////////////////////////////////////////////////
//Error Coding rate (CR)setting 
#define CR_4_5    
//#define CR_4_6    0
//#define CR_4_7    0
//#define CR_4_8    0 

#ifdef   CR_4_5
  #define CR    0x01                                       // 4/5
#else
  #ifdef   CR_4_6
    #define CR    0x02                                     // 4/6
  #else 
    #ifdef   CR_4_7 
      #define CR    0x03                                   // 4/7
    #else 
      #ifdef   CR_4_8
        #define CR    0x04                                 // 4/8
      #endif
    #endif
  #endif
#endif 

//CRC Enable
#define CRC_EN    

#ifdef  CRC_EN
  #define CRC   0x01                                       //CRC Enable
#else
  #define CRC   0x00
#endif

//RFM95 Internal registers Address
#define LR_RegFifo                                  0x0000
// Common settings
#define LR_RegOpMode                                0x0100
#define LR_RegFrMsb                                 0x0600
#define LR_RegFrMid                                 0x0700
#define LR_RegFrLsb                                 0x0800
// Tx settings
#define LR_RegPaConfig                              0x0900
#define LR_RegPaRamp                                0x0A00
#define LR_RegOcp                                   0x0B00
// Rx settings
#define LR_RegLna                                   0x0C00
// LoRa registers
#define LR_RegFifoAddrPtr                           0x0D00
#define LR_RegFifoTxBaseAddr                        0x0E00
#define LR_RegFifoRxBaseAddr                        0x0F00
#define LR_RegFifoRxCurrentaddr                     0x1000
#define LR_RegIrqFlagsMask                          0x1100
#define LR_RegIrqFlags                              0x1200
#define LR_RegRxNbBytes                             0x1300
#define LR_RegRxHeaderCntValueMsb                   0x1400
#define LR_RegRxHeaderCntValueLsb                   0x1500
#define LR_RegRxPacketCntValueMsb                   0x1600
#define LR_RegRxPacketCntValueLsb                   0x1700
#define LR_RegModemStat                             0x1800
#define LR_RegPktSnrValue                           0x1900
#define LR_RegPktRssiValue                          0x1A00
#define LR_RegRssiValue                             0x1B00
#define LR_RegHopChannel                            0x1C00
#define LR_RegModemConfig1                          0x1D00
#define LR_RegModemConfig2                          0x1E00
#define LR_RegSymbTimeoutLsb                        0x1F00
#define LR_RegPreambleMsb                           0x2000
#define LR_RegPreambleLsb                           0x2100
#define LR_RegPayloadLength                         0x2200
#define LR_RegMaxPayloadLength                      0x2300
#define LR_RegHopPeriod                             0x2400
#define LR_RegFifoRxByteAddr                        0x2500

// I/O settings
#define REG_LR_DIOMAPPING1                          0x4000
#define REG_LR_DIOMAPPING2                          0x4100
// Version
#define REG_LR_VERSION                              0x4200
// Additional settings
#define REG_LR_PLLHOP                               0x4400
#define REG_LR_TCXO                                 0x4B00
#define REG_LR_PADAC                                0x4D00
#define REG_LR_FORMERTEMP                           0x5B00

#define REG_LR_AGCREF                               0x6100
#define REG_LR_AGCTHRESH1                           0x6200
#define REG_LR_AGCTHRESH2                           0x6300
#define REG_LR_AGCTHRESH3                           0x6400

/*********************************************************/
//command
/*********************************************************/

/**********************************************************
**Name:     RFM95_Standby
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_Standby(void)
{
  SPIWrite(LR_RegOpMode+0x01);                              //Standby
}

/**********************************************************
**Name:     RFM95_Sleep
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_Sleep(void)
{
  SPIWrite(LR_RegOpMode+0x00);                              //Sleep
}


/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Name:     RFM95_EntryLoRa
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_EntryLoRa(void)
{
  SPIWrite(LR_RegOpMode+0x80);
}

/**********************************************************
**Name:     RFM95_LoRaClearIrq
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void RFM95_LoRaClearIrq(void)
{
  SPIWrite(LR_RegIrqFlags+0xFF);
}

/**********************************************************
**Name:     RFM95_LoRaEntryRx
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_LoRaEntryRx(void)
{
  u8 addr; 
        
  RFM95_Config(0);                                         //setting base parameter
  
  SPIWrite(0x4D00+0x84);                                   //Normal and Rx
  SPIWrite(LR_RegHopPeriod+0xFF);                          //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1+0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01
      
  SPIWrite(LR_RegIrqFlagsMask+0x3F);                       //Open RxDone interrupt & Timeout
  RFM95_LoRaClearIrq();   
  
  SPIWrite(LR_RegPayloadLength+ 21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
    
  addr = SPIRead((u8)(LR_RegFifoRxBaseAddr>>8));           //Read RxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr+addr);                        //RxBaseAddr -> FiFoAddrPtr¡¡ 
  SPIWrite(LR_RegOpMode+0x05);                        //Continuous Rx Mode
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM95_LoRaRxWaitStable
**Function: Determine whether the state of stable Rx
**Input:    none
**Output:   none
**********************************************************/
void RFM95_LoRaRxWaitStable(void)
{ 
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      if((SPIRead((u8)(LR_RegModemStat>>8))&0x04)==0x04)   //Rx-on going RegModemStat
      {
        gb_WaitStableFlag=1;        
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}

/**********************************************************
**Name:     RFM95_LoRaReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
u8 RFM95_LoRaReadRSSI(void)
{
  u16 temp=10;
  temp=SPIRead((u8)(LR_RegRssiValue>>8));                  //Read RegRssiValue£¬Rssi value
  temp=temp+127-137;                                       //127:Max RSSI, 137:RSSI offset
  return (u8)temp;
}

/**********************************************************
**Name:     RFM95_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
u8 RFM95_LoRaRxPacket(void)
{
  u8 i; 
  u8 addr;
  u8 packet_size;
  
  RFM95_LoRaRxWaitStable(); 
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;                                         //Rx state stable
  }       
 
  if((nIRQ0) && (gb_StatusRx==1))
  {
    for(i=0;i<32;i++) 
      gb_RxData[i] = 0x00;
    
    addr = SPIRead((u8)(LR_RegFifoRxCurrentaddr>>8));      //last packet addr
    SPIWrite(LR_RegFifoAddrPtr+addr);                      //RxBaseAddr -> FiFoAddrPtr    
    if(RFM95SpreadFactorTbl[gb_RateBuf_Addr]==6)           //When SpreadFactor is six£¬will used Implicit Header mode(Excluding internal packet length)
      packet_size=21;
    else
      packet_size = SPIRead((u8)(LR_RegRxNbBytes>>8));     //Number for received bytes    
    SPIBurstRead(0x00, gb_RxData, packet_size);
    
    RFM95_LoRaClearIrq();
    for(i=0;i<17;i++)
    {
      if(gb_RxData[i]!=RFM95Data[i])
        break;  
    }    
    if(i>=17)                                              //Rx success
      return(1);
    else
      return(0);
  }
  else
    return(0);
}

/**********************************************************
**Name:     RFM95_LoRaEntryTx
**Function: Entry Tx mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_LoRaEntryTx(void)
{
  u8 addr;
    
  RFM95_Config(0);                                         //setting base parameter
    
  SPIWrite(0x4D00+0x87);                                   //Tx for 20dBm
  SPIWrite(LR_RegHopPeriod);                               //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1+0x41);                       //DIO0=01, DIO1=00, DIO2=00, DIO3=01
  
  RFM95_LoRaClearIrq();
  SPIWrite(LR_RegIrqFlagsMask+0xF7);                       //Open TxDone interrupt
  SPIWrite(LR_RegPayloadLength+ 21);                       //RegPayloadLength  21byte
  
  addr = SPIRead((u8)(LR_RegFifoTxBaseAddr>>8));           //RegFiFoTxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr+addr);                        //RegFifoAddrPtr
    
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}
/**********************************************************
**Name:     RFM69_TxWaitStable
**Function: Determine whether the state of stable Tx
**Input:    none
**Output:   none
**********************************************************/
void RFM95_LoRaTxWaitStable(void)
{
  u8 temp; 
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      temp=SPIRead((u8)(LR_RegPayloadLength>>8));
      if(temp==21)
      {
        gb_WaitStableFlag=1; 
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}
/**********************************************************
**Name:     RFM95_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
u8 RFM95_LoRaTxPacket(void)
{ 
  u8 TxFlag=0;
  u8 addr;
  
  RFM95_LoRaTxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    if(gb_StatusTx==0)                                     //First entry Tx mode
    {
      gb_StatusTx=1;                                       //Tx state stable                     
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval 
            
      BurstWrite(0x00, (u8 *)RFM95Data, 21);
      SPIWrite(LR_RegOpMode+0x03);                         //Tx Mode           
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  if(gb_StatusTx==1)
  {
    if(gb_WaitStableFlag==3 && nIRQ0)                      //Packet send over
    {      
      SPIRead((u8)(LR_RegIrqFlags>>8));
      RFM95_LoRaClearIrq();                                //Clear irq
      
      gb_WaitStableFlag=0;
      RFM95_Standby();                                     //Entry Standby mode      
    }   
    if(gw_TxTimer==0 && gb_WaitStableFlag==0)              //It's time to Sending
    {
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval      
      //Send data 
      addr = SPIRead((u8)(LR_RegFifoTxBaseAddr>>8));       //RegFiFoTxBaseAddr
      SPIWrite(LR_RegFifoAddrPtr+addr);                    //RegFifoAddrPtr
      BurstWrite(0x00, (u8 *)RFM95Data, 21);
      SPIWrite(LR_RegOpMode+0x03);                         //Tx Mode 
      
      gb_WaitStableFlag=3;                                 //Promised to call mode stable decide
      TxFlag=1;
    }
  }
  return TxFlag;  
}


///////////////////////////////////////////////// FSK mode //////////////////////////////////////////////////
const u16 RFM95ConfigTbl[16] = 
{ 
  0x0402,                   //RegFdevMsb  35KHz 
  0x053D,                   //RegFdevLsb
  0x0B0B,                   //RegOcp  Close Ocp
  //0x0C20,                 //RegLNA  High & LNA Disable
  0x0C23,                   //RegLNA  High & LNA Enable
  0x1212,                   //RegRxBw   83KHz
  0x1FA0,                   //RegPreambleDet  Enable 2Byte 
  //0x1F20,                 //RegPreambleDet  Disable 
  0x2500,                   //RegPreambleMsb  
  0x2606,                   //RegPreambleLsb  6Byte Preamble
  0x2792,                   //RegSyncConfig Sync 2+1=3bytes
  0x2800+0xAA,              //SyncWord = aa2dd4
  0x2900+0x2D,              //
  0x2A00+0xD4,              //
  0x3000,                   //RegPacketConfig1  Disable CRC£¬NRZ
  0x3140,                   //RegPacketConfig2  Packet Mode
  0x3215,                   //RegPayloadLength  21bytes Fixed
  0x3595,                   //RegFiFoThresh   21bytes                        
};

const u16 RFM95FSKRateTbl[4][2] = 
{
  {0x0268, 0x032B},         //BR=1.2Kbps
  {0x0234, 0x0315},         //BR=2.4Kbps
  {0x021A, 0x030B},         //BR=4.8Kbps
  {0x020D, 0x0305},         //BR=9.6Kbps
};

const u16 RFM95RxTable[4] = 
{       
  0x090F,                   //RFIO Pin
  0x400C,                   //DIO0 Mapping for IRQ / DIO2 for RxData
  0x4100,                   //
  0x4D84,                   //Normal and Rx   
};
                              
const u16 RFM95TxTable[3] = 
{
  0x4000,                   //DIO0 Mapping for IRQ / DIO2 for RxData
  0x4100,                   //          
  0x4D87,                   //20dBm Tx
};

/**********************************************************
**Name:     RFM95_ReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
u8 RFM95_ReadRSSI(void)
{
  u8 temp=0xff;
  temp=SPIRead(0x11);
  temp>>=1;
  temp=127-temp;                                           //127:Max RSSI
  return temp;
}

/**********************************************************
**Name:     RFM95_FskClearFIFO
**Function: Change to RxMode from StandbyMode, can clear FIFO buffer
**Input:    None
**Output:   None
**********************************************************/
void RFM95_FskClearFIFO(void)
{
  SPIWrite(0x0101);                                        //Standby
  SPIWrite(0x0105);                                        //entry RxMode
}

/**********************************************************
**Name:     RFM95_FskEntryRx
**Function: Set RFM69 entry FSK Rx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM95_FskEntryRx(void)
{
  u8 i;
  
  Input_RFData();
    
  RFM95_Config(1);  
  for(i=0;i<2;i++)
    SPIWrite(RFM95FSKRateTbl[gb_RateBuf_Addr][i]);         //setting rf rate parameter  
  for(i=0;i<4;i++)                                         //Define to Rx mode 
    SPIWrite(RFM95RxTable[i]);    
  SPIWrite(0x0105);                                        //entry RxMode
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}
/**********************************************************
**Name:     RFM95_FSKRxWaitStable
**Function: Determine whether the state of stable Rx in FSK mode
**Input:    none
**Output:   none
**********************************************************/
void RFM95_FSKRxWaitStable(void)
{
  if(gb_WaitStableFlag==0x0f)
  {
    if(gb_SysTimer_1S!=0)
    {
      if((SPIRead(0x3E)&0xC0)==0xC0)
      {
        gb_WaitStableFlag=1;        
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}

/**********************************************************
**Name:     RFM95_FskRxPacket
**Function: Check for receive one packet
**Input:    none
**Output:   "!0"-----Receive one packet
**          "0"------Nothing for receive
**********************************************************/
u8 RFM95_FskRxPacket(void)
{
  u8 i; 
  
  RFM95_FSKRxWaitStable();  
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;                                         //Rx state stable
  }       
 
  if((nIRQ0) && (gb_StatusRx==1))
  { 
    for(i=0;i<32;i++) 
      gb_RxData[i] = 0x00;  
    
    SPIBurstRead(0x00, gb_RxData, 21);  
    RFM95_FskClearFIFO();
    for(i=0;i<17;i++)
    {
      if(gb_RxData[i]!=RFM95Data[i])
        break;  
    }
    if(i>=17) 
      return(1);                                           //Rx success
    else
      return(0);
  }
  else
    return(0);  
}

/**********************************************************
**Name:     RFM95_FskEntryTx
**Function: Set RFM95 entry FSK Tx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM95_FskEntryTx(void)
{
  u8 i;
  
  RFM95_Config(1);
  for(i=0;i<2;i++)
    SPIWrite(RFM95FSKRateTbl[gb_RateBuf_Addr][i]);         //setting rf rate parameter  
  for(i=0;i<3;i++)                                         //Define to Tx mode 
    SPIWrite(RFM95TxTable[i]);
  SPIWrite(0x0103);
      
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM95_FSKTxWaitStable
**Function: Determine whether the state of stable Tx
**Input:    none
**Output:   none
**********************************************************/
void RFM95_FSKTxWaitStable(void)
{ 
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      if((SPIRead(0x3E)&0xA0)==0xA0)
      {
        gb_WaitStableFlag=1; 
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}

/**********************************************************
**Name:     RFM95_FskTxPacket
**Function: Check RFM95 send over & send next packet
**Input:    none
**Output:   TxFlag=1, Send success
**********************************************************/
u8 RFM95_FskTxPacket(void)
{
  u8 TxFlag=0;
  
  RFM95_FSKTxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    if(gb_StatusTx==0)                                     //First entry Tx mode
    {
      gb_StatusTx=1;                                       //Tx state stable                     
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval          
      BurstWrite(0x00, (u8 *)RFM95Data, 21);          
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  if(gb_StatusTx==1)
  {
    if(gb_WaitStableFlag==3 && nIRQ0)                      //Packet send over
    {     
      gb_WaitStableFlag=0;
      RFM95_Standby();                                     //Entry Standby mode      
    }  
    if(gw_TxTimer==0)                                      //It's time to Sending
    {
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval
      gb_SysTimer_1S=3;
      gb_WaitStableFlag=0x0f;                              //Promised to call mode stable decide
      
      SPIWrite(0x0103);                                    //Entry Tx mode
    } 
    if(gb_WaitStableFlag==2)                               //It's time to Sending
    {
      BurstWrite(0x00, (u8 *)RFM95Data, 21);
      TxFlag=1;
      gb_WaitStableFlag=3;                                 //Promised to call mode stable decide
    }
  }
  return TxFlag;
}

/**********************************************************
**Name:     RFM95_Config
**Function: RFM95 base config
**Input:    mode
**Output:   None
**********************************************************/
void RFM95_Config(u8 mode)
{
  u8 i; 
    
  RFM95_PORchk_Out;
  RFM95_PORchkout=0;                                       //Ver1.1 (Change for Reset)
  for(i=200;i!=0;i--)                                      //Delay
    asm("NOP"); 
  RFM95_PORchkout=1; 
  for(i=200;i!=0;i--)                                      //Delay
    asm("NOP");  
  RFM95_PORchk_In;  
  for(gb_SysTimer_1S=1;gb_SysTimer_1S>0;);                 //waitting >10ms
    
  RFM95_Sleep();                                           //Change modem mode Must in Sleep mode 
  for(i=250;i!=0;i--)                                      //Delay
    asm("NOP");  

  if(!mode)
  {
    RFM95_EntryLoRa();  
    //SPIWrite(0x5904);   //?? Change digital regulator form 1.6V to 1.47V: see errata note
    
    for(i=0;i<3;i++)                                       //setting frequency parameter
    {
      SPIWrite(RFM95FreqTbl[gb_FreqBuf_Addr][i]);  
    }

    //setting base parameter 
    SPIWrite(RFM95PowerTbl[gb_PowerBuf_Addr]);             //Setting output power parameter  
    
    SPIWrite(LR_RegOcp+0x0B);                              //RegOcp,Close Ocp
    SPIWrite(LR_RegLna+0x23);                              //RegLNA,High & LNA Enable
    
    if(RFM95SpreadFactorTbl[gb_RateBuf_Addr]==6)           //SFactor=6
    {
      u8 tmp;
      SPIWrite(LR_RegModemConfig1+(RFM95LoRaBwTbl[gb_BandBuf_Addr]<<4)+(CR<<1)+0x01);//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      SPIWrite(LR_RegModemConfig2+(RFM95SpreadFactorTbl[gb_RateBuf_Addr]<<4)+(CRC<<2)+0x03);
      
      tmp = SPIRead(0x31);
      tmp &= 0xF8;
      tmp |= 0x05;
      SPIWrite(0x3100+tmp);
      SPIWrite(0x3700+0x0C);
    } 
    else
    {
      SPIWrite(LR_RegModemConfig1+(RFM95LoRaBwTbl[gb_BandBuf_Addr]<<4)+(CR<<1)+0x00);//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      SPIWrite(LR_RegModemConfig2+(RFM95SpreadFactorTbl[gb_RateBuf_Addr]<<4)+(CRC<<2)+0x03);  //SFactor &  LNA gain set by the internal AGC loop 
    }
    SPIWrite(LR_RegSymbTimeoutLsb+0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max) 
    
    SPIWrite(LR_RegPreambleMsb + 0);                       //RegPreambleMsb 
    SPIWrite(LR_RegPreambleLsb + 12);                      //RegPreambleLsb 8+4=12byte Preamble
    
    SPIWrite(REG_LR_DIOMAPPING2+0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
  
  }
  else
  {
    for(i=0;i<3;i++)                                       //setting frequency parameter
    {
      SPIWrite(RFM95FreqTbl[gb_FreqBuf_Addr][i]);  
    }
    SPIWrite(RFM95PowerTbl[gb_PowerBuf_Addr]);             //Setting output power parameter
    
    for(i=0;i<16;i++)                                      //setting base parameter
      SPIWrite(RFM95ConfigTbl[i]);
  }
  RFM95_Standby();                                         //Entry standby mode
}

///////////////////////////////////////////////// Test mode //////////////////////////////////////////////////
const u16 RFM95TestConfigTbl[9] = 
{
  0x0402,                   //RegFdevMsb  35KHz 
  0x053D,                   //RegFdevLsb
  0x0B0B,                   //RegOcp  Close Ocp
  0x0C20,                   //RegLNA  High & LNA Disable
  //0x0C23,                 //RegLNA  High & LNA Enable
  0x1212,                   //RegRxBw 83KHz
  0x1FA0,                   //RegPreambleDet  Enable 2Byte 
  //0x1F20,                 //RegPreambleDet  Disable 
  0x2500,                   //RegPreambleMsb  
  0x2600,                   //RegPreambleLsb  0Byte Preamble
  0x3100,                   //RegPacketConfig2  Continuous Mode
};

void RFM95_TestConfig(void)
{
  u8 i;
  
  RFM95_PORchk_Out;
  RFM95_PORchkout=0;                                       //Ver1.1 (Change for Reset)
  for(i=200;i!=0;i--)                                      //Delay
    asm("NOP"); 
  RFM95_PORchkout=1; 
  for(i=200;i!=0;i--)                                      //Delay
    asm("NOP");  
  RFM95_PORchk_In;  
  for(gb_SysTimer_1S=1;gb_SysTimer_1S>0;);                 //waitting >5ms
    
  RFM95_Sleep();                                           //Change modem mode Must in Sleep mode 
  for(i=250;i!=0;i--)                                      //Delay
    asm("NOP");  

  for(i=0;i<3;i++)                                         //setting frequency parameter
  {
    SPIWrite(RFM95FreqTbl[gb_FreqBuf_Addr][i]);  
  }
  for(i=0;i<2;i++)
    SPIWrite(RFM95FSKRateTbl[gb_RateBuf_Addr][i]);         //setting rf rate parameter
    
  SPIWrite(RFM95PowerTbl[gb_PowerBuf_Addr]);               //Setting output power parameter
  
  
  for(i=0;i<9;i++)                                         //setting base parameter 
    SPIWrite(RFM95TestConfigTbl[i]);
  RFM95_Standby();  
}

/**********************************************************
**Name:     RFM95_EntryTestTx
**Function: Set RFM95 entry Tx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_EntryTestTx(void)
{
  u8 i;
  
  Output_RFData();                                         //DIO2=0                                 
  RFData=0;
  RFM95_TestConfig();
  for(i=0;i<3;i++)                                         //Define to Tx mode  
    SPIWrite(RFM95TxTable[i]);
  SPIWrite(0x0100+0x20+0x03);
      
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

void RFM95_TestTx(void)
{ 
  RFM95_FSKTxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusTx=1;
    
    RFData=1;                                              //DIO2=1    
  }
}

/**********************************************************
**Name:     RFM95_EntryTestRx
**Function: Set RFM95 entry Rx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM95_EntryTestRx(void)
{
  u8 i;
  Input_RFData(); 
  RFM95_TestConfig();
  for(i=0;i<4;i++)                                         //Define to Rx mode 
    SPIWrite(RFM95RxTable[i]);
  SPIWrite(0x0105);
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

void RFM95_TestRx(void)
{   
  RFM95_FSKRxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;  
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 gb_WorkmodeBackup=0;
/**********************************************************
**Name:     RFM95_Running
**Function: RFM95 running function
**Input:    mode, work mode(Lo-T,Lo-R,FS-T,FS-R,T-T,T-R)
            WorkStatus, work status(LoRa,FSK,standby,sleep)
            ParaChangeFlag, Parameter changed when ParaChangeFlag=1
            *TxFlag, Tx success flag affect Tx count
            *RxFlag, Rx success flag affect Rx count
            *RSSI, RSSI value
**Output:   none
**********************************************************/
void RFM95_Running(u8 mode,u8 WorkStatus,u8 ParaChangeFlag,u8 *TxFlag,u8 *RxFlag,u8 *RSSI)
{ 
  u8 temp;  
  if(WorkStatus==C_ModuleWorkMode_LoRa)                    //LoRa modem mode
  {
    switch(mode)
    {
      case C_SysMode_LoRaTx:                               //Normal send mode(Abbreviation:Lo-T)
        if(ParaChangeFlag==1)                              //Parament changed
        {                   
          RFM95_LoRaEntryTx();
        }         
        temp=RFM95_LoRaTxPacket(); 
        if(temp==1){*TxFlag=1;}
        break;
      case C_SysMode_LoRaRx:                               //Normal receive mode(Abbreviation:Lo-R)      
        if(ParaChangeFlag==1)
        {           
          RFM95_LoRaEntryRx();
        }
        temp=RFM95_LoRaRxPacket(); 
        if(temp==1)
        {
          *RxFlag=1;
          BeepOn((u16*)MusicTab_RxSuccess);                //Buzzer on When received data success
        }
        
        *RSSI=RFM95_LoRaReadRSSI();                        //Read RSSI
        break;
    } 
  }
  else if(WorkStatus==C_ModuleWorkMode_FSK)                //FSK modem mode
  {
    switch(mode)
    {
      case C_SysMode_FSKTx:                                //Normal send mode(Abbreviation:FS-T)
        if(ParaChangeFlag==1)                              //Parament changed
        {                   
          RFM95_FskEntryTx();
        }         
        temp=RFM95_FskTxPacket(); 
        if(temp==1){*TxFlag=1;}
        break;
      case C_SysMode_FSKRx:                                //Normal receive mode(Abbreviation:FS-R)      
        if(ParaChangeFlag==1)
        {           
          RFM95_FskEntryRx();
        }
        temp=RFM95_FskRxPacket(); 
        if(temp==1)
        {
          *RxFlag=1;
          BeepOn((u16*)MusicTab_RxSuccess);                //Buzzer on When received data success
        }
        
        *RSSI=RFM95_ReadRSSI();                            //Read RSSI
        break;     
    } 
  }
  else if(WorkStatus==C_ModuleWorkMode_Standby)            //Standby status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Standby)
      RFM95_Standby();
  }
  else if(WorkStatus==C_ModuleWorkMode_Sleep)              //Sleep status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Sleep)
      RFM95_Sleep();
  }
  if(WorkStatus==C_ModuleWorkMode_LoRa || WorkStatus==C_ModuleWorkMode_FSK)
  {
    if(mode==C_SysMode_TestTx)
    {
      if(ParaChangeFlag==1)
      {           
        RFM95_EntryTestTx();
      }
      RFM95_TestTx();
    }
    else if(mode==C_SysMode_TestRx)
    {
      if(ParaChangeFlag==1)
      {       
        RFM95_EntryTestRx();
      }
      RFM95_TestRx();
      
      *RSSI=RFM95_ReadRSSI();                              //Read RSSI
    }
  }
  gb_WorkmodeBackup=WorkStatus;
}
