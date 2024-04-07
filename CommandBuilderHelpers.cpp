#include "CommandBuilderHelpers.h"

#include "masks.h"

namespace DAPHNE
{
namespace CommandBuilder
{
namespace Helpers
{
int calculateAFEVGainReferenceValue(const double vgain_volts)
{
  double vGain_value = ((2.49 + 1.5) / 1.5) * vgain_volts;
  return (int)(vGain_value * 1000.0);
}

void applyReg52Mask_LNAGain(uint16_t &regValue, const LNA_GAIN_DB_t &gain_db)
{
  uint16_t maskLNAGain;

  switch ((int)gain_db)
  {
    case LNA_GAIN_12_DB:
      maskLNAGain = MASK_LNA_GAIN_12DB_REG_52;
      break;
    case LNA_GAIN_18_DB:
      maskLNAGain = MASK_LNA_GAIN_18DB_REG_52;
      break;
    case LNA_GAIN_24_DB:
      maskLNAGain = MASK_LNA_GAIN_24DB_REG_52;
      break;
    default:
      maskLNAGain = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_LNA_GAIN_CONTROL_REG_52;
  regValue = eraseAndApplyMask(regValue, maskLNAGain, eraser);
}

void applyReg52Mask_LNAIntegratorEnable(uint16_t &regValue, const bool &enable)
{
  uint16_t maskLNAIntegratorEnable =
      (enable) ? MASK_LNA_INTEGRATOR_EN_REG_52 : MASK_LNA_INTEGRATOR_DIS_REG_52;

  uint16_t eraser = MASK_ERASER_LNA_INTEGRATOR_REG_52;
  regValue = eraseAndApplyMask(regValue, maskLNAIntegratorEnable, eraser);
}

void applyReg52Mask_LNAClampLevel(uint16_t &regValue, const LNA_CLAMP_LEVEL_t &LNAClampLevel)
{
  uint16_t maskLNAClampLevel;
  switch ((int)LNAClampLevel)
  {
    case 0:
      maskLNAClampLevel = MASK_LNA_CLAMP_LEVEL_AUTO_REG_52;
      break;
    case 1:
      maskLNAClampLevel = MASK_LNA_CLAMP_LEVEL_15VPP_REG_52;
      break;
    case 2:
      maskLNAClampLevel = MASK_LNA_CLAMP_LEVEL_115VPP_REG_52;
      break;
    case 3:
      maskLNAClampLevel = MASK_LNA_CLAMP_LEVEL_06VPP_REG_52;
      break;
    default:
      maskLNAClampLevel = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_LNA_CLAMP_LEVEL_REG_52;
  regValue = eraseAndApplyMask(regValue, maskLNAClampLevel, eraser);
}

void applyReg52Mask_activeTerminationEnable(uint16_t &regValue, const bool &enable)
{
  uint16_t maskActiveTerminationEnable =
      (enable) ? MASK_ACTIVE_TERMINATION_EN_REG_52 : MASK_ACTIVE_TERMINATION_DIS_REG_52;

  uint16_t eraser = MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52;
  regValue = eraseAndApplyMask(regValue, maskActiveTerminationEnable, eraser);
}

void applyReg52Mask_presetActiveTerminationImpedance(
    uint16_t &regValue, const PRESET_ACTIVE_TERMINATION_IMPEDANCE_t &impedance)
{
  uint16_t maskPresetActiveTerminationImpedance;
  switch ((int)impedance)
  {
    case IMPEDANCE_50_OHMS:
      maskPresetActiveTerminationImpedance = MASK_PRESET_ACTIVE_TERMINATION_50OHMS_REG_52;
      break;
    case IMPEDANCE_100_OHMS:
      maskPresetActiveTerminationImpedance = MASK_PRESET_ACTIVE_TERMINATION_100OHMS_REG_52;
      break;
    case IMPEDANCE_200_OHMS:
      maskPresetActiveTerminationImpedance = MASK_PRESET_ACTIVE_TERMINATION_200OHMS_REG_52;
      break;
    case IMPEDANCE_400_OHMS:
      maskPresetActiveTerminationImpedance = MASK_PRESET_ACTIVE_TERMINATION_400OHMS_REG_52;
      break;
    default:
      maskPresetActiveTerminationImpedance = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_PRESET_ACTIVE_TERMINATIONS_REG_52;
  regValue = eraseAndApplyMask(regValue, maskPresetActiveTerminationImpedance, eraser);
}

void applyReg51Mask_PGAGain(uint16_t &regValue, const PGA_GAIN_DB_t &gain_db)
{
  uint16_t maskPGAGain;
  switch ((int)gain_db)
  {
    case PGA_GAIN_24_DB:
      maskPGAGain = MASK_PGA_GAIN_24DB_CONTROL_REG_51;
      break;
    case PGA_GAIN_30_DB:
      maskPGAGain = MASK_PGA_GAIN_30DB_CONTROL_REG_51;
      break;
    default:
      maskPGAGain = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_PGA_GAIN_CONTROL_REG_51;
  regValue = eraseAndApplyMask(regValue, maskPGAGain, eraser);
}

void applyReg51Mask_PGAIntegratorEnable(uint16_t &regValue, const bool &enable)
{
  uint16_t maskPGAIntegratorEnable =
      (enable) ? MASK_PGA_INTEGRATOR_EN_REG_51 : MASK_PGA_INTEGRATOR_DIS_REG_51;

  uint16_t eraser = MASK_ERASER_PGA_INTEGRATOR_REG_51;
  regValue = eraseAndApplyMask(regValue, maskPGAIntegratorEnable, eraser);
}

void applyReg51Mask_PGAClampLevel(uint16_t &regValue, const PGA_CLAMP_LEVEL_DBFS_t &PGAClampLevel)
{
  uint16_t maskPGAClampLevel;
  switch ((int)PGAClampLevel)
  {
    case PGA_CLAMP_NEG_2_DBFS:
      maskPGAClampLevel = MASK_PGA_CLAMP_LEVEL_N2DBFS_REG_51;
      break;
    case PGA_CLAMP_0_DBFS:
      maskPGAClampLevel = MASK_PGA_CLAMP_LEVEL_0DBFS_REG_51;
      break;
    case PGA_CLAMP_DISABLED:
      maskPGAClampLevel = MASK_PGA_CLAMP_LEVEL_DISABLED_REG_51;
      break;
    default:
      maskPGAClampLevel = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_PGA_CLAMP_LEVEL_REG_51;
  regValue = eraseAndApplyMask(regValue, maskPGAClampLevel, eraser);
}

void applyReg51Mask_LPFFrequency(uint16_t &regValue, const LPF_FREQUENCY_t &frequency_MHz)
{
  uint16_t maskLPFFrequency;
  switch ((int)frequency_MHz)
  {
    case LPF_FREQ_10_MHZ:
      maskLPFFrequency = MASK_LPF_10MHZ_PROGRAMABILITY_REG_51;
      break;
    case LPF_FREQ_15_MHZ:
      maskLPFFrequency = MASK_LPF_15MHZ_PROGRAMABILITY_REG_51;
      break;
    case LPF_FREQ_20_MHZ:
      maskLPFFrequency = MASK_LPF_20MHZ_PROGRAMABILITY_REG_51;
      break;
    case LPF_FREQ_30_MHZ:
      maskLPFFrequency = MASK_LPF_30MHZ_PROGRAMABILITY_REG_51;
      break;
    default:
      maskLPFFrequency = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_LPF_REG_51;
  regValue = eraseAndApplyMask(regValue, maskLPFFrequency, eraser);
}

void applyReg4Mask_ADCOutputFormat(uint16_t &regValue, const ADC_OUTPUT_FORMAT_t &ADCOutputFormat)
{
  uint16_t maskADCOutputFormat;

  switch ((int)ADCOutputFormat)
  {
    case ADC_FORMAT_2COMP:
      maskADCOutputFormat = MASK_ADC_OUTPUT_FORMAT_2COMP_REG_4;
      break;
    case ADC_FORMAT_OFFSET_BINARY:
      maskADCOutputFormat = MASK_ADC_OUTPUT_FORMAT_OFFSET_BIN_REG_4;
      break;
    default:
      maskADCOutputFormat = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_ADC_OUTPUT_FORMAT_REG_4;
  regValue = eraseAndApplyMask(regValue, maskADCOutputFormat, eraser);
}

void applyReg4Mask_ADCOutputFirstBit(
    uint16_t &regValue, const ADC_OUTPUT_FIRST_BIT_t &ADCOutputFirstBit)
{
  uint16_t maskADCFirstBit;

  switch ((int)ADCOutputFirstBit)
  {
    case ADC_ENDIAN_LSB_FIRST:
      maskADCFirstBit = MASK_LSB_FIRST_REG_4;
      break;
    case ADC_ENDIAN_MSB_FIRST:
      maskADCFirstBit = MASK_MSB_FIRST_REG_4;
      break;
    default:
      maskADCFirstBit = 0;
      break;
  }

  uint16_t eraser = MASK_ERASER_ADC_FIRST_BIT_REG_4;
  regValue = eraseAndApplyMask(regValue, maskADCFirstBit, eraser);
}

uint16_t eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser)
{
  uint16_t value_reg, eraser_;
  eraser_ = ~eraser;
  value_reg = reg & eraser_;
  value_reg = value_reg | mask;
  return value_reg;
}

}  // namespace Helpers
}  // namespace CommandBuilder
}  // namespace DAPHNE