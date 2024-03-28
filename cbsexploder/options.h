#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

typedef struct _OPTIONS {
	BOOL debug = FALSE;
	BOOL localStack = FALSE;
	BOOL enumPkgs = FALSE;
	BOOL help = FALSE;
	LPCWSTR batchPath = NULL;
	LPCWSTR pkgPath = NULL;
	LPCWSTR logPath = L"C:\\cbsexploder.log";
	LPCWSTR winDir = NULL;
	CbsInstallState intendedPkgState = CbsInstallState::Unknown;
} OPTIONS;

extern OPTIONS g_options;