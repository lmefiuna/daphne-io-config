#ifndef _DAPHNE_CONFIG_H_
#define _DAPHNE_CONFIG_H_

#include <stdio.h>

#include "types.h"

#define NUMBER_AFES     5
#define NUMBER_CHANNELS 40

typedef struct
{
  REG_52_PARAMS_t afe_reg_52_params[NUMBER_AFES];
  REG_51_PARAMS_t afe_reg_51_params[NUMBER_AFES];
  REG_4_PARAMS_t  afe_reg_4_params[NUMBER_AFES];
  REG_1_PARAMS_t  afe_reg_1_params[NUMBER_AFES];
  REG_21_PARAMS_t afe_reg_21_params[NUMBER_AFES];
  REG_33_PARAMS_t afe_reg_33_params[NUMBER_AFES];
  REG_59_PARAMS_t afe_reg_59_params[NUMBER_AFES];
  double          afe_gain_V[NUMBER_AFES];

  int ch_offset_gain_enable[NUMBER_CHANNELS];
  int ch_offset_voltage_mV[NUMBER_CHANNELS];
} DaphneConfig_t;

void SetDefaultConfiguration(DaphneConfig_t *DaphneConfig);

int ParseConfigFile(FILE *f_ini, DaphneConfig_t *DaphneConfig);

#endif