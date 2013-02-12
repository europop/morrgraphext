#pragma once

#include "mwse/WinHeader.h"

#define FATALERROR(a) { LOG::log(a); ExitProcess(-1); }
#define MSG(a) { LOG::log(a); }

struct LOG
{
	LOG();
	virtual ~LOG();
	static bool open(const char* filename);
	static int log(const char* fmt, ...);
	static int logline(const char* fmt, ...);
	static int logbinary(void* addr, int size);
	static void close();
private:
	static HANDLE handle;
};
