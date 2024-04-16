#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "SerialPort.h"

int main()
{
  int fd;

  const char  serial_port[] = "/dev/ttyUSB0";
  const char *commands[] = {
      "WR AFE 0 REG 52 V 17216\r\n\0", "WR AFE 0 REG 51 V 8\r\n\0", "WR AFE 0 REG 4 V 24\r\n\0"};

  fd = open_port(serial_port);
  configure_port(&fd);

  const int BUFFER_SIZE = 127;
  char      buffer[BUFFER_SIZE];

  const int TOTAL_COMMANDS = sizeof(commands) / sizeof(commands[0]);
  int       i;

  for (i = 0; i < TOTAL_COMMANDS; ++i)
  {
    if (send_command(&fd, commands[i]) == -1)
    {
      continue;
    }
    read_response(&fd, buffer, BUFFER_SIZE);
  }

  close(fd);

  return (0);
}