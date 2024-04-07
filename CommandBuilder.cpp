#include "CommandBuilder.h"

#include <iostream>

#include "CommandBuilderHelpers.h"
#include "masks.h"

namespace DAPHNE
{
namespace CommandBuilder
{
std::string enableChannelOffsetGain(const CHANNEL_NUMBER_t &channel, bool enable)
{
  std::ostringstream ss;

  ss << "CFG OFFSET CH " << channel << " GAIN ";
  ss << ((enable) ? 2 : 1);
  ss << "\r\n";

  return ss.str();
}

std::string applyChannelOffsetVoltage_mV(const CHANNEL_NUMBER_t &channel, const int value_mVolts)
{
  std::ostringstream ss;

  ss << "WR OFFSET CH " << channel << " V " << value_mVolts << "\r\n";

  return ss.str();
}

std::string applyAFEGain_V(const AFE_NUMBER_t &afe, const double value_Volts)
{
  std::ostringstream ss;

  ss << "WR AFE " << (int)afe << " VGAIN V ";
  ss << Helpers::calculateAFEVGainReferenceValue(value_Volts);
  ss << "\r\n";

  return ss.str();
}

std::string configureAFEReg52(const AFE_NUMBER_t &afe, const REG_52_PARAMS_t &reg_52_params)
{
  std::ostringstream ss;
  uint16_t           reg_52_value = 0;

  Helpers::applyReg52Mask_LNAGain(reg_52_value, reg_52_params.LNAGain_dB);
  Helpers::applyReg52Mask_LNAIntegratorEnable(reg_52_value, reg_52_params.LNAIntegratorEnable);
  Helpers::applyReg52Mask_LNAClampLevel(reg_52_value, reg_52_params.LNAClampLevel_Vpp);
  Helpers::applyReg52Mask_activeTerminationEnable(
      reg_52_value, reg_52_params.activeTerminationEnable);
  Helpers::applyReg52Mask_presetActiveTerminationImpedance(
      reg_52_value, reg_52_params.presetActiveTerminationImpedance);

  ss << "WR AFE " << (int)afe << " REG 52 V " << reg_52_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg51(const AFE_NUMBER_t &afe, const REG_51_PARAMS_t &reg_51_params)
{
  std::ostringstream ss;
  uint16_t           reg_51_value = 0;

  Helpers::applyReg51Mask_PGAGain(reg_51_value, reg_51_params.PGAGain_dB);
  Helpers::applyReg51Mask_PGAIntegratorEnable(reg_51_value, reg_51_params.PGAIntegratorEnable);
  Helpers::applyReg51Mask_PGAClampLevel(reg_51_value, reg_51_params.PGAClampLevel_dBFS);
  Helpers::applyReg51Mask_LPFFrequency(reg_51_value, reg_51_params.LPFFrequency_MHz);

  ss << "WR AFE " << (int)afe << " REG 51 V " << reg_51_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg4(const AFE_NUMBER_t &afe, const REG_4_PARAMS_t &reg_4_params)
{
  std::ostringstream ss;
  uint16_t           reg_51_value = 0;
  Helpers::applyReg4Mask_ADCOutputFormat(reg_51_value, reg_4_params.ADCOutputFormat);
  Helpers::applyReg4Mask_ADCOutputFirstBit(reg_51_value, reg_4_params.ADCOutputFirstBit);

  ss << "WR AFE " << (int)afe << " REG 4 V " << reg_51_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg1(const AFE_NUMBER_t &afe, const REG_1_PARAMS_t &reg_1_params)
{
  std::ostringstream ss;
  uint16_t           reg_1_value = 0;

  ss << "WR AFE " << (int)afe << " REG 1 V " << reg_1_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg21(const AFE_NUMBER_t &afe, const REG_21_PARAMS_t &reg_21_params)
{
  std::ostringstream ss;
  uint16_t           reg_21_value = 0;

  ss << "WR AFE " << (int)afe << " REG 21 V " << reg_21_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg33(const AFE_NUMBER_t &afe, const REG_33_PARAMS_t &reg_33_params)
{
  std::ostringstream ss;
  uint16_t           reg_33_value = 0;

  ss << "WR AFE " << (int)afe << " REG 33 V " << reg_33_value << "\r\n";

  return ss.str();
}

std::string configureAFEReg59(const AFE_NUMBER_t &afe, const REG_59_PARAMS_t &reg_59_params)
{
  std::ostringstream ss;
  uint16_t           reg_59_value = 0;

  ss << "WR AFE " << (int)afe << " REG 59 V " << reg_59_value << "\r\n";

  return ss.str();
}

std::string FPGAReset(void)
{
  return "CFG FPGA RESET\r\n";
}

}  // namespace CommandBuilder
}  // namespace DAPHNE