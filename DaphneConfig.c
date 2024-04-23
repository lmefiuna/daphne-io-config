#include "DaphneConfig.h"

static void SetDefaultConfiguration(DaphneConfig_t *DaphneConfig)
{
  int i;
  for (i = 0; i < NUMBER_AFES; ++i)
  {
    DaphneConfig->afe_reg_52_params[i] = (REG_52_PARAMS_t){
        .LNAGain_dB = LNA_GAIN_12_DB,
        .LNAIntegratorEnable = 1,
        .activeTerminationEnable = 1,
        .presetActiveTerminationImpedance = IMPEDANCE_50_OHMS,
        .LNAClampLevel_Vpp = LNA_CLAMP_1_5_VPP};

    DaphneConfig->afe_reg_51_params[i] = (REG_51_PARAMS_t){
        .PGAGain_dB = PGA_GAIN_24_DB,
        .PGAIntegratorEnable = 1,
        .PGAClampLevel_dBFS = PGA_CLAMP_NEG_2_DBFS,
        .LPFFrequency_MHz = LPF_FREQ_10_MHZ};

    DaphneConfig->afe_reg_4_params[i] = (REG_4_PARAMS_t){
        .ADCOutputFormat = ADC_FORMAT_OFFSET_BINARY, .ADCOutputFirstBit = ADC_ENDIAN_MSB_FIRST};

    DaphneConfig->afe_reg_1_params[i] = (REG_1_PARAMS_t){};
    DaphneConfig->afe_reg_21_params[i] = (REG_21_PARAMS_t){};
    DaphneConfig->afe_reg_33_params[i] = (REG_33_PARAMS_t){};
    DaphneConfig->afe_reg_59_params[i] = (REG_59_PARAMS_t){};

    DaphneConfig->afe_gain_V[i] = 0.70;
  }

  for (i = 0; i < NUMBER_CHANNELS; ++i)
  {
    DaphneConfig->ch_offset_gain_enable[i] = 1;
    DaphneConfig->ch_offset_voltage_mV[i] = 2300;
  }
}

int ParseConfigFile(FILE *f_ini, DaphneConfig_t *DaphneConfig)
{
  char str[1000];
  int  current_afe = -1;
  int  current_channel = -1;
  int  value_read;

  int ret = 0;

  SetDefaultConfiguration(DaphneConfig);

  while (!feof(f_ini))
  {
    int   read;
    char *res;

    // read a word from the file
    read = fscanf(f_ini, "%s", str);
    if (!read || (read == EOF) || !strlen(str))
    {
      continue;
    }

    // skip comments
    if (str[0] == '#')
    {
      res = fgets(str, 1000, f_ini);
      printf("%s: comment\n", str);
      continue;
    }

    // Section (COMMON or individual channel)
    if (str[0] == '[')
    {
      if (strstr(str, "COMMON"))
      {
        current_afe = -1;
        current_channel = -1;
        printf("section common");
        continue;
      }
      if (strstr(str, "AFE"))
      {
        sscanf(str + 1, "AFE%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_AFES)
        {
          printf("%s: Invalid AFE number\n", str);
        }
        else
        {
          current_afe = value_read;
          printf("section afe %d", current_afe);
        }
      }
      else if (strstr(str, "CHANNEL"))
      {
        sscanf(str + 1, "CHANNEL%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_CHANNELS)
        {
          printf("%s: Invalid channel number\n", str);
        }
        else
        {
          current_channel = value_read;
          printf("section channel %d", current_channel);
        }
      }
      continue;
    }
  }
  return ret;
}