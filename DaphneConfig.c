#include "DaphneConfig.h"

#include <string.h>

void SetDefaultConfiguration(DaphneConfig_t *DaphneConfig) {
  int i;
  for (i = 0; i < NUMBER_AFES; ++i) {
    DaphneConfig->enableConfigureAFE[i] = 0;

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

  for (i = 0; i < NUMBER_CHANNELS; ++i) {
    DaphneConfig->enableConfigureChannel[i] = 0;

    DaphneConfig->ch_offset_gain_enable[i] = 1;
    DaphneConfig->ch_offset_voltage_mV[i] = 2300;
  }
}

int ParseConfigFile(FILE *f_ini, DaphneConfig_t *DaphneConfig) {
  char str[1000];
  int  current_afe = -1;
  int  current_channel = -1;
  int  value_read;
  int  i;

  int line_counter = 0;

  int ret = 0;

  SetDefaultConfiguration(DaphneConfig);

  while (!feof(f_ini)) {
    int   read;
    char *res;

    // read a word from the file
    read = fscanf(f_ini, "%s", str);
    if (!read || (read == EOF) || !strlen(str)) {
      continue;
    }
    // printf("%s\n", str);
    line_counter += 1;

    // skip comments
    if (str[0] == '#') {
      res = fgets(str, 1000, f_ini);
      // printf("%s: comment\n", str);
      continue;
    }

    // Section (COMMON or individual afe/channel)
    if (str[0] == '[') {
      if (strstr(str, "COMMON")) {
        current_afe = -1;
        current_channel = -1;
        // printf("section common");
        continue;
      }
      if (strstr(str, "AFE")) {
        sscanf(str + 1, "AFE%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_AFES) {
          printf("Line %d: Invalid AFE number\n", line_counter);
        } else {
          current_afe = value_read;
          // printf("section afe %d", current_afe);
        }
      } else if (strstr(str, "CHANNEL")) {
        sscanf(str + 1, "CHANNEL%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_CHANNELS) {
          printf("Line %d: Invalid channel number\n", line_counter);
        } else {
          current_channel = value_read;
        }
      } else {
        printf("Line %d: Invalid section identifier\n", line_counter);
      }
      continue;
    }

    if (strstr(str, "LNA_GAIN_DB") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case LNA_GAIN_12_DB:
        case LNA_GAIN_18_DB:
        case LNA_GAIN_24_DB:
          break;
        default:
          printf("Line %d: Invalid LNA_GAIN_DB value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_52_params[i].LNAGain_dB = value_read;
        }
      } else {
        DaphneConfig->afe_reg_52_params[current_afe].LNAGain_dB = value_read;
      }
      continue;
    }

    if (strstr(str, "CONFIGURE_AFE") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid CONFIGURE_AFE value\n", line_counter);
          continue;
      }

      //printf("Line %d: CONFIGURE_AFE: %d\n", line_counter, value_read);

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->enableConfigureAFE[i] = value_read;
        }
      } else {
        // printf("Line %d: Enable configure CONFIGURE_AFE: %d\n", line_counter, value_read);
        // printf("Previous value: %d\n", DaphneConfig->enableConfigureAFE[current_afe]);
        DaphneConfig->enableConfigureAFE[current_afe] = value_read;
      }
      // printf("AFE %d enable configure new value: %d\n", current_afe, DaphneConfig->enableConfigureAFE[current_afe]);
      continue;
    }

    if (strstr(str, "CONFIGURE_CHANNEL") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid CONFIGURE_CHANNEL value\n", line_counter);
          continue;
      }

      if (current_channel == -1) {
        for (i = 0; i < NUMBER_CHANNELS; ++i) {
          DaphneConfig->enableConfigureChannel[i] = value_read;
        }
      } else {
        DaphneConfig->enableConfigureChannel[current_channel] = value_read;
      }
      continue;
    }

    if (strstr(str, "LNA_INTEGRATOR_ENABLE") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid LNA_INTEGRATOR_ENABLE value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_52_params[i].LNAIntegratorEnable = value_read;
        }
      } else {
        DaphneConfig->afe_reg_52_params[current_afe].LNAIntegratorEnable = value_read;
      }
      continue;
    }

    if (strstr(str, "LNA_CLAMP_LEVEL_VPP") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case LNA_CLAMP_AUTO:
        case LNA_CLAMP_1_5_VPP:
        case LNA_CLAMP_1_15_VPP:
        case LNA_CLAMP_0_6_VPP:
          break;

        default:
          printf("Line %d: Invalid LNA_CLAMP_LEVEL_VPP value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_52_params[i].LNAClampLevel_Vpp = value_read;
        }
      } else {
        DaphneConfig->afe_reg_52_params[current_afe].LNAClampLevel_Vpp = value_read;
      }
      continue;
    }

    if (strstr(str, "ACTIVE_TERMINATION_ENABLE") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid ACTIVE_TERMINATION_ENABLE value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_52_params[i].activeTerminationEnable = value_read;
        }
      } else {
        DaphneConfig->afe_reg_52_params[current_afe].activeTerminationEnable = value_read;
      }
      continue;
    }

    if (strstr(str, "PRESET_ACTIVE_TERMINATION_IMPEDANCE_OHM") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case IMPEDANCE_50_OHMS:
        case IMPEDANCE_100_OHMS:
        case IMPEDANCE_200_OHMS:
        case IMPEDANCE_400_OHMS:
          break;

        default:
          printf("Line %d: Invalid PRESET_ACTIVE_TERMINATION_IMPEDANCE_OHM value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_52_params[i].presetActiveTerminationImpedance = value_read;
        }
      } else {
        DaphneConfig->afe_reg_52_params[current_afe].presetActiveTerminationImpedance = value_read;
      }
      continue;
    }

    if (strstr(str, "PGA_GAIN_DB") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case PGA_GAIN_24_DB:
        case PGA_GAIN_30_DB:
          break;

        default:
          printf("Line %d: Invalid PGA_GAIN_DB value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_51_params[i].PGAGain_dB = value_read;
        }
      } else {
        DaphneConfig->afe_reg_51_params[current_afe].PGAGain_dB = value_read;
      }
      continue;
    }

    if (strstr(str, "PGA_INTEGRATOR_ENABLE") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid PGA_INTEGRATOR_ENABLE value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_51_params[i].PGAIntegratorEnable = value_read;
        }
      } else {
        DaphneConfig->afe_reg_51_params[current_afe].PGAIntegratorEnable = value_read;
      }
      continue;
    }

    if (strstr(str, "PGA_CLAMP_LEVEL_DBFS") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case PGA_CLAMP_0_DBFS:
        case PGA_CLAMP_NEG_2_DBFS:
        case PGA_CLAMP_DISABLED:
          break;

        default:
          printf("Line %d: Invalid PGA_CLAMP_LEVEL_DBFS value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_51_params[i].PGAClampLevel_dBFS = value_read;
        }
      } else {
        DaphneConfig->afe_reg_51_params[current_afe].PGAClampLevel_dBFS = value_read;
      }
      continue;
    }

    if (strstr(str, "LPF_FREQUENCY_MHZ") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case LPF_FREQ_10_MHZ:
        case LPF_FREQ_15_MHZ:
        case LPF_FREQ_20_MHZ:
        case LPF_FREQ_30_MHZ:
          break;

        default:
          printf("Line %d: Invalid LPF_FREQUENCY_MHZ value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_51_params[i].LPFFrequency_MHz = value_read;
        }
      } else {
        DaphneConfig->afe_reg_51_params[current_afe].LPFFrequency_MHz = value_read;
      }
      continue;
    }

    if (strstr(str, "GAIN_V") != NULL) {
      double gain_V;
      read = fscanf(f_ini, "%lf", &gain_V);

      if (gain_V < 0 || gain_V > 1.5) {
        printf("Line %d: Invalid GAIN_V value\n", line_counter);
        continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_gain_V[i] = gain_V;
        }
      } else {
        DaphneConfig->afe_gain_V[current_afe] = gain_V;
      }
      continue;
    }

    if (strstr(str, "ADC_OUTPUT_FORMAT") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case ADC_FORMAT_OFFSET_BINARY:
        case ADC_FORMAT_2COMP:
          break;

        default:
          printf("Line %d: Invalid ADC_OUTPUT_FORMAT value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_4_params[i].ADCOutputFormat = value_read;
        }
      } else {
        DaphneConfig->afe_reg_4_params[current_afe].ADCOutputFormat = value_read;
      }
      continue;
    }

    if (strstr(str, "ADC_OUTPUT_FIRST_BIT") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case ADC_ENDIAN_LSB_FIRST:
        case ADC_ENDIAN_MSB_FIRST:
          break;

        default:
          printf("Line %d: Invalid ADC_OUTPUT_FIRST_BIT value\n", line_counter);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->afe_reg_4_params[i].ADCOutputFirstBit = value_read;
        }
      } else {
        DaphneConfig->afe_reg_4_params[current_afe].ADCOutputFirstBit = value_read;
      }
      continue;
    }

    if (strstr(str, "CHANNEL_OFFSET_GAIN_ENABLE") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          break;

        default:
          printf("Line %d: Invalid CHANNEL_OFFSET_GAIN_ENABLE value\n", line_counter);
          continue;
      }

      if (current_channel == -1) {
        for (i = 0; i < NUMBER_CHANNELS; ++i) {
          DaphneConfig->ch_offset_gain_enable[i] = value_read;
        }
      } else {
        DaphneConfig->ch_offset_gain_enable[current_channel] = value_read;
      }
      continue;
    }

    if (strstr(str, "CHANNEL_OFFSET_VOLTAGE_mV") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      if (value_read < 0 || value_read > 4095) {
        printf("Line %d: Invalid CHANNEL_OFFSET_VOLTAGE_mV value\n", line_counter);
        continue;
      }

      if (current_channel == -1) {
        for (i = 0; i < NUMBER_CHANNELS; ++i) {
          DaphneConfig->ch_offset_voltage_mV[i] = value_read;
        }
      } else {
        DaphneConfig->ch_offset_voltage_mV[current_channel] = value_read;
      }
      continue;
    }

    printf("Line %d: invalid setting: %s\n", line_counter, str);
    res = fgets(str, 1000, f_ini);
  }
  return ret;
}