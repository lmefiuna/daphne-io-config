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
        .lna_integrator_enable = true};

    REG_51_PARAMS_t afe_0_reg_51 = {
        .pga_gain_db = PGA_GAIN_30_DB,
        .pga_integrator_enable = true};

    outputCommands << commandBuilder.enableChannelOffsetGain(CHANNEL_0, false);
    outputCommands << commandBuilder.applyChannelOffsetVoltage_mV(CHANNEL_0, 2300);
    outputCommands << commandBuilder.applyAFEGain_V(AFE_0, 0.70);
    outputCommands << commandBuilder.configureAFEReg52(AFE_0, afe_0_reg_52);
    outputCommands << commandBuilder.configureAFEReg51(AFE_0, afe_0_reg_51);

    std::cout << outputCommands.str();

    return 0;
}