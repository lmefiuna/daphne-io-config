#ifndef _COMMAND_BUILDER_
#define _COMMAND_BUILDER_

#include <stdint.h>

#include <sstream>

#include "types.h"

namespace DAPHNE
{
namespace CommandBuilder
{
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

std::string FPGAReset(void);
}  // namespace CommandBuilder
}  // namespace DAPHNE

#endif