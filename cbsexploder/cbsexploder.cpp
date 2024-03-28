#include "uihandler.h"
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

int ProcessOptions(int argc, LPCWSTR* argv, BOOL isCmdline) {
    for (int i = 0; i < argc; i++) {
        LPCWSTR arg = argv[i];
        
        if (!wcsncmp(arg, L"/sp", 3)) {
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
        else if (!wcsncmp(arg, L"/m", 2)) {
            LPCWSTR val = wcschr(arg, L':');
            ASRT_NL(val, "ERROR: Invalid argument %s", arg);
            g_options.pkgPath = val + 1;
        }

        if (isCmdline) {
            if (!wcsncmp(arg, L"/ep", 3)) {
                g_options.enumPkgs = TRUE;
            }
            else if (!wcsncmp(arg, L"/dbg", 4)) {
                g_options.debug = TRUE;
            }
            else if (!wcsncmp(arg, L"/o", 2)) {
                LPCWSTR val = wcschr(arg, L':');
                ASRT_NL(val, "ERROR: Invalid argument %s", arg);
                g_options.winDir = val + 1;
            }
            else if (!wcsncmp(arg, L"/log", 4)) {
                LPCWSTR val = wcschr(arg, L':');
                ASRT_NL(val, "ERROR: Invalid argument %s", arg);
                g_options.logPath = val + 1;
            }
            else if (!wcsncmp(arg, L"/b", 2)) {
                LPCWSTR val = wcschr(arg, L':');
                ASRT_NL(val, "ERROR: Invalid argument %s", arg);
                g_options.batchPath = val + 1;
            }
            else if (!wcsncmp(arg, L"/?", 2)) {
                g_options.help = TRUE;
            }
        }
    }

    ASRT_NL(g_options.winDir, "ERROR: No offline Windows directory path specified");

    if (!g_options.batchPath) {
        if (!g_options.enumPkgs) {
            ASRT_NL(g_options.intendedPkgState != CbsInstallState::Unknown, "ERROR: No package operation specified");
            ASRT_NL(g_options.pkgPath, "ERROR: No package path specified");
        }
    }
    else {
        ASRT_NL(!g_options.enumPkgs, "ERROR: Batch file cannot be used with /ep")

        if (isCmdline) {
            ASRT_NL(g_options.intendedPkgState == CbsInstallState::Unknown, "ERROR: Batch file cannot be used with /sp, /ip, or /up");
            ASRT_NL(!g_options.pkgPath, "ERROR: Batch file cannot be used with /m option");
        }
    }

    return 0;
}

void PrintUsage(LPCWSTR exename) {
    wprintf(
        L"\n"
        "Usage: %s /<mode> /m:<path> /o:<path> [/b:<path>] [/dbg] [/?]\n"
        "\nModes:\n"
        "/sp\tStage package\n"
        "/ip\tInstall package\n"
        "/up\tUninstall package\n"
        "/ep\tEnumerate installed packages (overrides other arguments)\n"
        "\nSession arguments:\n"
        "/m\tManifest path for package\n"
        "/o\tOffline image Windows directory path\n"
        "\nOptional:\n"
        "/dbg\tEnable debug mode (more verbose output and logging)\n"
        "/?\tPrint this usage guide\n"
        "/b\tUse batch file\n"
        "\nBatch file:\n"
        "/b argument cannot be used with /sp, /ip, /up, /ep, or /m.\n"
        "Each line in batch file represents one package operation, in the same format as the above arguments.\n\n"
        "Example batch file:\n"
        "/ip /m:C:\\pkg1.mum\n"
        "/sp /m:C:\\pkg2.mum\n",
        exename
    );
}

int wmain(int argc, LPCWSTR* argv)
{
    printf("cbsexploder 0.0.2 by witherornot\n");

    if (ProcessOptions(argc, argv, TRUE)) {
        PrintUsage(argv[0]);
        return 1;
    }

    if (g_options.help) {
        PrintUsage(argv[0]);
        return 0;
    }
    
    WCHAR fullLogPath[MAX_PATH];
    ASRT_NL(GetFullPathNameW(g_options.logPath, MAX_PATH, fullLogPath, NULL), "ERROR: Log path %s is invalid", g_options.logPath);
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
    
    if (g_options.enumPkgs) {
        ComPtr<IEnumCbsIdentity> pkgEnum;
        CHK_HR(cbsSess->EnumeratePackages(0x50, &pkgEnum), "ERROR: Failed to enumerate packages");

        UINT pbFetched = 1;
        ComPtr<ICbsIdentity> pkgIdent;

        while (pbFetched) {
            HRESULT result = pkgEnum->Next(1, &pkgIdent, &pbFetched);

            if (SUCCEEDED(result) && pbFetched) {
                PWSTR identName;
                pkgIdent->GetStringId(&identName);
                ConLog("%s", identName);
            }
        }

        return 0;
    }

    FILE* batchFile = NULL;

    if (g_options.batchPath) {
        _wfopen_s(&batchFile, g_options.batchPath, L"r");
        ASRT(batchFile, "ERROR: Failed to open batch file %s", g_options.batchPath);
    }

    while (1) {
        if (batchFile) {
            WCHAR line[2048] = { 0 };
            fgetws(line, 2048, batchFile);

            if (!wcslen(line)) {
                break;
            }

            LPWSTR* bargv = (LPWSTR*)malloc(8 * sizeof(LPWSTR));
            int bargc = 0;

            WCHAR* tokCtx = NULL;
            WCHAR* token = NULL;
            LPCWSTR delims = L" \t\n";

            token = wcstok_s(line, delims, &tokCtx);
            while (token) {
                bargv[bargc] = (WCHAR*)malloc((wcslen(token) + 1) * sizeof(WCHAR*));
                ASRT(bargv[bargc], "ERROR: Failed to allocate batch file argument");
                memset(bargv[bargc], 0, (wcslen(token) + 1) * sizeof(WCHAR*));
                memcpy(bargv[bargc], token, sizeof(WCHAR) * wcslen(token));
                bargc += 1;

                token = wcstok_s(NULL, delims, &tokCtx);
            }

            if (ProcessOptions(bargc, (LPCWSTR*)bargv, FALSE)) {
                return 1;
            }

            for (int i = 0; i < bargc; i++) free(bargv[i]);
            free(bargv);
        }

        WCHAR fullPkgPath[MAX_PATH];
        ASRT_NL(GetFullPathNameW(g_options.pkgPath, MAX_PATH, fullPkgPath, NULL), "Package path %s is invalid", g_options.pkgPath);

        ComPtr<ICbsPackage> pkg;
        CHK_HR(cbsSess->CreatePackage(0, CbsPackageType::ExpandedWithMum, fullPkgPath, NULL, &pkg), "ERROR: Failed to create package (pkgPath = \"%s\") [HR = %08x]", fullPkgPath)

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

        const ComPtr<UIHandler> pHandler = new UIHandler();
        pkg->InitiateChanges(0, g_options.intendedPkgState, pHandler.Get());

        if (!batchFile) {
            break;
        }
        else {
            g_options.intendedPkgState = CbsInstallState::Unknown;
        }
    }

    CbsRequiredAction reqAct;
    CHK_HR(cbsSess->FinalizeEx(0, &reqAct), "ERROR: Failed to finalize CBS session [HR = %08x]");

    if (reqAct == CbsRequiredAction::Reboot) {
        LOG("CBS has indicated that a reboot is required");
    }

    return 0;
}
