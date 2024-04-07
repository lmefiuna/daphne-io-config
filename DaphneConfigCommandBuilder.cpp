#include "DaphneConfigCommandBuilder.h"

#include <iostream>

#include "masks.h"

DaphneConfigCommandBuilder::DaphneConfigCommandBuilder() {}
DaphneConfigCommandBuilder::~DaphneConfigCommandBuilder() {}

std::string DaphneConfigCommandBuilder::enableChannelOffsetGain(
    const CHANNEL_NUMBER_t &channel, bool enable)
{
  std::ostringstream ss;

  ss << "CFG OFFSET CH " << channel << " GAIN ";
  ss << ((enable) ? 2 : 1);
  ss << "\r\n";

  return ss.str();
}

std::string DaphneConfigCommandBuilder::applyChannelOffsetVoltage_mV(
    const CHANNEL_NUMBER_t &channel, const int value_mVolts)
{
  std::ostringstream ss;

  ss << "WR OFFSET CH " << channel << " V " << value_mVolts << "\r\n";

  return ss.str();
}

std::string DaphneConfigCommandBuilder::applyAFEGain_V(
    const AFE_NUMBER_t &afe, const double value_Volts)
{
  std::ostringstream ss;

  ss << "WR AFE " << (int)afe << " VGAIN V ";
  ss << this->calculateAFEVGainReferenceValue(value_Volts);
  ss << "\r\n";

  return ss.str();
}

int DaphneConfigCommandBuilder::calculateAFEVGainReferenceValue(const double vgain_volts)
{
  double vGain_value = ((2.49 + 1.5) / 1.5) * vgain_volts;
  return (int)(vGain_value * 1000.0);
}

std::string DaphneConfigCommandBuilder::configureAFEReg52(
    const AFE_NUMBER_t &afe, const REG_52_PARAMS_t &reg_52_params)
{
  std::ostringstream ss;
  uint16_t           reg_52_value = 0;

  this->applyReg52Mask_LNAGain(reg_52_value, reg_52_params.lna_gain_db);
  this->applyReg52Mask_LNAIntegratorEnable(reg_52_value, reg_52_params.lna_integrator_enable);
  this->applyReg52Mask_LNAClampLevel(reg_52_value, reg_52_params.LNAClampLevel);
  this->applyReg52Mask_activeTerminationEnable(reg_52_value, reg_52_params.activeTerminationEnable);
  this->applyReg52Mask_presetActiveTerminationImpedance(
      reg_52_value, reg_52_params.activeTerminationImpedance);

  ss << "WR AFE " << (int)afe << " REG 52 V " << reg_52_value << "\r\n";

  return ss.str();
}

void DaphneConfigCommandBuilder::applyReg52Mask_LNAGain(
    uint16_t &regValue, const LNA_GAIN_DB_t &gain_db)
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
  regValue = this->eraseAndApplyMask(regValue, maskLNAGain, eraser);
}

void DaphneConfigCommandBuilder::applyReg52Mask_LNAIntegratorEnable(
    uint16_t &regValue, const bool &enable)
{
  uint16_t maskLNAIntegratorEnable =
      (enable) ? MASK_LNA_INTEGRATOR_EN_REG_52 : MASK_LNA_INTEGRATOR_DIS_REG_52;

  uint16_t eraser = MASK_ERASER_LNA_INTEGRATOR_REG_52;
  regValue = this->eraseAndApplyMask(regValue, maskLNAIntegratorEnable, eraser);
}

void DaphneConfigCommandBuilder::applyReg52Mask_LNAClampLevel(
    uint16_t &regValue, const LNA_CLAMP_LEVEL_t &LNAClampLevel)
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
  regValue = this->eraseAndApplyMask(regValue, maskLNAClampLevel, eraser);
}

void DaphneConfigCommandBuilder::applyReg52Mask_activeTerminationEnable(
    uint16_t &regValue, const bool &enable)
{
  uint16_t maskActiveTerminationEnable =
      (enable) ? MASK_ACTIVE_TERMINATION_EN_REG_52 : MASK_ACTIVE_TERMINATION_DIS_REG_52;

  uint16_t eraser = MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52;
  regValue = this->eraseAndApplyMask(regValue, maskActiveTerminationEnable, eraser);
}

void DaphneConfigCommandBuilder::applyReg52Mask_presetActiveTerminationImpedance(
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
  regValue = this->eraseAndApplyMask(regValue, maskPresetActiveTerminationImpedance, eraser);
}

std::string DaphneConfigCommandBuilder::configureAFEReg51(
    const AFE_NUMBER_t &afe, const REG_51_PARAMS_t &reg_51_params)
{
  std::ostringstream ss;
  uint16_t           reg_51_value = 0;

  this->applyReg51Mask_PGAGain(reg_51_value, reg_51_params.pga_gain_db);
  this->applyReg51Mask_PGAIntegratorEnable(reg_51_value, reg_51_params.pga_integrator_enable);
  this->applyReg51Mask_PGAClampLevel(reg_51_value, reg_51_params.PGAClampLevel);
  this->applyReg51Mask_LPFFrequency(reg_51_value, reg_51_params.LPFFrequency_MHz);

  ss << "WR AFE " << (int)afe << " REG 51 V " << reg_51_value << "\r\n";

  return ss.str();
}

void DaphneConfigCommandBuilder::applyReg51Mask_PGAGain(
    uint16_t &regValue, const PGA_GAIN_DB_t &gain_db)
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
  regValue = this->eraseAndApplyMask(regValue, maskPGAGain, eraser);
}

void DaphneConfigCommandBuilder::applyReg51Mask_PGAIntegratorEnable(
    uint16_t &regValue, const bool &enable)
{
  uint16_t maskPGAIntegratorEnable =
      (enable) ? MASK_PGA_INTEGRATOR_EN_REG_51 : MASK_PGA_INTEGRATOR_DIS_REG_51;

  uint16_t eraser = MASK_ERASER_PGA_INTEGRATOR_REG_51;
  regValue = this->eraseAndApplyMask(regValue, maskPGAIntegratorEnable, eraser);
}

void DaphneConfigCommandBuilder::applyReg51Mask_PGAClampLevel(
    uint16_t &regValue, const PGA_CLAMP_LEVEL_DBFS_t &PGAClampLevel)
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
  regValue = this->eraseAndApplyMask(regValue, maskPGAClampLevel, eraser);
}

void DaphneConfigCommandBuilder::applyReg51Mask_LPFFrequency(
    uint16_t &regValue, const LPF_FREQUENCY_t &frequency_MHz)
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
  regValue = this->eraseAndApplyMask(regValue, maskLPFFrequency, eraser);
}

std::string DaphneConfigCommandBuilder::configureAFEReg4(
    const AFE_NUMBER_t &afe, const REG_4_PARAMS_t &reg_4_params)
{
  std::ostringstream ss;
  uint16_t           reg_51_value = 0;
  this->applyReg4Mask_ADCOutputFormat(reg_51_value, reg_4_params.ADCOutputFormat);
  this->applyReg4Mask_ADCOutputFirstBit(reg_51_value, reg_4_params.ADCOutputFirstBit);

  ss << "WR AFE " << (int)afe << " REG 4 V " << reg_51_value << "\r\n";

  return ss.str();
}

void DaphneConfigCommandBuilder::applyReg4Mask_ADCOutputFormat(
    uint16_t &regValue, const ADC_OUTPUT_FORMAT_t &ADCOutputFormat)
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
  regValue = this->eraseAndApplyMask(regValue, maskADCOutputFormat, eraser);
}

void DaphneConfigCommandBuilder::applyReg4Mask_ADCOutputFirstBit(
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
  regValue = this->eraseAndApplyMask(regValue, maskADCFirstBit, eraser);
}

std::string DaphneConfigCommandBuilder::configureAFEReg1(
    const AFE_NUMBER_t &afe, const REG_1_PARAMS_t &reg_1_params)
{
  std::ostringstream ss;
  uint16_t           reg_1_value = 0;

  ss << "WR AFE " << (int)afe << " REG 1 V " << reg_1_value << "\r\n";

  return ss.str();
}

std::string DaphneConfigCommandBuilder::configureAFEReg21(
    const AFE_NUMBER_t &afe, const REG_21_PARAMS_t &reg_21_params)
{
  std::ostringstream ss;
  uint16_t           reg_21_value = 0;

  ss << "WR AFE " << (int)afe << " REG 21 V " << reg_21_value << "\r\n";

  return ss.str();
}

std::string DaphneConfigCommandBuilder::configureAFEReg33(
    const AFE_NUMBER_t &afe, const REG_33_PARAMS_t &reg_33_params)
{
  std::ostringstream ss;
  uint16_t           reg_33_value = 0;

  ss << "WR AFE " << (int)afe << " REG 33 V " << reg_33_value << "\r\n";

  return ss.str();
}

std::string DaphneConfigCommandBuilder::configureAFEReg59(
    const AFE_NUMBER_t &afe, const REG_59_PARAMS_t &reg_59_params)
{
  std::ostringstream ss;
  uint16_t           reg_59_value = 0;

  ss << "WR AFE " << (int)afe << " REG 59 V " << reg_59_value << "\r\n";

  return ss.str();
}

uint16_t DaphneConfigCommandBuilder::eraseAndApplyMask(
    uint16_t &reg, uint16_t &mask, uint16_t &eraser)
{
  uint16_t value_reg, eraser_;
  eraser_ = ~eraser;
  value_reg = reg & eraser_;
  value_reg = value_reg | mask;
  return value_reg;
}

std::string DaphneConfigCommandBuilder::FPGAReset(void) { return "CFG FPGA RESET\r\n"; }
