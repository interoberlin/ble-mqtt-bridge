#ifndef DEBUG_H_
#define DEBUG_H_

typedef enum { DEBUG_NONE=0, DEBUG_NORMAL, DEBUG_MORE, DEBUG_MOST, DEBUG_ALL=9} debug_flags;

extern debug_flags debug_flag;
extern bool config_check_flag;

#endif