#ifndef _COMMAND_BUILDER_
#define _COMMAND_BUILDER_

#include <stdint.h>

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif
void enableChannelOffsetGain(char *buffer, const CHANNEL_NUMBER_t channel, int enable);
void applyChannelOffsetVoltage_mV(
    char *buffer, const CHANNEL_NUMBER_t channel, const int value_mVolts);
void applyAFEGain_V(char *buffer, const AFE_NUMBER_t afe, const double value_Volts);
void configureAFEReg52(char *buffer, const AFE_NUMBER_t afe, const REG_52_PARAMS_t reg_52_params);
void configureAFEReg51(char *buffer, const AFE_NUMBER_t afe, const REG_51_PARAMS_t reg_51_params);
void configureAFEReg4(char *buffer, const AFE_NUMBER_t afe, const REG_4_PARAMS_t reg_4_params);
void configureAFEReg1(char *buffer, const AFE_NUMBER_t afe, const REG_1_PARAMS_t reg_1_params);
void configureAFEReg21(char *buffer, const AFE_NUMBER_t afe, const REG_21_PARAMS_t reg_21_params);
void configureAFEReg33(char *buffer, const AFE_NUMBER_t afe, const REG_33_PARAMS_t reg_33_params);
void configureAFEReg59(char *buffer, const AFE_NUMBER_t afe, const REG_59_PARAMS_t reg_59_params);

void FPGAReset(char *buffer);
#ifdef __cplusplus
}
#endif
#endif