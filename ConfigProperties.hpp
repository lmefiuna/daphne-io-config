#ifndef _CONFIG_PROPERTIES_H
#define _CONFIG_PROPERTIES_H

#include <string>

#include "types.h"

namespace DAPHNE
{
namespace ConfigReader
{
const std::string keys[] = {
    "LNAGain_dB",
    "LNAIntegratorEnable",
    "LNAClampLevel_Vpp",
    "activeTerminationEnable",
    "presetActiveTerminationImpedance",
    "PGAGain_dB",
    "PGAIntegratorEnable",
    "PGAClampLevel_dBFS",
    "LPFFrequency_MHz",
    "ADCOutputFormat",
    "ADCOutputFirstBit"};

const REG_52_PARAMS_t reg52Params = {
    .LNAGain_dB = LNA_GAIN_12_DB,
    .LNAIntegratorEnable = true,
    .activeTerminationEnable = true,
    .presetActiveTerminationImpedance = IMPEDANCE_100_OHMS,
    .LNAClampLevel_Vpp = LNA_CLAMP_1_5_VPP};

const REG_51_PARAMS_t reg51Params = {
    .PGAGain_dB = PGA_GAIN_24_DB,
    .PGAIntegratorEnable = true,
    .PGAClampLevel_dBFS = PGA_CLAMP_NEG_2_DBFS,
    .LPFFrequency_MHz = LPF_FREQ_10_MHZ};

const REG_4_PARAMS_t reg4Params = {
    .ADCOutputFormat = ADC_FORMAT_OFFSET_BINARY, .ADCOutputFirstBit = ADC_ENDIAN_MSB_FIRST};

const REG_1_PARAMS_t  reg1Params = {};
const REG_21_PARAMS_t reg21Params = {};
const REG_33_PARAMS_t reg33Params = {};
const REG_59_PARAMS_t reg59Params = {};

}  // namespace ConfigReader
}  // namespace DAPHNE

#endif