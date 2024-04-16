#ifndef _serial_port_h
#define _serial_port_h

/* 'open_port(port)' - Open given serial port.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int open_port(const char* port);

/* 'configure_port(&fd)' - Configure serial port given by file descriptor.
 *
 * current configurations are:
 * - 115200 bps
 * - no parity check
 * - 8 character bits
 * - 1 stop bit
 * - raw input (non canonical)
 * - raw output (non processed)
 * - no minimum characters to read
 * - timeout 10 ?
 */
void configure_port(int* file_descriptor);

/*
 * 'send_command(&fd, command)' - Send command (\0 terminated) over serial
 *                                port with MAXIMUM_RETRIES = 3.
 *
 * Returns 0 if success, -1 if error.
 */
int send_command(int* file_descriptor, const char* command);

/*
 * 'read_response(&fd, buffer, BUFFER_SIZE)' -
 * Returns 0 if response ends with "success" or "done".
 */
int read_response(int* fd, char buffer[], const int buffer_size);

#endif