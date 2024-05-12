#include "SerialPort.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "Common.h"

char log_buffer[200];

int open_port(const char* port) {
  if (verbose) {
    fprintf(stdout, "[DEBUG] - SerialPort - Opening serial port %s\n", port);
  }

  int file_descriptor;

  file_descriptor = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (file_descriptor == -1) {
    char tmp[100];
    sprintf(tmp, "open_port(%s)", port);
    system_error(tmp);
    exit(1);
  }

  fcntl(file_descriptor, F_SETFL, 0);

  return (file_descriptor);
}

void configure_port(int* file_descriptor) {
  if (verbose) {
    fprintf(stdout, "[DEBUG] - SerialPort - Configuring serial port.\n");
  }
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

int send_command(int* fd, const char* command) {
  const int MAXIMUM_RETRIES = 3;
  int       tries;
  const int bytes_to_send = (int)strlen(command);
  int       sent_bytes;

  /* TODO: remove redundant \r\n in log entry */
  sprintf(log_buffer, "Serial Port - send_command:\t%s", command);
  *(log_buffer + strlen(log_buffer) - 2) = '\0';
  log_entry(log_buffer);
  //printf("send_command:\t%s", command);

  for (tries = 0; tries < MAXIMUM_RETRIES; ++tries) {
    sent_bytes = write(*fd, command, bytes_to_send);
    if (sent_bytes < bytes_to_send) {
      continue;
    }
    return (0);
  }

  sprintf(log_buffer, "Could not send command %s", command);
  error_m("SerialPort", log_buffer);
  return (-1);
}

int read_response(int* fd, char buffer[], const int buffer_size) {
  char* buffer_pointer; /* Current char in buffer */
  int   nbytes;         /* Number of bytes read */

  memset(buffer, 0, 200);
  buffer_pointer = buffer;
  while ((nbytes = read(*fd, buffer_pointer, buffer + buffer_size - buffer_pointer - 1)) > 0)
  {
    buffer_pointer += nbytes;
  }

  int response_length = strlen(buffer);
  int i;

  for (i=0;i<response_length;++i) {
    if (buffer[i] == '\r' || buffer[i] == '\n' || buffer[i] == '>') {
      buffer[i] = ' ';
    }
  }

  if (verbose) {
    fprintf(stdout, "[DEBUG] - Serial Port - read_response - Response length:%d\n", response_length);
    fprintf(stdout, "[DEBUG] - Serial Port - read_response - Response:\t%s\n", buffer);
  }
  // printf("response length: %d\n", response_length);
  if ((strstr(buffer, "done") != NULL) ||
      (strstr(buffer, "success") != NULL) ||
      (strstr(buffer, "updated") != NULL))
    {

    // /* delete last 5 chars: \n\n\n\n> */
    // if (buffer[response_length-1] == '>') {
    //   *(buffer_pointer-5) = '\0';
    // }
    // /* delete first 5 chars: "\n\n>\n\n" */
    // char* ps;
    // for (ps = buffer; *ps != '\0'; ++ps) {
    //   *ps = *(ps + 5);
    // }

    // sprintf(log_buffer, "Serial Port - read_response: %s, last char %d %c", buffer, buffer[strlen(buffer)], 47);
    // if (log_buffer[-1] == '\n')
    log_entry("Serial_Port - Command executed successfully.");
    return (0);
  }

  sprintf(
      log_buffer,
      "read_response:\tcommand was not executed succesfully or did not"
      " get proper response. Response: %s",
      buffer);
  error_m("SerialPort", log_buffer);
  return (-1);
}