#ifndef _DAPHNE_CONFIG_COMMAND_BUILDER_
#define _DAPHNE_CONFIG_COMMAND_BUILDER_

#include <stdint.h>
#include <sstream>
#include "types.h"

class DaphneConfigCommandBuilder
{
public:
    DaphneConfigCommandBuilder();
    ~DaphneConfigCommandBuilder();
    std::string enableChannelOffsetGain(const CHANNEL_NUMBER_t &channel, bool enable);
    std::string applyChannelOffsetVoltage_mV(const CHANNEL_NUMBER_t &channel, const int value_mVolts);
    std::string applyAFEGain_V(const AFE_NUMBER_t &afe, const double value_Volts);
    std::string configureAFEReg52(const AFE_NUMBER_t &afe, const REG_52_PARAMS_t &reg_52_params);
    std::string configureAFEReg51(const AFE_NUMBER_t &afe, const REG_51_PARAMS_t &reg_51_params);

private:
    int calculateAFEVGainReferenceValue(const double vgain_volts);

    void applyAFEReg52Mask_LNAGain(uint16_t &reg_value, const LNA_GAIN_DB_t &gain_db);
    void applyAFEReg52Mask_LNAIntegrator(uint16_t &reg_value, const bool &enable);

    void applyAFEReg51Mask_PGAGain(uint16_t &reg_value, const PGA_GAIN_DB_t &gain_db);
    void applyAFEReg51Mask_PGAIntegrator(uint16_t &reg_value, const bool &enable);

    uint16_t eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser);
};

#endif