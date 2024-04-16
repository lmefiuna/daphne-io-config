#include "SerialPort.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int open_port(const char* port)
{
  int file_descriptor;

  file_descriptor = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (file_descriptor == -1)
  {
    fprintf(stderr, "open_port(%s) : %s\n", port, strerror(errno));
    exit(1);
  }

  fcntl(file_descriptor, F_SETFL, 0);

  return (file_descriptor);
}

void configure_port(int* file_descriptor)
{
  struct termios options;

  tcgetattr(*file_descriptor, &options);

  cfsetispeed(&options, B115200); /* Input baudrate 115200 */
  cfsetospeed(&options, B115200); /* Output baudrate 115200 */

  options.c_cflag |= (CLOCAL | CREAD);

  options.c_cflag &= ~PARENB; /* No parity */
  options.c_cflag &= ~CSIZE;  /* Mask the character size bits */
  options.c_cflag |= CS8;     /* 8 character bits */
  options.c_cflag &= ~CSTOPB; /* 1 stop bit */

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* raw input */
  options.c_oflag &= ~OPOST;                          /* raw output */

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 10;

  tcsetattr(*file_descriptor, TCSANOW, &options);
}

int send_command(int* fd, const char* command)
{
  const int MAXIMUM_RETRIES = 3;
  int       tries;
  const int bytes_to_send = (int)strlen(command);
  int       sent_bytes;

  printf("send_command: %s", command);

  for (tries = 0; tries < MAXIMUM_RETRIES; ++tries)
  {
    sent_bytes = write(*fd, command, bytes_to_send);
    if (sent_bytes < bytes_to_send)
    {
      continue;
    }
    return (0);
  }

  fprintf(stderr, "Could not send command %s", command);
  return (-1);
}

int read_response(int* fd, char buffer[], const int buffer_size)
{
  char* buffer_pointer; /* Current char in buffer */
  int   nbytes;         /* Number of bytes read */

  buffer_pointer = buffer;
  while ((nbytes = read(*fd, buffer_pointer, buffer + buffer_size - buffer_pointer - 1)) > 0)
  {
    buffer_pointer += nbytes;

    /* uC response ends in "\n\n\n\n>" */
    if (buffer_pointer - buffer > 5)
    {
      if (buffer_pointer[-5] == '\n' && buffer_pointer[-4] == '\n' && buffer_pointer[-3] == '\n' &&
          buffer_pointer[-2] == '\n' && buffer_pointer[-1] == '>')
      {
        buffer_pointer -= 5;
        break;
      }
    }
  }

  *buffer_pointer = '\0';
  int response_length = strlen(buffer);
  if ((strcmp(&buffer[response_length - 4], "done") == 0) ||
      (strcmp(&buffer[response_length - 7], "success") == 0))
  {
    /* delete first 5 chars: "\n\n>\n\n" */
    char* ps;
    for (ps = buffer; *ps != '\0'; ++ps)
    {
      *ps = *(ps + 5);
    }

    printf("read_response: %s\n", buffer);
    return (0);
  }

  fprintf(
      stderr,
      "read_response: command was not executed succesfully or did not"
      " get proper response. Response: %s\n",
      buffer);
  return (-1);
}