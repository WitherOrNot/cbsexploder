#include "logging.h"

void VConLog(LPCSTR fmt, va_list va_args) {
	int len = MultiByteToWideChar(CP_UTF8, 0, fmt, -1, NULL, 0);
	LPWSTR wfmt = (LPWSTR)malloc((len + 2) * sizeof(WCHAR));

	if (!wfmt) {
		return;
	}

	memset(wfmt, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, fmt, -1, wfmt, len);
	wfmt[len - 1] = L'\n';
	wfmt[len] = L'\0';
	vwprintf((LPCWSTR)wfmt, va_args);
	free(wfmt);
}

void ConLog(LPCSTR fmt, ...) {
	va_list va_args;
	va_start(va_args, fmt);
	VConLog(fmt, va_args);
	va_end(va_args);
}

HRESULT WdsLog(WdsLogSource source, WdsLogLevel level, LPCSTR fmt, ...) {
	va_list va_args;
	HRESULT result;

	va_start(va_args, fmt);

	if (g_Debug) {
		VConLog(fmt, va_args);
	}

	LPVOID partMsg = ConstructPartialMsgVW(level, fmt, va_args);
	result = WdsSetupLogMessageW(partMsg, source, L"D", 0, 1, __FILEW__, __FUNCTIONW__, CurrentIP(), 0, NULL, 0);

	va_end(va_args);
	return result;
}