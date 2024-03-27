#pragma once

#include "CbsApi.h"
#include "options.h"
#include <strsafe.h>

#define LOG(fmt,...) WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelInfo, fmt, ##__VA_ARGS__)
#define LDBG(fmt,...) if (g_options.debug) {WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelInfo, fmt, ##__VA_ARGS__);}
#define LERR(fmt,...) WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelError, fmt, ##__VA_ARGS__)
#define LWRN(fmt,...) WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelWarning, fmt, ##__VA_ARGS__)
#define CHK_HR(hr,fmt,...) if(FAILED(hr)) {WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelError, fmt, ##__VA_ARGS__, hr); return 1;}
#define CHK_NL_HR(hr,fmt,...) if(FAILED(hr)) {ConLog(fmt, ##__VA_ARGS__, hr); return 1;}
#define ASRT(cond,fmt,...) if(!(cond)) {WdsLog(WdsLogSource::WdsLogSourceDISM, WdsLogLevel::WdsLogLevelError, fmt, ##__VA_ARGS__); return 1;}
#define ASRT_NL(cond,fmt,...) if(!(cond)) {ConLog(fmt,##__VA_ARGS__); return 1;}

HRESULT WdsLog(WdsLogSource source, WdsLogLevel level, LPCSTR fmt, ...);
void ConLog(LPCSTR fmt, ...);