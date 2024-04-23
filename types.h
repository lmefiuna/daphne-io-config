#ifndef _types_h_
#define _types_h_

// namespace DAPHNE
// {
typedef enum
{
  AFE_0 = 0,
  AFE_1 = 1,
  AFE_2 = 2,
  AFE_3 = 3,
  AFE_4 = 4,
} AFE_NUMBER_t;

typedef enum
{
  CHANNEL_0 = 0,
  CHANNEL_1 = 1,
  CHANNEL_2 = 2,
  CHANNEL_3 = 3,
  CHANNEL_4 = 4,
  CHANNEL_5 = 5,
  CHANNEL_6 = 6,
  CHANNEL_7 = 7,
  CHANNEL_8 = 8,
  CHANNEL_9 = 9,
  CHANNEL_10 = 10,
  CHANNEL_11 = 11,
  CHANNEL_12 = 12,
  CHANNEL_13 = 13,
  CHANNEL_14 = 14,
  CHANNEL_15 = 15,
  CHANNEL_16 = 16,
  CHANNEL_17 = 17,
  CHANNEL_18 = 18,
  CHANNEL_19 = 19,
  CHANNEL_20 = 20,
  CHANNEL_21 = 21,
  CHANNEL_22 = 22,
  CHANNEL_23 = 23,
  CHANNEL_24 = 24,
  CHANNEL_25 = 25,
  CHANNEL_26 = 26,
  CHANNEL_27 = 27,
  CHANNEL_28 = 28,
  CHANNEL_29 = 29,
  CHANNEL_30 = 30,
  CHANNEL_31 = 31,
  CHANNEL_32 = 32,
  CHANNEL_33 = 33,
  CHANNEL_34 = 34,
  CHANNEL_35 = 35,
  CHANNEL_36 = 36,
  CHANNEL_37 = 37,
  CHANNEL_38 = 38,
  CHANNEL_39 = 39,
  CHANNEL_40 = 40
} CHANNEL_NUMBER_t;

typedef enum
{
  LNA_GAIN_12_DB = 12,
  LNA_GAIN_18_DB = 18,
  LNA_GAIN_24_DB = 24
} LNA_GAIN_DB_t;

typedef enum
{
  LNA_CLAMP_AUTO,
  LNA_CLAMP_1_5_VPP,
  LNA_CLAMP_1_15_VPP,
  LNA_CLAMP_0_6_VPP
} LNA_CLAMP_LEVEL_t;

typedef enum
{
  PGA_GAIN_24_DB = 24,
  PGA_GAIN_30_DB = 30
} PGA_GAIN_DB_t;

typedef enum
{
  PGA_CLAMP_0_DBFS,
  PGA_CLAMP_NEG_2_DBFS,
  PGA_CLAMP_DISABLED
} PGA_CLAMP_LEVEL_DBFS_t;

typedef enum
{
  LPF_FREQ_10_MHZ = 10,
  LPF_FREQ_15_MHZ = 15,
  LPF_FREQ_20_MHZ = 20,
  LPF_FREQ_30_MHZ = 30
} LPF_FREQUENCY_t;

typedef enum
{
  IMPEDANCE_50_OHMS = 50,
  IMPEDANCE_100_OHMS = 100,
  IMPEDANCE_200_OHMS = 200,
  IMPEDANCE_400_OHMS = 400
} PRESET_ACTIVE_TERMINATION_IMPEDANCE_t;

typedef struct
{
  LNA_GAIN_DB_t                         LNAGain_dB;
  int                                   LNAIntegratorEnable;
  int                                   activeTerminationEnable;
  PRESET_ACTIVE_TERMINATION_IMPEDANCE_t presetActiveTerminationImpedance;
  LNA_CLAMP_LEVEL_t                     LNAClampLevel_Vpp;
} REG_52_PARAMS_t;

typedef struct
{
  PGA_GAIN_DB_t          PGAGain_dB;
  int                    PGAIntegratorEnable;
  PGA_CLAMP_LEVEL_DBFS_t PGAClampLevel_dBFS;
  LPF_FREQUENCY_t        LPFFrequency_MHz;
} REG_51_PARAMS_t;

typedef enum
{
  ADC_FORMAT_OFFSET_BINARY,
  ADC_FORMAT_2COMP
} ADC_OUTPUT_FORMAT_t;

typedef enum
{
  ADC_ENDIAN_LSB_FIRST,
  ADC_ENDIAN_MSB_FIRST
} ADC_OUTPUT_FIRST_BIT_t;

typedef struct
{
  ADC_OUTPUT_FORMAT_t    ADCOutputFormat;
  ADC_OUTPUT_FIRST_BIT_t ADCOutputFirstBit;
} REG_4_PARAMS_t;

typedef struct
{
} REG_1_PARAMS_t;

typedef struct
{
} REG_21_PARAMS_t;

typedef struct
{
} REG_33_PARAMS_t;

typedef struct
{
} REG_59_PARAMS_t;
// }  // namespace DAPHNE
#endif