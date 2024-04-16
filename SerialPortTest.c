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

  const char serial_port[] = "/dev/ttyUSB0";
  const char command[] = "CFG FPGA RESET\r\n";

  fd = open_port(serial_port);
  configure_port(&fd);

  char  buffer[255];    /* Input buffer */
  char *buffer_pointer; /* Current char in buffer */
  int   nbytes;         /* Number of bytes read */

  /* wait for uC to stabilize properly */
  // sleep(2);

  int bytes_to_send = sizeof(command) / sizeof(command[0]);

  int sent_bytes = write(fd, command, bytes_to_send);
  if (sent_bytes < bytes_to_send)
  {
    printf("Error al enviar datos\n");
  }
  else
  {
    buffer_pointer = buffer;
    while ((nbytes = read(fd, buffer_pointer, buffer + sizeof(buffer) - buffer_pointer - 1)) > 0)
    {
      buffer_pointer += nbytes;

      /* uC response ends in "\r\n\n\r>" */
      if (buffer_pointer - buffer > 5)
      {
        if (buffer_pointer[-5] == '\n' && buffer_pointer[-4] == '\n' &&
            buffer_pointer[-3] == '\n' && buffer_pointer[-2] == '\n' && buffer_pointer[-1] == '>')
        {
          buffer_pointer -= 5;
          break;
        }
      }
    }

    *buffer_pointer = '\0';
    printf("Received: %s\n", buffer + 5);
  }

  close(fd);

  return (0);
}