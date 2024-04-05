#include <iostream>
#include <sstream>
#include "DaphneConfigCommandBuilder.h"
#include "types.h"
int main()
{
    std::ostringstream outputCommands;
    DaphneConfigCommandBuilder commandBuilder;

    REG_52_PARAMS_t afe_0_reg_52 = {
        .lna_gain_db = LNA_GAIN_24_DB,
        .lna_integrator_enable = true,
        .activeTerminationEnable = true,
        .activeTerminationImpedance = IMPEDANCE_100_OHMS,
        .LNAClampLevel = LNA_CLAMP_1_5_VPP};

    REG_51_PARAMS_t afe_0_reg_51 = {
        .pga_gain_db = PGA_GAIN_30_DB,
        .pga_integrator_enable = true,
        .PGAClampLevel = PGA_CLAMP_NEG_2_DBFS,
        .LPFFrequency_MHz = LPF_FREQ_10_MHZ};

    outputCommands << commandBuilder.enableChannelOffsetGain(CHANNEL_0, false);
    outputCommands << commandBuilder.applyChannelOffsetVoltage_mV(CHANNEL_0, 2300);
    outputCommands << commandBuilder.applyAFEGain_V(AFE_0, 0.70);
    outputCommands << commandBuilder.configureAFEReg52(AFE_0, afe_0_reg_52);
    outputCommands << commandBuilder.configureAFEReg51(AFE_0, afe_0_reg_51);

    std::cout << outputCommands.str();

    return 0;
}