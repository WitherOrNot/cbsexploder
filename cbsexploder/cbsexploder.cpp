#include "CbsApi.h"
#include "options.h"
#include "logging.h"
#include <wrl/client.h>
#include <Shlwapi.h>
#include <stdio.h>

#pragma comment(lib, "shlwapi.lib")

using Microsoft::WRL::ComPtr;

OPTIONS g_options;

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

int ProcessOptions(int argc, LPCWSTR* argv) {
    for (int i = 0; i < argc; i++) {
        LPCWSTR arg = argv[i];

        if (!wcsncmp(arg, L"/dbg", 4)) {
            g_options.debug = TRUE;
        }
        else if (!wcsncmp(arg, L"/sp", 3)) {
            ASRT_NL(g_options.intendedPkgState == CbsInstallState::Unknown, "ERROR: Multiple package operations specified");
            g_options.intendedPkgState = CbsInstallState::Staged;
        }
        else if (!wcsncmp(arg, L"/ip", 3)) {
            ASRT_NL(g_options.intendedPkgState == CbsInstallState::Unknown, "ERROR: Multiple package operations specified");
            g_options.intendedPkgState = CbsInstallState::Installed;
        }
        else if (!wcsncmp(arg, L"/up", 3)) {
            ASRT_NL(g_options.intendedPkgState == CbsInstallState::Unknown, "ERROR: Multiple package operations specified");
            g_options.intendedPkgState = CbsInstallState::Absent;
        }
        else if (!wcsncmp(arg, L"/ep", 3)) {
            ASRT_NL(g_options.intendedPkgState == CbsInstallState::Unknown, "ERROR: Option /ep is incompatible with package operations");
            g_options.noAct = TRUE;
        }
        else if (!wcsncmp(arg, L"/o", 2)) {
            LPCWSTR val = wcschr(arg, L':');
            ASRT_NL(val, "ERROR: Invalid argument %s", arg);
            g_options.winDir = val + 1;
        }
        else if (!wcsncmp(arg, L"/m", 2)) {
            LPCWSTR val = wcschr(arg, L':');
            ASRT_NL(val, "ERROR: Invalid argument %s", arg);
            g_options.pkgPath = val + 1;
        }
        else if (!wcsncmp(arg, L"/log", 4)) {
            LPCWSTR val = wcschr(arg, L':');
            ASRT_NL(val, "ERROR: Invalid argument %s", arg);
            g_options.logPath = val + 1;
        }
        else if (!wcsncmp(arg, L"/ls", 3)) {
            g_options.localStack = TRUE;
        }
    }

    ASRT_NL(g_options.winDir, "ERROR: No offline Windows directory path specified");
    ASRT_NL(g_options.intendedPkgState != CbsInstallState::Unknown, "ERROR: No package operation specified");
    ASRT_NL(g_options.pkgPath, "ERROR: No package path specified");

    return 0;
}

LPCWSTR PkgStateAsStr(CbsInstallState state) {
    switch (state) {
        case CbsInstallState::Absent:
            return L"Uninstalled";
            break;
        case CbsInstallState::Installed:
            return L"Installed";
            break;
        case CbsInstallState::PartiallyInstalled:
            return L"Partially Installed";
            break;
        case CbsInstallState::Staged:
            return L"Staged";
            break;
        case CbsInstallState::Invalid:
            return L"Invalid";
            break;
        case CbsInstallState::Invalid_Installed:
            return L"Invalid Installed";
            break;
        case CbsInstallState::Invalid_Staged:
            return L"Invalid Staged";
            break;
        case CbsInstallState::Unknown:
        default:
            return L"Unknown";
            break;
    }
}

int wmain(int argc, LPCWSTR* argv)
{
    printf("cbsexploder 0.0.1 by witherornot\n");

    if (ProcessOptions(argc, argv)) {
        return 1;
    }

    SetEnvironmentVariableW(L"COMPONENT_BASED_SERVICING_LOGFILE", g_options.logPath);

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

    WCHAR bootDrive[MAX_PATH] = { 0 };
    LPCWSTR winDir = g_options.winDir;

    LPCWSTR winPart = StrStrIW(winDir, L"Windows");
    ASRT(winPart, "ERROR: Invalid offline Windows directory %s", winDir);

    ULONG64 bootLen = winPart - winDir;
    wcsncpy_s(bootDrive, winDir, bootLen - 1);

    CHK_HR(cbsSess->Initialize(CbsSessionOption::None, L"CbsExploder", bootDrive, winDir), "ERROR: Failed to initialize CBS Session (bootDrive = \"%s\", winDir = \"%s\") [HR = %08x]", bootDrive, winDir);
    LDBG("Initialized CBS Session (bootDrive = \"%s\", winDir = \"%s\")", bootDrive, winDir);

    WCHAR fullPkgPath[MAX_PATH];
    GetFullPathNameW(g_options.pkgPath, MAX_PATH, fullPkgPath, NULL);

    ComPtr<ICbsPackage> pkg;
    CHK_HR(cbsSess->CreatePackage(0, CbsPackageType::ExpandedWithMum, fullPkgPath, NULL, &pkg), "ERROR: Failed to create package (pkgPath = \"%s\") [HR = %08x]", fullPkgPath);

    LPWSTR pkgIdent;
    pkg->GetProperty(CbsPackageProperty::IdentityString, &pkgIdent);

    LDBG("Loaded package %s", pkgIdent);

    CbsInstallState curState, appState;
    CHK_HR(pkg->EvaluateApplicability(0, &appState, &curState), "ERROR: Failed to evaluate applicability for %s [HR = %08x]", pkgIdent);
    
    LPCWSTR curStateStr = PkgStateAsStr(curState);
    LPCWSTR intendStateStr = PkgStateAsStr(g_options.intendedPkgState);

    LDBG("Package current state: %s", curStateStr);

    ASRT(curState != g_options.intendedPkgState, "ERROR: Package %s is already %s", pkgIdent, curStateStr);
    ASRT(appState == CbsInstallState::Installed, "ERROR: Package %s is not applicable to the current image", pkgIdent);

    LOG("Setting package %s as %s", pkgIdent, intendStateStr);

    pkg->InitiateChanges(0, CbsInstallState::Installed, NULL);

    CbsRequiredAction reqAct;
    CHK_HR(cbsSess->FinalizeEx(0, &reqAct), "ERROR: Failed to finalize CBS session [HR = %08x]");

    if (reqAct == CbsRequiredAction::Reboot) {
        LOG("CBS has indicated that a reboot is required");
    }

    return 0;
}
