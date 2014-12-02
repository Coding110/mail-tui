#ifndef _HEDADER_LOG_
#define _HEDADER_LOG_

#include <stdio.h>

#define LOG_BUF_SIZE 2048
enum E_LOG_LEVEL
{
	E_LOG_NONE = 0,
	E_LOG_ERROR,
	E_LOG_WARNNING,
	E_LOG_INFO,
	E_LOG_DEBUG
};

enum E_LOG_MODEL
{
	M_LOG_FILE = 0,
	M_LOG_DLG,
};

typedef void(*LoggingHandle)(char *logstr, void *arg);

int OpenLog(const char *logfile, int log_level);
int Logging(int level, const char *format, ...);
int CloseLog();
int SetLogModel(int model, LoggingHandle flhd, void *arg);

char *gettime(char *timestr);

#endif
