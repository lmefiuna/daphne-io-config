#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "DaphneConfig.h"

const char* program_name;

void print_usage(FILE* stream, int exit_code) {
  fprintf(stream, "Usage : %s -s SERIAL_PORT [-c CONFIG_FILE]\n", program_name);
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
    fprintf(stderr, "Error: Serial port is mandatory (E.g: -s /dev/ttyUSB0)\n");
    print_usage(stderr, 1);
  }

  if (config_file_path == NULL) {
    fprintf(stdout, "Error: Configuration file is mandatory (E.g: -c /home/user/daphne.config)\n");
    print_usage(stderr, 1);
  }

  DaphneConfig_t DaphneConfig;
  FILE*          f_ini;

  f_ini = fopen(config_file_path, "r");
  if (f_ini == NULL) {
    fprintf(stderr, "Could not open config file %s.\n", config_file_path);
    exit(2);
  }

  ParseConfigFile(f_ini, &DaphneConfig);

  return 0;
}