// cbsexploder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CbsApi.h"
#include "options.h"
#include "logging.h"
#include <wrl/client.h>
#include <stdio.h>

using Microsoft::WRL::ComPtr;

BOOL g_Debug;
LPCWSTR g_LogFile;

PCBS_CORE_INITIALIZE CbsCoreInitialize;
PCBS_CORE_SET_STATE CbsCoreSetState;
PCBS_CORE_FINALIZE CbsCoreFinalize;
PCBS_CORE_SET_STATE SetTestMode;
PWDS_SETUP_LOG_MESSAGEW WdsSetupLogMessageW;
PCONSTRUCT_PARTIAL_MSG_VW ConstructPartialMsgVW;
PCURRENT_IP CurrentIP;

IMalloc* g_alloc;

int __stdcall Ret1(int x) { return 1; }
void Nop() { }

int LoadServicingStackLocal() {
    HMODULE cbscore = LoadLibraryA("cbscore.dll");

    ASRT_NL(cbscore, "ERROR: Failed to load CbsCore.dll");

    CbsCoreInitialize = (PCBS_CORE_INITIALIZE)GetProcAddress(cbscore, "CbsCoreInitialize");
    CbsCoreFinalize = (PCBS_CORE_FINALIZE)GetProcAddress(cbscore, "CbsCoreFinalize");
    CbsCoreSetState = (PCBS_CORE_SET_STATE)GetProcAddress(cbscore, "CbsCoreSetState");

    ASRT_NL(CbsCoreInitialize && CbsCoreFinalize && CbsCoreSetState, "ERROR: Failed to load required functions from CbsCore.dll");

    return 0;
}

int LoadWds() {
    HMODULE wdscore = GetModuleHandleA("wdscore.dll");

    ASRT_NL(wdscore, "ERROR: Failed to load CbsCore.dll");

    WdsSetupLogMessageW = (PWDS_SETUP_LOG_MESSAGEW)GetProcAddress(wdscore, "WdsSetupLogMessageW");
    ConstructPartialMsgVW = (PCONSTRUCT_PARTIAL_MSG_VW)GetProcAddress(wdscore, "ConstructPartialMsgVW");
    CurrentIP = (PCURRENT_IP)GetProcAddress(wdscore, "CurrentIP");

    ASRT_NL(WdsSetupLogMessageW && ConstructPartialMsgVW && CurrentIP, "ERROR: Failed to load required functions from wdscore.dll");

    return 0;
}

int wmain(int argc, LPCWSTR* argv)
{
    g_Debug = TRUE;
    g_LogFile = L"C:\\test.log";

    printf("cbsexploder 0.0.0 by witherornot\n");

    SetEnvironmentVariableW(L"COMPONENT_BASED_SERVICING_LOGFILE", g_LogFile);

    CHK_NL_HR(CoInitialize(NULL), "ERROR: CoInitialize Failed [HR = %08x]");
    CHK_NL_HR(CoGetMalloc(1, &g_alloc), "ERROR: CoGetMalloc Failed [HR = %08x]");

    if (LoadServicingStackLocal()) {
        return 1;
    }

    ComPtr<IClassFactory> cbsFactory;
    CHK_NL_HR(CbsCoreInitialize(g_alloc, &Ret1, &Nop, &Nop, &Nop, &Nop, &Nop, &cbsFactory), "ERROR: Failed to initialize CbsCore [HR = %08x]");

    if (LoadWds()) {
        return 1;
    }

    LDBG("Initialized CbsCore");

    ComPtr<ICbsSession> cbsSess;
    CHK_HR(cbsFactory->CreateInstance(NULL, __uuidof(ICbsSession), &cbsSess), "ERROR: Failed to create CBS Session [HR = %08x]");

    LPCWSTR bootDrive = L"X:\\image";
    LPCWSTR winDir = L"X:\\image\\Windows";

    CHK_HR(cbsSess->Initialize(CbsSessionOption::None, L"CbsExploder", bootDrive, winDir), "ERROR: Failed to initialize CBS Session (bootDrive = \"%s\", winDir = \"%s\") [HR = %08x]", bootDrive, winDir);
    LDBG("Initialized CBS Session (bootDrive = \"%s\", winDir = \"%s\")", bootDrive, winDir);

    LPCWSTR pkgPath = L"microsoft-windows-professionaledition~31bf3856ad364e35~amd64~~6.3.9600.16384.mum";

    ComPtr<ICbsPackage> pkg;
    CHK_HR(cbsSess->CreatePackage(0, CbsPackageType::ExpandedWithMum, pkgPath, L"X:\\temp", &pkg), "ERROR: Failed to create package (pkgPath = \"%s\") [HR = %08x]", pkgPath);

    LPWSTR pkgIdent;
    pkg->GetProperty(CbsPackageProperty::IdentityString, &pkgIdent);

    LDBG("Loaded package %s", pkgIdent);

    CbsInstallState curState, appState;
    CHK_HR(pkg->EvaluateApplicability(0, &appState, &curState), "ERROR: Failed to evaluate applicability for %s [HR = %08x]", pkgIdent);

    ASRT(curState != CbsInstallState::Installed, "ERROR: Package %s is already installed", pkgIdent);
    ASRT(appState == CbsInstallState::Installed, "ERROR: Package %s is not applicable to the current image", pkgIdent);

    LDBG("Installing package %s", pkgIdent);

    pkg->InitiateChanges(0, CbsInstallState::Installed, NULL);

    CbsRequiredAction reqAct;
    CHK_HR(cbsSess->FinalizeEx(0, &reqAct), "ERROR: Failed to finalize CBS session [HR = %08x]");

    if (reqAct == CbsRequiredAction::Reboot) {
        LOG("CBS has indicated that a reboot is required");
    }

    return 0;
}
