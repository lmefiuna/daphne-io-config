#ifndef _serial_port_h
#define _serial_port_h

/* 'open_port(port)' - Open given serial port.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int open_port(const char* port);

/* 'configure_port(fd)' - Configure serial port given by file descriptor.
 *
 * current configurations are:
 * - 115200 bps
 * - no parity check
 * - 8 character bits
 * - 1 stop bit
 * - raw input (non canonical)
 * - raw output (non processed)
 * - no minimum characters to read
 */
void configure_port(int* file_descriptor);

void send_command();

#endif