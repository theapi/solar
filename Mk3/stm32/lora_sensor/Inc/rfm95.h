
#ifndef __rfm95_H
#define __rfm95_H
#ifdef __cplusplus
 extern "C" {
#endif

// Register names (LoRa Mode, from table 85 of sx_1276 datasheet)
#define RF95_REG_FIFO                                0x00
#define RF95_REG_OP_MODE                             0x01
#define RF95_REG_FRF_MSB                             0x06
#define RF95_REG_FRF_MID                             0x07
#define RF95_REG_FRF_LSB                             0x08
#define RF95_REG_PA_CONFIG                           0x09
#define RF95_REG_PA_RAMP                             0x0a
#define RF95_REG_OCP                                 0x0b
#define RF95_REG_LNA                                 0x0c
#define RF95_REG_FIFO_ADDR_PTR                       0x0d
#define RF95_REG_FIFO_TX_BASE_ADDR                   0x0e
#define RF95_REG_FIFO_RX_BASE_ADDR                   0x0f
#define RF95_REG_FIFO_RX_CURRENT_ADDR                0x10
#define RF95_REG_IRQ_FLAGS_MASK                      0x11
#define RF95_REG_IRQ_FLAGS                           0x12
#define RF95_REG_RX_NB_BYTES                         0x13
#define RF95_REG_RX_HEADER_CNT_VALUE_MSB             0x14
#define RF95_REG_RX_HEADER_CNT_VALUE_LSB             0x15
#define RF95_REG_RX_PACKET_CNT_VALUE_MSB             0x16
#define RF95_REG_RX_PACKET_CNT_VALUE_LSB             0x17
#define RF95_REG_MODEM_STAT                          0x18
#define RF95_REG_PKT_SNR_VALUE                       0x19
#define RF95_REG_PKT_RSSI_VALUE                      0x1a
#define RF95_REG_RSSI_VALUE                          0x1b
#define RF95_REG_HOP_CHANNEL                         0x1c
#define RF95_REG_MODEM_CONFIG1                       0x1d
#define RF95_REG_MODEM_CONFIG2                       0x1e
#define RF95_REG_SYMB_TIMEOUT_LSB                    0x1f
#define RF95_REG_PREAMBLE_MSB                        0x20
#define RF95_REG_PREAMBLE_LSB                        0x21
#define RF95_REG_PAYLOAD_LENGTH                      0x22
#define RF95_REG_MAX_PAYLOAD_LENGTH                  0x23
#define RF95_REG_HOP_PERIOD                          0x24
#define RF95_REG_FIFO_RX_BYTE_ADDR                   0x25
#define RF95_REG_MODEM_CONFIG3                       0x26
#define RF95_REG_FEI_MSB                             0x28
#define RF95_REG_FEI_MID                             0x29
#define RF95_REG_FEI_LSB                             0x2a
#define RF95_REG_RSSI_WIDEBAND                       0x2c
#define RF95_REG_DETECT_OPTIMIZ                      0x31
#define RF95_REG_INVERT_IQ                           0x33
#define RF95_REG_DETECTION_THRESHOLD                 0x37
#define RF95_REG_SYNC_WORD                           0x39
#define RF95_REG_DIO_MAPPING1                        0x40
#define RF95_REG_DIO_MAPPING2                        0x41
#define RF95_REG_VERSION                             0x42
#define RF95_REG_TCXO                                0x4b
#define RF95_REG_PA_DAC                              0x4d
#define RF95_REG_FORMER_TEMP                         0x5b
#define RF95_REG_AGC_REF                             0x61
#define RF95_REG_AGC_THRESH1                         0x62
#define RF95_REG_AGC_THRESH2                         0x63
#define RF95_REG_AGC_THRESH3                         0x64
#define RF95_REG_PLL                                 0x70

#ifdef __cplusplus
}
#endif
#endif /*__rfm95_H */
