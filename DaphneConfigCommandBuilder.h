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
    std::string configureAFEReg4(const AFE_NUMBER_t &afe, const REG_4_PARAMS_t &reg_4_params);
    std::string configureAFEReg1(const AFE_NUMBER_t &afe, const REG_1_PARAMS_t &reg_1_params);
    std::string configureAFEReg21(const AFE_NUMBER_t &afe, const REG_21_PARAMS_t &reg_21_params);
    std::string configureAFEReg33(const AFE_NUMBER_t &afe, const REG_33_PARAMS_t &reg_33_params);
    std::string configureAFEReg59(const AFE_NUMBER_t &afe, const REG_59_PARAMS_t &reg_59_params);
    static std::string FPGAReset(void);

private:
    int calculateAFEVGainReferenceValue(const double vgain_volts);

    void applyReg52Mask_LNAGain(uint16_t &regValue, const LNA_GAIN_DB_t &gain_db);
    void applyReg52Mask_LNAIntegratorEnable(uint16_t &regValue, const bool &enable);
    void applyReg52Mask_LNAClampLevel(uint16_t &regValue, const LNA_CLAMP_LEVEL_t &LNAClampLevel);
    void applyReg52Mask_activeTerminationEnable(uint16_t &regValue, const bool &enable);
    void applyReg52Mask_presetActiveTerminationImpedance(
        uint16_t &regValue, const PRESET_ACTIVE_TERMINATION_IMPEDANCE_t &impedance);

    void applyReg51Mask_PGAGain(uint16_t &regValue, const PGA_GAIN_DB_t &gain_db);
    void applyReg51Mask_PGAIntegratorEnable(uint16_t &regValue, const bool &enable);
    void applyReg51Mask_PGAClampLevel(uint16_t &regValue, const PGA_CLAMP_LEVEL_DBFS_t &PGAClampLevel);
    void applyReg51Mask_LPFFrequency(uint16_t &regValue, const LPF_FREQUENCY_t &frequency_MHz);

    void applyReg4Mask_ADCOutputFormat(uint16_t &regValue, const ADC_OUTPUT_FORMAT_t &ADCOutputFormat);
    void applyReg4Mask_ADCOutputFirstBit(uint16_t &regValue, const ADC_OUTPUT_FIRST_BIT_t &ADCOutputFirstBit);

    uint16_t eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser);
};

#endif