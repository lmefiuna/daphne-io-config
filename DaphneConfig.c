#include "DaphneConfig.h"

#include <string.h>

#include "Common.h"

void SetDefaultConfiguration(DaphneConfig_t *DaphneConfig) {
  if (verbose) {
    fprintf(stdout, "[DEBUG] - Setting default configuration to all AFEs and channels.\n");
  }

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
        if (verbose) {
          fprintf(
              stdout, "[DEBUG] - ParseConfigFile - Line %d:\t Section ****** COMMON ******\n",
              line_counter);
        }
        continue;
      }
      if (strstr(str, "AFE")) {
        sscanf(str + 1, "AFE%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_AFES) {
          sprintf(str, "Line %d:\t Invalid AFE number %d", line_counter, value_read);
          error("ParseConfigFile", str);
        } else {
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t Section ****** AFE%d ******\n",
                line_counter, value_read);
          }
          current_afe = value_read;
        }
      } else if (strstr(str, "CHANNEL")) {
        sscanf(str + 1, "CHANNEL%d", &value_read);
        if (value_read < 0 || value_read >= NUMBER_CHANNELS) {
          sprintf(str, "Line %d:\t Invalid channel number %d", line_counter, value_read);
          error("ParseConfigFile", str);
        } else {
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t Section ****** CHANNEL%d ******\n",
                line_counter, value_read);
          }
          current_channel = value_read;
        }
      } else {
        sprintf(str, "Line %d:\t Invalid section identifier", line_counter);
        error("ParseConfigFile", str);
      }
      continue;
    }

    if (strstr(str, "LNA_GAIN_DB") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case LNA_GAIN_12_DB:
        case LNA_GAIN_18_DB:
        case LNA_GAIN_24_DB:
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t LNA_GAIN_DB %d\n", line_counter,
                value_read);
          }
          break;
        default:
          sprintf(str, "Line %d:\t Invalid LNA_GAIN_DB value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t CONFIGURE_AFE %d\n", line_counter,
                value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid CONFIGURE_AFE value %d", line_counter, value_read);
          error("ParseConfigFile", str);
          continue;
      }

      if (current_afe == -1) {
        for (i = 0; i < NUMBER_AFES; ++i) {
          DaphneConfig->enableConfigureAFE[i] = value_read;
        }
      } else {
        DaphneConfig->enableConfigureAFE[current_afe] = value_read;
      }
      continue;
    }

    if (strstr(str, "CONFIGURE_CHANNEL") != NULL) {
      read = fscanf(f_ini, "%d", &value_read);

      switch (value_read) {
        case 0:
        case 1:
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t CONFIGURE_CHANNEL %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid CONFIGURE_CHANNEL value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t LNA_INTEGRATOR_ENABLE %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(
              str, "Line %d:\t Invalid LNA_INTEGRATOR_ENABLE value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t LNA_CLAMP_LEVEL_VPP %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid LNA_CLAMP_LEVEL_VPP value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t ACTIVE_TERMINATION_ENABLE %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid ACTIVE_TERMINATION_ENABLE value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout,
                "[DEBUG] - ParseConfigFile - Line %d:\t PRESET_ACTIVE_TERMINATION_IMPEDANCE_OHM "
                "%d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(
              str, "Line %d:\t Invalid PRESET_ACTIVE_TERMINATION_IMPEDANCE_OHM value %d",
              line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t PGA_GAIN_DB %d\n", line_counter,
                value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid PGA_GAIN_DB value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t PGA_INTEGRATOR_ENABLE %d\n", line_counter,
                value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid PGA_INTEGRATOR_ENABLE value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t PGA_CLAMP_LEVEL_DBFS %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(
              str, "Line %d:\t Invalid PGA_CLAMP_LEVEL_DBFS value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t LPF_FREQUENCY_MHZ %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(
              str, "Line %d:\t Invalid PGA_CLAMP_LEVEL_DBFS value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
        sprintf(str, "Line %d:\t Invalid GAIN_V value %lf", line_counter, gain_V);
        error("ParseConfigFile", str);
        continue;
      }

      if (verbose) {
        fprintf(
            stdout, "[DEBUG] - ParseConfigFile - Line %d:\t GAIN_V %.2lf\n",
            line_counter, gain_V);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t ADC_OUTPUT_FORMAT %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid ADC_OUTPUT_FORMAT value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t ADC_OUTPUT_FIRST_BIT %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(str, "Line %d:\t Invalid ADC_OUTPUT_FIRST_BIT value %d", line_counter, value_read);
          error("ParseConfigFile", str);
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
          if (verbose) {
            fprintf(
                stdout, "[DEBUG] - ParseConfigFile - Line %d:\t CHANNEL_OFFSET_GAIN_ENABLE %d\n",
                line_counter, value_read);
          }
          break;

        default:
          sprintf(
              str, "Line %d:\t Invalid CHANNEL_OFFSET_GAIN_ENABLE value %d", line_counter,
              value_read);
          error("ParseConfigFile", str);
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
        sprintf(
            str, "Line %d:\t Invalid CHANNEL_OFFSET_VOLTAGE_mV value %d", line_counter, value_read);
        error("ParseConfigFile", str);
        continue;
      }

      if (verbose) {
        fprintf(
            stdout, "[DEBUG] - ParseConfigFile - Line %d:\t CHANNEL_OFFSET_VOLTAGE_mV %d\n",
            line_counter, value_read);
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

    char tmp[100];
    memcpy(tmp, str, 100);
    sprintf(str, "Line %d: Invalid parameter %s", line_counter, tmp);
    error("ParseConfigFile", str);
    // printf("Line %d:\t invalid setting: %s\n", line_counter, str);
    res = fgets(str, 1000, f_ini);
  }
  return ret;
}