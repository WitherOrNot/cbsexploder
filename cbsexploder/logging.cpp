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
	
	VConLog(fmt, va_args);

	LPVOID partMsg = ConstructPartialMsgVW(level, fmt, va_args);
	result = WdsSetupLogMessageW(partMsg, source, L"D", 0, 1, __FILEW__, __FUNCTIONW__, CurrentIP(), 0, NULL, 0);

	va_end(va_args);
	return result;
}

LPCWSTR PkgStateAsStr(CbsInstallState state) {
    switch (state) {
        case CbsInstallState::Absent:
            return L"Uninstalled";
        case CbsInstallState::UninstallRequested:
            return L"Uninstall Requested";
        case CbsInstallState::Installed:
            return L"Installed";
        case CbsInstallState::PartiallyInstalled:
            return L"Partially Installed";
        case CbsInstallState::InstallRequested:
            return L"Install Requested";
        case CbsInstallState::Staging:
            return L"Staging";
        case CbsInstallState::Resolved:
            return L"Resolved";
        case CbsInstallState::Resolving:
            return L"Resolving";
        case CbsInstallState::Staged:
            return L"Staged";
        case CbsInstallState::Superseded:
            return L"Superseded";
        case CbsInstallState::Permanent:
            return L"Permanent";
        case CbsInstallState::Invalid_Permanent:
            return L"Invalid Permanent";
        case CbsInstallState::Cancel:
            return L"Cancel";
        case CbsInstallState::Default:
            return L"Default";
        case CbsInstallState::Invalid:
            return L"Invalid";
        case CbsInstallState::Invalid_Installed:
            return L"Invalid Installed";
        case CbsInstallState::Invalid_Staged:
            return L"Invalid Staged";
        case CbsInstallState::Unknown:
        default:
            return L"Unknown";
    }
}

LPCWSTR CbsOpStageAsStr(CbsOperationStage stage) {
    switch (stage) {
        case CbsOperationStage::Waiting:
            return L"Waiting";
        case CbsOperationStage::Planning:
            return L"Planning";
        case CbsOperationStage::Downloading:
            return L"Downloading";
        case CbsOperationStage::Extracting:
            return L"Extracting";
        case CbsOperationStage::Resolving:
            return L"Extracting";
        case CbsOperationStage::Staging:
            return L"Staging";
        case CbsOperationStage::Installing:
        case CbsOperationStage::InstallingEx:
            return L"Installing";
        case CbsOperationStage::ReservicingLCU:
            return L"Reservicing LCU";
        default:
            return L"Unknown";
    }
}