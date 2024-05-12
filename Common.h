#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <stdint.h>

extern const char * program_name;
extern uint8_t verbose;
extern void system_error (const char* operation);
extern void error (const char* cause, const char* message);
extern void error_m (const char* cause, const char* message);
void log_entry(const char * message);


#endif /* COMMON_H */