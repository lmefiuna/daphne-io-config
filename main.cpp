#include <fstream>
#include <iostream>
#include <sstream>

#include "CommandBuilder.h"
#include "ConfigExceptions.hpp"
#include "ConfigParser.hpp"
#include "ConfigProperties.hpp"
#include "types.h"

using namespace DAPHNE;

int main(int argc, char *argv[], char *envp[])
{
  // try
  // {
    //   ConfigReader::ConfigParser configParser("example.config", envp);

    //   for (uint8_t i = 0; i < sizeof(ConfigReader::keys) / sizeof(ConfigReader::keys[0]); ++i)
    //   {
    //     ConfigReader::values[i] = configParser.pDouble(ConfigReader::keys[i]);
    //     std::cout << ConfigReader::keys[i] << ":\t";
    //     std::cout << ConfigReader::values[i] << std::endl;
    //     // std::cout << *(values[i]) << std::endl;
    //   }

    //   // int LNAGain_dB;
    //   // LNAGain_dB = configParser.pInt("LNAGain_dB");
    //   // std::cout << "LNA Gain dB: " << LNAGain_dB << std::endl;
    // }
    // catch (ConfigReader::ErrorOpeningFileException &e)
    // {
    //   std::cerr << e.what();
    //   return 2;
    // }
    // catch (ConfigReader::MissingPropertyException &e)
    // {
    //   std::cerr << e.what();
    //   return 2;
    // }
    // catch (std::exception &e)
    // {
    //   std::cerr << e.what() << std::endl;
    //   return 1;
    // }

    std::ostringstream outputCommands;

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
    outputCommands << CommandBuilder::configureAFEReg52(AFE_0, afe_0_reg_52);
    outputCommands << CommandBuilder::configureAFEReg51(AFE_0, afe_0_reg_51);
    outputCommands << CommandBuilder::configureAFEReg4(AFE_0, afe_0_reg_4);
    outputCommands << CommandBuilder::configureAFEReg1(AFE_0, afe_0_reg_1);
    outputCommands << CommandBuilder::configureAFEReg21(AFE_0, afe_0_reg_21);
    outputCommands << CommandBuilder::configureAFEReg33(AFE_0, afe_0_reg_33);
    outputCommands << CommandBuilder::configureAFEReg59(AFE_0, afe_0_reg_59);
    outputCommands << CommandBuilder::enableChannelOffsetGain(CHANNEL_0, true);
    outputCommands << CommandBuilder::applyChannelOffsetVoltage_mV(CHANNEL_0, 2300);
    outputCommands << CommandBuilder::applyAFEGain_V(AFE_0, 0.70);
    // outputCommands << CommandBuilder::FPGAReset();

    std::cout << outputCommands.str();

    std::ofstream mFile("output.txt");
    if (mFile.is_open())
    {
      mFile << outputCommands.str();
      mFile.close();
      std::cout << "Content written to file successfully." << std::endl;
    }
    else
    {
      std::cerr << "Error: Unable to open file." << std::endl;
    }

    return 0;
  }