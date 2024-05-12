#include <fstream>
#include <iostream>
#include <sstream>

#include "CommandBuilder.h"

int main() {
  std::ostringstream outputCommands;

  char command_buffer[200];

  REG_52_PARAMS_t afe_0_reg_52 = {
      .LNAGain_dB = LNA_GAIN_12_DB,
      .LNAIntegratorEnable = true,
      .activeTerminationEnable = true,
      .presetActiveTerminationImpedance = IMPEDANCE_100_OHMS,
      .LNAClampLevel_Vpp = LNA_CLAMP_1_5_VPP};

  REG_51_PARAMS_t afe_0_reg_51 = {
      .PGAGain_dB = PGA_GAIN_24_DB,
      .PGAIntegratorEnable = true,
      .PGAClampLevel_dBFS = PGA_CLAMP_NEG_2_DBFS,
      .LPFFrequency_MHz = LPF_FREQ_10_MHZ};

  REG_4_PARAMS_t afe_0_reg_4 = {
      .ADCOutputFormat = ADC_FORMAT_OFFSET_BINARY, .ADCOutputFirstBit = ADC_ENDIAN_MSB_FIRST};

  REG_1_PARAMS_t  afe_0_reg_1 = {};
  REG_21_PARAMS_t afe_0_reg_21 = {};
  REG_33_PARAMS_t afe_0_reg_33 = {};
  REG_59_PARAMS_t afe_0_reg_59 = {};
  configureAFEReg52(command_buffer, AFE_0, afe_0_reg_52);
  outputCommands << command_buffer;
  configureAFEReg51(command_buffer, AFE_0, afe_0_reg_51);
  outputCommands << command_buffer;
  configureAFEReg4(command_buffer, AFE_0, afe_0_reg_4);
  outputCommands << command_buffer;
  configureAFEReg1(command_buffer, AFE_0, afe_0_reg_1);
  outputCommands << command_buffer;
  configureAFEReg21(command_buffer, AFE_0, afe_0_reg_21);
  outputCommands << command_buffer;
  configureAFEReg33(command_buffer, AFE_0, afe_0_reg_33);
  outputCommands << command_buffer;
  configureAFEReg59(command_buffer, AFE_0, afe_0_reg_59);
  outputCommands << command_buffer;
  enableChannelOffsetGain(command_buffer, CHANNEL_0, true);
  outputCommands << command_buffer;
  applyChannelOffsetVoltage_mV(command_buffer, CHANNEL_0, 2300);
  outputCommands << command_buffer;
  applyAFEGain_V(command_buffer, AFE_0, 0.70);
  // outputCommands << CommandBuilder::FPGAReset();

  std::cout << outputCommands.str();

  std::ofstream mFile("output_commands.txt");
  if (mFile.is_open()) {
    mFile << outputCommands.str();
    mFile.close();
    std::cout << "Content written to file successfully." << std::endl;
  } else {
    std::cerr << "Error: Unable to open file." << std::endl;
  }
  return 0;
}