#include <stdio.h>

#include "DaphneConfig.h"

const char CONFIG_FILE_PATH[] =
    "/home/caleb/caleb/fiuna/pasantia-tfg/mango-digitizer-daphne-v1/daphne-io-config/"
    "example.config";

int main()
{
  DaphneConfig_t DaphneConfig;
  FILE          *f_ini;

  f_ini = fopen(CONFIG_FILE_PATH, "r");
  if (f_ini == NULL)
  {
    fprintf(stderr, "could not find config file %s", CONFIG_FILE_PATH);
    return 1;
  }
  ParseConfigFile(f_ini, &DaphneConfig);
  return 0;
}