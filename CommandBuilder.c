#include "CommandBuilder.h"

#include <stdio.h>

#include "CommandBuilderHelpers.h"
#include "masks.h"

void enableChannelOffsetGain(char *buffer, const CHANNEL_NUMBER_t channel, int enable) {
  // std::ostringstream ss;

  sprintf(buffer, "CFG OFFSET CH %d GAIN %d\r\n", channel, ((enable) ? 2 : 1));

  // ss << "CFG OFFSET CH " << channel << " GAIN ";
  // ss << ((enable) ? 2 : 1);
  // ss << "\r\n";

  // buffer = ss.str().c_str();
}

void applyChannelOffsetVoltage_mV(
    char *buffer, const CHANNEL_NUMBER_t channel, const int value_mVolts) {
  // std::ostringstream ss;

  sprintf(buffer, "WR OFFSET CH %d V %d\r\n", channel, value_mVolts);
  // ss << "WR OFFSET CH " << channel << " V " << value_mVolts << "\r\n";

  // buffer = ss.str().c_str();
}

void applyAFEGain_V(char *buffer, const AFE_NUMBER_t afe, const double value_Volts) {
  // std::ostringstream ss;

  sprintf(buffer, "WR AFE %d VGAIN V %d\r\n", afe, calculateAFEVGainReferenceValue(value_Volts));
  // ss << "WR AFE " << (int)afe << " VGAIN V ";
  // ss << calculateAFEVGainReferenceValue(value_Volts);
  // ss << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg52(char *buffer, const AFE_NUMBER_t afe, const REG_52_PARAMS_t reg_52_params) {
  // std::ostringstream ss;
  uint16_t reg_52_value = 0;
  // *reg_52_value = 0;

  applyReg52Mask_LNAGain(&reg_52_value, reg_52_params.LNAGain_dB);
  applyReg52Mask_LNAIntegratorEnable(&reg_52_value, reg_52_params.LNAIntegratorEnable);
  applyReg52Mask_LNAClampLevel(&reg_52_value, reg_52_params.LNAClampLevel_Vpp);
  applyReg52Mask_activeTerminationEnable(&reg_52_value, reg_52_params.activeTerminationEnable);
  applyReg52Mask_presetActiveTerminationImpedance(
      &reg_52_value, reg_52_params.presetActiveTerminationImpedance);

  sprintf(buffer, "WR AFE %d REG 52 V %d\r\n", afe, reg_52_value);

  // ss << "WR AFE " << (int)afe << " REG 52 V " << reg_52_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg51(char *buffer, const AFE_NUMBER_t afe, const REG_51_PARAMS_t reg_51_params) {
  // std::ostringstream ss;
  uint16_t reg_51_value = 0;

  applyReg51Mask_PGAGain(&reg_51_value, reg_51_params.PGAGain_dB);
  applyReg51Mask_PGAIntegratorEnable(&reg_51_value, reg_51_params.PGAIntegratorEnable);
  applyReg51Mask_PGAClampLevel(&reg_51_value, reg_51_params.PGAClampLevel_dBFS);
  applyReg51Mask_LPFFrequency(&reg_51_value, reg_51_params.LPFFrequency_MHz);

  sprintf(buffer, "WR AFE %d REG 51 V %d\r\n", afe, reg_51_value);
  // ss << "WR AFE " << (int)afe << " REG 51 V " << reg_51_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg4(char *buffer, const AFE_NUMBER_t afe, const REG_4_PARAMS_t reg_4_params) {
  // std::ostringstream ss;
  uint16_t reg_4_value = 0;
  applyReg4Mask_ADCOutputFormat(&reg_4_value, reg_4_params.ADCOutputFormat);
  applyReg4Mask_ADCOutputFirstBit(&reg_4_value, reg_4_params.ADCOutputFirstBit);

  sprintf(buffer, "WR AFE %d REG 4 V %d\r\n", afe, reg_4_value);
  // ss << "WR AFE " << (int)afe << " REG 4 V " << reg_51_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg1(char *buffer, const AFE_NUMBER_t afe, const REG_1_PARAMS_t reg_1_params) {
  // std::ostringstream ss;
  uint16_t reg_1_value = 0;

  sprintf(buffer, "WR AFE %d REG 1 V %d\r\n", afe, reg_1_value);

  // ss << "WR AFE " << (int)afe << " REG 1 V " << reg_1_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg21(char *buffer, const AFE_NUMBER_t afe, const REG_21_PARAMS_t reg_21_params) {
  // std::ostringstream ss;
  uint16_t reg_21_value = 0;

  sprintf(buffer, "WR AFE %d REG 21 V %d\r\n", afe, reg_21_value);
  // ss << "WR AFE " << (int)afe << " REG 21 V " << reg_21_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg33(char *buffer, const AFE_NUMBER_t afe, const REG_33_PARAMS_t reg_33_params) {
  // std::ostringstream ss;
  uint16_t reg_33_value = 0;

  sprintf(buffer, "WR AFE %d REG 33 V %d\r\n", afe, reg_33_value);

  // ss << "WR AFE " << (int)afe << " REG 33 V " << reg_33_value << "\r\n";

  // buffer = ss.str().c_str();
}

void configureAFEReg59(char *buffer, const AFE_NUMBER_t afe, const REG_59_PARAMS_t reg_59_params) {
  // std::ostringstream ss;
  uint16_t reg_59_value = 0;

  sprintf(buffer, "WR AFE %d REG 59 V %d\r\n", afe, reg_59_value);
  // ss << "WR AFE " << (int)afe << " REG 59 V " << reg_59_value << "\r\n";

  // buffer = ss.str().c_str();
}

void FPGAReset(char *buffer) {
  sprintf(buffer, "CFG FPGA RESET\r\n");
  // buffer = "CFG FPGA RESET\r\n";
}

// }  // namespace CommandBuilder
// }  // namespace DAPHNE