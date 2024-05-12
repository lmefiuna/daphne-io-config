#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include "Common.h"

const char * program_name;
uint8_t verbose;

/* Generate an error message for a system errno. */
void system_error (const char* operation)
{
    error(operation, strerror (errno));
}

/* Print a critical error message to stderr in standard format and stop execution. */
void error (const char* cause, const char* message)
{
    fprintf(stderr, "%s: critical error: (%s) %s\n\r", program_name, cause, message);
    exit (1);
}

/* Print an error message to stderr in standard format and continue execution, errors after this 
function must be handled */
void error_m (const char * cause, const char * message)
{
    fprintf(stderr, "%s: error: (%s) %s\n\r", program_name, cause, message);    
}

/* Print a logging message to stdout for output redirection before execution */
void log_entry(const char * message)
{
	char    Datetime[20];
    time_t  now;
    struct 	tm * tm;

    now = time (NULL);
    tm	= localtime(&now);
    strftime(Datetime, 20, "%Y-%m-%d %H:%M:%S", tm);
    fprintf(stdout, "[%s] - %s\n\r", Datetime, message);
}
