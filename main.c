#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "CommandBuilder.h"
#include "DaphneConfig.h"
#include "SerialPort.h"

const char* program_name;

#define BUFFER_SIZE 200

void print_usage(FILE* stream, int exit_code) {
  fprintf(stream, "Usage : %s -p SERIAL_PORT -c CONFIG_FILE\n", program_name);
  fprintf(
      stream,
      "Options:\n"
      " -h --help                    Display this usage information.\n"
      " -p --port filename           Serial port to use.\n"
      " -c --config-file filepath    Path to config file for DAPHNE IO.\n");
  exit(exit_code);
}

int main(int argc, char* argv[]) {
  int                 next_option;
  const char* const   short_options = "hp:c:";
  const struct option long_options[] = {
      {"help", 0, NULL, 'h'},
      {"port", 1, NULL, 'p'},
      {"config-file", 1, NULL, 'c'},
      {NULL, 0, NULL, 0}};

  const char* config_file_path = NULL;
  const char* serial_port_path = NULL;

  program_name = argv[0];

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option) {
      case 'h':
        print_usage(stdout, 0);
        break;

      case 'p':
        serial_port_path = optarg;
        break;

      case 'c':
        config_file_path = optarg;
        break;

      case '?':
        print_usage(stderr, 1);

      case -1:
        break;

      default:
        abort();
    }
  } while (next_option != -1);

  if (serial_port_path == NULL) {
    fprintf(stderr, "Error: Serial port is mandatory (E.g: -p /dev/ttyUSB0)\n");
    print_usage(stderr, 1);
  }

  if (config_file_path == NULL) {
    fprintf(stdout, "Error: Configuration file is mandatory (E.g: -c /home/user/daphne.config)\n");
    print_usage(stderr, 1);
  }

  DaphneConfig_t DaphneConfig;
  FILE*          f_ini;
  char           buffer[BUFFER_SIZE];

  f_ini = fopen(config_file_path, "r");
  if (f_ini == NULL) {
    fprintf(stderr, "Could not open config file %s.\n", config_file_path);
    exit(2);
  }

  ParseConfigFile(f_ini, &DaphneConfig);

  int fd;
  int i;

  fd = open_port(serial_port_path);
  configure_port(&fd);

  for (i=0; i<NUMBER_AFES; ++i){

    // printf("afe %d, enable configure: %d\n", i, DaphneConfig.enableConfigureAFE[i]);
    if (DaphneConfig.enableConfigureAFE[i] == 0) {
      // printf("Skipping afe %d\n", i);
      continue;
    }

    printf("Configuring AFE %d\n", i);

    configureAFEReg52(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_52_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg51(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_51_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg4(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_4_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg1(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_1_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg21(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_21_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg33(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_33_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    configureAFEReg59(buffer, (AFE_NUMBER_t)i, DaphneConfig.afe_reg_59_params[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    applyAFEGain_V(buffer, AFE_0, DaphneConfig.afe_gain_V[0]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);
  }

    for (i=0; i<NUMBER_CHANNELS; ++i){
    if (DaphneConfig.enableConfigureChannel[i] == 0) {
      continue;
    }

    printf("Configuring Channel %d\n", i);

    enableChannelOffsetGain(buffer, CHANNEL_0, DaphneConfig.ch_offset_gain_enable[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);

    applyChannelOffsetVoltage_mV(buffer, CHANNEL_0, DaphneConfig.ch_offset_voltage_mV[i]);
    if (send_command(&fd, buffer) == -1) {
      fprintf(stderr, "Could not send command %s.\n", buffer);
    }
    read_response(&fd, buffer, BUFFER_SIZE);    
    }

  return 0;
}