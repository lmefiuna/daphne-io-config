#include <stdio.h>

#include "DaphneConfig.h"

const char CONFIG_FILE_PATH[] =
    "/home/caleb/caleb/fiuna/pasantia-tfg/mango-digitizer-daphne-v1/daphne-io-config/"
    "example.config";

int main() {
  DaphneConfig_t DaphneConfig;
  FILE          *f_ini;

  f_ini = fopen(CONFIG_FILE_PATH, "r");
  if (f_ini == NULL) {
    fprintf(stderr, "could not find config file %s", CONFIG_FILE_PATH);
    return 1;
  }
  // SetDefaultConfiguration(&DaphneConfig);
  ParseConfigFile(f_ini, &DaphneConfig);
  printf("AFE0 lna gain dB: %d\n", DaphneConfig.afe_reg_52_params[0].LNAGain_dB);
  printf("AFE0 lna integrator enable: %d\n", DaphneConfig.afe_reg_52_params[0].LNAIntegratorEnable);
  printf("AFE0 lna clamp level Vpp: %d\n", DaphneConfig.afe_reg_52_params[0].LNAClampLevel_Vpp);
  printf(
      "AFE0 active termination enable: %d\n",
      DaphneConfig.afe_reg_52_params[0].activeTerminationEnable);
  printf(
      "AFE0 preset active termination impedance: %d\n",
      DaphneConfig.afe_reg_52_params[0].presetActiveTerminationImpedance);
  printf("AFE0 pga gain db: %d\n", DaphneConfig.afe_reg_51_params[0].PGAGain_dB);
  printf("AFE0 pga integrator enable: %d\n", DaphneConfig.afe_reg_51_params[0].PGAIntegratorEnable);
  printf("AFE0 clamp level dbfs: %d\n", DaphneConfig.afe_reg_51_params[0].PGAClampLevel_dBFS);
  printf("AFE0 lpf freq MHz: %d\n", DaphneConfig.afe_reg_51_params[0].LPFFrequency_MHz);
  printf("AFE0 output format: %d\n", DaphneConfig.afe_reg_4_params[0].ADCOutputFormat);
  printf("AFE0 output first bit: %d\n", DaphneConfig.afe_reg_4_params[0].ADCOutputFirstBit);
  return 0;
}