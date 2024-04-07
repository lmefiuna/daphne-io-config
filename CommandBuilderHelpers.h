#ifndef _COMMAND_BUILDER_HELPERS_
#define _COMMAND_BUILDER_HELPERS_
#include <stdint.h>

#include "types.h"

namespace DAPHNE
{
namespace CommandBuilder
{
namespace Helpers
{
int calculateAFEVGainReferenceValue(const double vgain_volts);

void applyReg52Mask_LNAGain(uint16_t &regValue, const LNA_GAIN_DB_t &gain_db);
void applyReg52Mask_LNAIntegratorEnable(uint16_t &regValue, const bool &enable);
void applyReg52Mask_LNAClampLevel(uint16_t &regValue, const LNA_CLAMP_LEVEL_t &LNAClampLevel_Vpp);
void applyReg52Mask_activeTerminationEnable(uint16_t &regValue, const bool &enable);
void applyReg52Mask_presetActiveTerminationImpedance(
    uint16_t &regValue, const PRESET_ACTIVE_TERMINATION_IMPEDANCE_t &impedance);

void applyReg51Mask_PGAGain(uint16_t &regValue, const PGA_GAIN_DB_t &gain_db);
void applyReg51Mask_PGAIntegratorEnable(uint16_t &regValue, const bool &enable);
void applyReg51Mask_PGAClampLevel(uint16_t &regValue, const PGA_CLAMP_LEVEL_DBFS_t &PGAClampLevel_dBFS);
void applyReg51Mask_LPFFrequency(uint16_t &regValue, const LPF_FREQUENCY_t &frequency_MHz);

void applyReg4Mask_ADCOutputFormat(uint16_t &regValue, const ADC_OUTPUT_FORMAT_t &ADCOutputFormat);
void applyReg4Mask_ADCOutputFirstBit(
    uint16_t &regValue, const ADC_OUTPUT_FIRST_BIT_t &ADCOutputFirstBit);

uint16_t eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser);
}  // namespace Helpers
}  // namespace CommandBuilder
}  // namespace DAPHNE

#endif