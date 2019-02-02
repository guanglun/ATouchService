#ifndef __LOG_H__
#define __LOG_H__

#define LOG_PRINTF_TIME     0
#define LOG_PRINTF          1

#if LOG_PRINTF_TIME
#define LOG (printf("%s:%u %s:%s:\t", __FILE__, __LINE__, __DATE__, __TIME__), printf)
#elif LOG_PRINTF
#define LOG  printf
#else
#define LOG /\
/LOG
#endif

#endif