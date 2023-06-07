#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef DEBUG_LEVEL
# define DEBUG_PRINT(level, ...) ((int) level < (int) DEBUG_LEVEL) ? 0 : printf(__VA_ARGS__);
#else
# define DEBUG_PRINT(level, ...) /* Do nothing */
#endif

#endif

