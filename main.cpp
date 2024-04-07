#include <iostream>
#include <sstream>
#include <fstream>
#include "DaphneConfigCommandBuilder.h"
#include "types.h"

int main()
{
    std::ostringstream outputCommands;
    DaphneConfigCommandBuilder commandBuilder;

    REG_52_PARAMS_t afe_0_reg_52 = {
        .lna_gain_db = LNA_GAIN_12_DB,
        .lna_integrator_enable = true,
        .activeTerminationEnable = true,
        .activeTerminationImpedance = IMPEDANCE_100_OHMS,
        .LNAClampLevel = LNA_CLAMP_1_5_VPP};

    REG_51_PARAMS_t afe_0_reg_51 = {
        .pga_gain_db = PGA_GAIN_24_DB,
        .pga_integrator_enable = true,
        .PGAClampLevel = PGA_CLAMP_NEG_2_DBFS,
        .LPFFrequency_MHz = LPF_FREQ_10_MHZ};

    REG_4_PARAMS_t afe_0_reg_4 = {
        .ADCOutputFormat = ADC_FORMAT_OFFSET_BINARY,
        .ADCOutputFirstBit = ADC_ENDIAN_MSB_FIRST};

    REG_1_PARAMS_t afe_0_reg_1 = {};
    REG_21_PARAMS_t afe_0_reg_21 = {};
    REG_33_PARAMS_t afe_0_reg_33 = {};
    REG_59_PARAMS_t afe_0_reg_59 = {};

    outputCommands << commandBuilder.configureAFEReg52(AFE_0, afe_0_reg_52);
    outputCommands << commandBuilder.configureAFEReg51(AFE_0, afe_0_reg_51);
    outputCommands << commandBuilder.configureAFEReg4(AFE_0, afe_0_reg_4);
    outputCommands << commandBuilder.configureAFEReg1(AFE_0, afe_0_reg_1);
    outputCommands << commandBuilder.configureAFEReg21(AFE_0, afe_0_reg_21);
    outputCommands << commandBuilder.configureAFEReg33(AFE_0, afe_0_reg_33);
    outputCommands << commandBuilder.configureAFEReg59(AFE_0, afe_0_reg_59);
    outputCommands << commandBuilder.enableChannelOffsetGain(CHANNEL_0, true);
    outputCommands << commandBuilder.applyChannelOffsetVoltage_mV(CHANNEL_0, 2300);
    outputCommands << commandBuilder.applyAFEGain_V(AFE_0, 0.70);
    outputCommands << commandBuilder.FPGAReset();

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