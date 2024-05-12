#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"
#include "CommandBuilder.h"
#include "DaphneConfig.h"
#include "SerialPort.h"

#define BUFFER_SIZE 200
#define SERIAL_PORT_LENGTH 15
#define CONFIG_FILE_LENGTH 170

/* Description of long options for getopt_long. */
static const struct option long_options[] = {
    {"help",        0, NULL, 'h'},
    {"port",        1, NULL, 'p'},
    {"config-file", 1, NULL, 'c'},
    {"verbose",     0, NULL, 'v'},
    {NULL,          0, NULL,  0}};

/* Description of short options for getopt_long. */
static const char * const short_options = "hp:c:v";

/* Usage summary text. */
static const char * const usage_template =
	"Usage : %s -p SERIAL_PORT -c CONFIG_FILE [-v]\n"
	"   -h, --help                      Print this information.\n"
	"   -p, --port /dev/ttyUSB*         Serial port.\n"
  "   -c --config-file /path/to/file  Path to config file for DAPHNE IO.\n"
	"   -v, --verbose                   Print debug messages.\n";

/* Print usage information and exit. If IS_ERROR is nonzero, write to
stderr and use an error exit code. Otherwise, write to stdout and
use a non-error termination code. Does not return. */
static void print_usage(int is_error)
{
	fprintf(is_error ? stderr : stdout, usage_template, program_name);
}

int main(int argc, char* argv[]) {
  
  /* Set defaults for options. */
  int next_option;
  char Config_File[CONFIG_FILE_LENGTH];
  char Serial_Port[SERIAL_PORT_LENGTH];
  
  memset(Config_File, 0, CONFIG_FILE_LENGTH);
  memset(Serial_Port, 0, SERIAL_PORT_LENGTH);

  /* Set values to default */
  program_name = argv[0];

  /* Parse options. */
  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option) {
      /* Print help info */
      case 'h':
        print_usage(0);
        return 0;
        break;

      /* Load serial port */
      case 'p':
        if (strlen(optarg) > SERIAL_PORT_LENGTH) {
          error(optarg, "Serial port name too long.");
        }
        strncpy(Serial_Port, optarg, SERIAL_PORT_LENGTH);
        break;

      /* Config file path */
      case 'c':
        if (strlen(optarg) > CONFIG_FILE_LENGTH) {
          error(optarg, "Config file name too long.");
        }
        strncpy(Config_File, optarg, CONFIG_FILE_LENGTH);
        break;

      /* Enable verbose mode */
      case 'v':
        verbose = 1;
        break;
      
      /* The user specified an invalid option. */
      case '?':
        print_usage(1);

      /* Done with options */
      case -1:
        break;

      default:
        abort();
        break;
    }
  } while (next_option != -1);

  if (strlen(Serial_Port) == 0) {
    error("Serial_Port", "Serial port is mandatory (E.g: -p /dev/ttyUSB0)");
    print_usage(1);
  }

  if (strlen(Config_File) == 0) {
    error("Config_File", "Configuration file is mandatory (E.g: -c /home/user/daphne.config)");
    print_usage(1);
  }

  char buffer[BUFFER_SIZE];

  /* Start program */
  log_entry("DAPHNE Board V1 IO config started.");
  if (verbose)
	{
    fprintf(stdout, "[DEBUG] - Config File:\t\t%s\n", Config_File);
    fprintf(stdout, "[DEBUG] - Serial Port:\t\t%s\n", Serial_Port);
    fprintf(stdout, "[DEBUG] - Debug messages enabled\n");
	}
  DaphneConfig_t DaphneConfig;
  FILE*          f_ini;

  f_ini = fopen(Config_File, "r");
  if (f_ini == NULL) {
    sprintf(buffer, "Could not open config file %s.", Config_File);
    error("Config_File", buffer);
    exit(1);
  }

  ParseConfigFile(f_ini, &DaphneConfig);
  int fd;
  int i;

  fd = open_port(Serial_Port);
  configure_port(&fd);

  for (i=0; i<NUMBER_AFES; ++i){

    if (DaphneConfig.enableConfigureAFE[i] == 0) {
      if (verbose) {
        fprintf(stdout, "[DEBUG] - Skipping AFE %d", i);
      }
      continue;
    }

    sprintf(buffer, "Configuring AFE %d", i);
    log_entry(buffer);

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
      if (verbose) {
        fprintf(stdout, "[DEBUG] - Skipping channel %d", i);
      }
      continue;
    }

    sprintf(buffer, "Configuring channel %d", i);
    log_entry(buffer);

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