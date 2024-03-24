#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <unknwn.h>

/*
Sources:
Declarations in 10.0.14393.0 WDK isolation.h
https://github.com/seven-mile/UFCase
wcp.dll symbols
*/

#pragma region Exported Functions

/* [v1_enum] */
enum _OFFLINE_STORE_CREATION_PARAMETERS_FLAGS
{
    OFFLINE_STORE_CREATION_PARAMETERS_FLAG_FAKE_ONLINE = 0x4,
    OFFLINE_STORE_CREATION_PARAMETERS_FLAG_APPLY_NO_KTM = 0x10
};
typedef struct _OFFLINE_STORE_CREATION_PARAMETERS
{
    SIZE_T cbSize;
    DWORD dwFlags;
    LPCWSTR pszHostSystemDrivePath;
    LPCWSTR pszHostWindowsDirectoryPath;
    LPCWSTR pszTargetWindowsDirectoryPath;
    LPCWSTR pszHostRegistryMachineSoftwarePath;
    LPCWSTR pszHostRegistryMachineSystemPath;
    LPCWSTR pszHostRegistryMachineSecurityPath;
    LPCWSTR pszHostRegistryMachineSAMPath;
    LPCWSTR pszHostRegistryMachineComponentsPath;
    LPCWSTR pszHostRegistryUserDotDefaultPath;
    LPCWSTR pszHostRegistryDefaultUserPath;
    ULONG ulProcessorArchitecture;
    LPCWSTR pszHostRegistryMachineOfflineSchemaPath;
} 	OFFLINE_STORE_CREATION_PARAMETERS;

typedef struct _OFFLINE_STORE_CREATION_PARAMETERS* POFFLINE_STORE_CREATION_PARAMETERS;

typedef const OFFLINE_STORE_CREATION_PARAMETERS* PCOFFLINE_STORE_CREATION_PARAMETERS;

struct _OFFLINE_STORE_CREATION_PARAMETERS_EX : _OFFLINE_STORE_CREATION_PARAMETERS
{
    LPCWSTR pszExternalServicingDirectory;          // Non-null value operates 
    LPCWSTR pszHostRegistryMachineDriversPath;      // path in the host's registry namespace to HKEY_LOCAL_MACHINE\DRIVERS (a/k/a \Registry\Machine\DRIVERS) for the target OS
    LPCWSTR pszHostRegistryBcdPath;                 // path in the host's registry namespace to BCD (a/k/a \boot\BCD ) for the EFIESP Partition in mobile
    LPCWSTR pszHostRegistryExternalServicingPath;   // optionally set up external storage hive using preloaded key
};
typedef _OFFLINE_STORE_CREATION_PARAMETERS_EX OFFLINE_STORE_CREATION_PARAMETERS_EX;
typedef OFFLINE_STORE_CREATION_PARAMETERS_EX* POFFLINE_STORE_CREATION_PARAMETERS_EX;
typedef const OFFLINE_STORE_CREATION_PARAMETERS_EX* PCOFFLINE_STORE_CREATION_PARAMETERS_EX;

typedef HRESULT(__stdcall* PCREATE_NEW_OFFLINE_STORE_FUNCTION)(
    /* [annotation][in] */
    _In_  DWORD dwFlags,
    /* [annotation][in] */
    _In_  PCOFFLINE_STORE_CREATION_PARAMETERS pParameters,
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][iid_is][out] */
    _Out_  IUnknown** ppStore,
    /* [annotation][out] */
    _Out_  DWORD* pdwDisposition);

enum __MIDL___MIDL_itf_isolation_0000_0043_0001
{
    CREATE_NEW_OFFLINE_STORE_FLAGS_NO_MANIFEST_CACHING = 0x1
};

enum __MIDL___MIDL_itf_isolation_0000_0043_0002
{
    CREATE_NEW_WINDOWS_DISABLE_COMPONENT_BACKUPS = 0x1,
    CREATE_NEW_WINDOWS_DISABLE_SMI = 0x2,
    CREATE_NEW_WINDOWS_DISABLE_MANIFEST_CACHE = 0x4,
    CREATE_NEW_WINDOWS_SINGLE_RESOURCE_OWNERSHIP = 0x8,
    CREATE_NEW_WINDOWS_MOBILE_STORE = 0x20,
    CREATE_NEW_WINDOWS_IS_ONE_CORE = 0x40
};

typedef HRESULT(__stdcall* PCREATE_NEW_OFFLINE_STORE_FUNCTION)(
    /* [annotation][in] */
    _In_  DWORD dwFlags,
    /* [annotation][in] */
    _In_  PCOFFLINE_STORE_CREATION_PARAMETERS pParameters,
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][iid_is][out] */
    _Out_  IUnknown** ppStore,
    /* [annotation][out] */
    _Out_  DWORD* pdwDisposition);


enum __MIDL___MIDL_itf_isolation_0000_0043_0003
{
    OPEN_EXISTING_OFFLINE_STORE_FLAGS_NO_MANIFEST_CACHING = 0x1
};

typedef HRESULT(__stdcall* POPEN_EXISTING_OFFLINE_STORE_FUNCTION)(
    /* [annotation][in] */
    _In_  DWORD dwFlags,
    /* [annotation][in] */
    _In_  PCOFFLINE_STORE_CREATION_PARAMETERS pParameters,
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][iid_is][out] */
    _Out_  IUnknown** ppStore,
    /* [annotation][out] */
    _Out_  DWORD* pdwDisposition);


enum __MIDL___MIDL_itf_isolation_0000_0043_0004
{
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_PICK_A_LOCATION = 0x1,
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_WINPE_LAYOUT = 0x2,
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_PICK_REG_ROOT = 0x4,
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_USE_EXT_STORAGE = 0x8,
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_KEEP_EXISTING = 0x10,
    CREATE_NEW_PSEUDO_WINDOWS_FLAG_MOBILESTORE = 0x20,
    CREATE_NEW_PSEUDO_WINDOWS_VALID_FLAGS = 0x3f
};

enum __MIDL___MIDL_itf_isolation_0000_0043_0005
{
    CREATE_NEW_WINDOWS_VERSION = 1
};
enum class StoreStyle : BYTE
{
    Phone = 0,
    OneCore = (1 << 0),
    Desktop = (1 << 1),
};

typedef HRESULT(__stdcall* PCREATE_NEW_PSEUDO_WINDOWS_FUNCTION)(
    /* [annotation][in] */
    _In_  DWORD dwFlags,
    /* [annotation][full][in] */
    _In_opt_  LPCWSTR pszDirectoryRoot,
    /* [annotation][full][in] */
    _In_opt_  LPCWSTR pszRegistryRoot,
    /* [annotation][out] */
    _Inout_  POFFLINE_STORE_CREATION_PARAMETERS pParameters);

typedef HRESULT(__stdcall* PCREATE_NEW_PSEUDO_WINDOWS_EX_FUNCTION)(
    /* [annotation][in] */
    _In_  DWORD dwFlags,
    /* [annotation][full][in] */
    _In_opt_  LPCWSTR pszDirectoryRoot,
    /* [annotation][full][in] */
    _In_opt_  LPCWSTR pszRegistryRoot,
    /* [annotation][out] */
    _Inout_  POFFLINE_STORE_CREATION_PARAMETERS pParameters,
    /* [annotation][out] */
    _Out_opt_  PVOID* ppvRootkey);

typedef HRESULT(__stdcall* PCREATE_NEW_WINDOWS_FUNCTION)(
    /* [annotation][in] */
    _In_opt_  DWORD dwFlags,
    /* [annotation][full][in] */
    _In_  LPCWSTR pszSystemDrive,
    /* [annotation][out] */
    _Inout_  POFFLINE_STORE_CREATION_PARAMETERS pParameters,
    /* [annotation][out] */
    _Out_opt_  PVOID* ppvKeys,
    /* [annotation][out] */
    _Out_opt_  DWORD* pdwDisposition);

typedef HRESULT(WINAPI* PSET_ISOLATION_MALLOC_FUNCTION)(IMalloc* alloc);

typedef HRESULT(__stdcall* PWCP_INITIALIZE_FUNCTION)(PVOID* ppCookie);

typedef HRESULT(__stdcall* PWCP_SHUTDOWN_FUNCTION)(PVOID pCookie);

typedef HRESULT(__stdcall* PDISMOUNT_REGISTRY_HIVES_FUNCTION)(PVOID pCookie);

MIDL_INTERFACE("ace1b703-1aac-4956-ab87-90cac8b93ce6")
IManifestParseErrorCallback : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnError(
        /* [in] */ ULONG nStartLine,
        /* [in] */ ULONG nStartColumn,
        /* [in] */ ULONG cCharacterCount,
        /* [in] */ HRESULT hr,
        /* [in] */ LPCWSTR pszErrorStatusHostFile,
        /* [in] */ SIZE_T cParameterCount,
        /* [size_is][in] */ LPCWSTR * prgpszParameters) = 0;

};

typedef HRESULT(__stdcall* PPARSE_MANIFEST_FUNCTION)(
    /* [annotation][in] */
    _In_  LPCWSTR pszManifestPath,
    /* [annotation][full][in] */
    _In_opt_  IManifestParseErrorCallback* pIManifestParseErrorCallback,
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][iid_is][out] */
    _Out_  IUnknown** ppManifest);

#pragma endregion

#pragma region CSI Interfaces

typedef struct _IDENTITY_ATTRIBUTE
{
    LPCWSTR pszNamespace;
    LPCWSTR pszName;
    LPCWSTR pszValue;
} 	IDENTITY_ATTRIBUTE;

typedef struct _IDENTITY_ATTRIBUTE* PIDENTITY_ATTRIBUTE;

typedef const IDENTITY_ATTRIBUTE* PCIDENTITY_ATTRIBUTE;

MIDL_INTERFACE("9cdaae75-246e-4b00-a26d-b9aec137a3eb")
IEnumIDENTITY_ATTRIBUTE : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Next(
        /* [annotation][in] */
        _In_  SIZE_T celt,
        /* [annotation][length_is][size_is][out] */
        _Out_writes_to_(celt, *pceltWritten)  IDENTITY_ATTRIBUTE rgAttributes[],
        /* [annotation][out] */
        _Out_  SIZE_T * pceltWritten) = 0;

    virtual HRESULT STDMETHODCALLTYPE CurrentIntoBuffer(
        /* [annotation][in] */
        _In_  SIZE_T cbAvailable,
        /* [annotation][length_is][size_is][out] */
        _Out_writes_to_(cbAvailable, *pcbUsed)  BYTE pbData[],
        /* [annotation][out] */
        _Out_  SIZE_T* pcbUsed) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        /* [annotation][in] */
        _In_  SIZE_T celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        /* [annotation][out] */
        _Out_  IEnumIDENTITY_ATTRIBUTE** ppIEnumIDENTITY_ATTRIBUTE) = 0;

    virtual HRESULT STDMETHODCALLTYPE CurrentSize(
        /* [annotation][out] */
        _Out_  SIZE_T* pcbRequired) = 0;

};

MIDL_INTERFACE("587bf538-4d90-4a3c-9ef1-58a200a8a9e7")
IDefinitionIdentity : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetAttribute(
        /* [annotation][unique][in] */
        _In_opt_  LPCWSTR pszNamespace,
        /* [annotation][in] */
        _In_  LPCWSTR pszName,
        /* [annotation][retval][out] */
        _Outptr_  LPWSTR * ppszValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetAttribute(
        /* [annotation][unique][in] */
        _In_opt_  LPCWSTR pszNamespace,
        /* [annotation][in] */
        _In_  LPCWSTR pszName,
        /* [annotation][unique][in] */
        _In_opt_  LPCWSTR pszValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumAttributes(
        /* [annotation][out] */
        _Out_  IEnumIDENTITY_ATTRIBUTE** ppIEAIA) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        /* [annotation][in] */
        _In_  SIZE_T cDeltas,
        /* [annotation][unique][size_is][in] */
        _In_reads_opt_(cDeltas)  const IDENTITY_ATTRIBUTE prgDeltas[],
        /* [annotation][out] */
        _Out_  IDefinitionIdentity** ppIDefinitionIdentity) = 0;

};

MIDL_INTERFACE("f3549d9c-fc73-4793-9c00-1cd204254c0c")
IEnumDefinitionIdentity : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Next(
        /* [annotation][in] */
        _In_  SIZE_T celt,
        /* [annotation][length_is][size_is][out] */
        _Out_writes_to_(celt, *pceltWritten)  IDefinitionIdentity * rgpIDefinitionIdentity[],
        /* [annotation][out] */
        _Out_  SIZE_T * pceltWritten) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        /* [annotation][in] */
        _In_  SIZE_T celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        /* [annotation][out] */
        _Out_  IEnumDefinitionIdentity** ppIEnumDefinitionIdentity) = 0;

};

typedef union _COMPONENT_VERSION
{
    ULONGLONG Version64;
    struct __MIDL___MIDL_itf_isolation_0000_0000_0001
    {
        ULONG BuildAndRevision;
        ULONG MajorAndMinor;
    } 	Version32;
    struct __MIDL___MIDL_itf_isolation_0000_0000_0002
    {
        USHORT Revision;
        USHORT Build;
        USHORT Minor;
        USHORT Major;
    } 	Version16;
} 	COMPONENT_VERSION;

typedef union _COMPONENT_VERSION* PCOMPONENT_VERSION;

typedef const COMPONENT_VERSION* PCCOMPONENT_VERSION;

struct CSIFILE {
    int unk;
    IDefinitionIdentity* defIds[2];
    LPCWSTR fname;
    void* unk2[5];
};

struct IEnumCSI_FILE : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

    virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt,
        _Out_ struct CSIFILE* rgpCol,
        _Out_ ULONG64* pbFetched) = 0;
    virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
    virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCSI_FILE**) = 0;
};

MIDL_INTERFACE("16b07adc-182f-4fe3-bc9b-e53991770f25") ICSITransaction : IUnknown{
    virtual HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID riid, _Out_ LPVOID * ppvObject) override = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

    virtual HRESULT STDMETHODCALLTYPE
        InstallDeployment(ULONG, IDefinitionIdentity*, wchar_t const*,
            wchar_t const*, wchar_t const*, wchar_t const*,
            wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        PinDeployment(ULONG, IDefinitionIdentity*, wchar_t const*, wchar_t const*,
            wchar_t const*, wchar_t const*, wchar_t const*,
            wchar_t const*, unsigned __int64, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        UninstallDeployment(ULONG, IDefinitionIdentity*, wchar_t const*,
            wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        UnpinDeployment(ULONG, IDefinitionIdentity*, wchar_t const*,
            wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        EnumMissingComponents(ULONG, IEnumDefinitionIdentity**, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddComponent(ULONG, IDefinitionIdentity*,
        wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnumMissingFiles(ULONG,
        IEnumCSI_FILE**) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddFile(ULONG, IDefinitionIdentity*,
        wchar_t const*, wchar_t const*,
        ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Analyze(ULONG, _GUID const&,
        IUnknown**) = 0;
    virtual HRESULT STDMETHODCALLTYPE Commit(ULONG,
        struct ICSITransactionCommitSink*,
        ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Abort(ULONG, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Scavenge(ULONG, IDefinitionIdentity*,
        wchar_t const*, wchar_t const*,
        ULONG*) = 0;
};

MIDL_INTERFACE("0e695bd1-628c-40a1-88cf-925083986d16") ICSITransaction2 : IUnknown{
    virtual HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID riid, _Out_ LPVOID * ppvObject) override = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;
    virtual HRESULT STDMETHODCALLTYPE AddFiles(ULONG, ULONG, IDefinitionIdentity**, wchar_t const** const, wchar_t const** const, ULONG* const, ULONG*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddComponents(ULONG, ULONG, IDefinitionIdentity**, wchar_t const*, ULONG*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Scavenge(ULONG, void*, IDefinitionIdentity*, wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE Analyze(ULONG, _GUID const&, IUnknown**, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnstageDeploymentPayload(ULONG, IDefinitionIdentity*, wchar_t const*, wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE MarkDeploymentStaged(ULONG, IDefinitionIdentity*, wchar_t const*, wchar_t const*, wchar_t const*, ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE MarkDeploymentUnstaged(ULONG, IDefinitionIdentity*, wchar_t const*, wchar_t const*, wchar_t const*, ULONG*) = 0;
};

struct ICSIRepairTransaction : ICSITransaction {
    virtual HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;
};

enum ICSISTORE_GET_COMPONENT_INFORMATION_CLASS {
    ICSISTORE_GET_COMPONENT_INFORMATION_CLASS_BASIC = 1,
    ICSISTORE_GET_COMPONENT_INFORMATION_CLASS_PAYLOAD = 2,
};

/// <summary>
/// STAGED      = 0x1, map to 2
/// LOCKED      = 0x2, map to 2
/// PINNED      = 0x4, map to 3
/// INSTALLED   = 0x8, map to 4(v) 5(x)
/// PRESTAGED   = 0x10 map to 1
/// </summary>
typedef enum _CSI_COMPONENT_STATUS {
    CSI_COMPONENT_STATUS_PRESTAGED = 1,
    CSI_COMPONENT_STATUS_STAGED = 2,
    CSI_COMPONENT_STATUS_PINNED = 3,
    CSI_COMPONENT_STATUS_INSTALLED_MATCH = 4,
    CSI_COMPONENT_STATUS_INSTALLED_MISMATCH = 5,
} CSI_COMPONENT_STATUS;

typedef struct _CSI_COMPONENT_BASIC_INFORMATION {
    DWORD cbSize;
    DWORD dwCsiStatus;
    BOOL bIsComplete;
} CSI_COMPONENT_BASIC_INFORMATION;

typedef struct _CSI_COMPONENT_PAYLOAD_INFORMATION {
    DWORD cbSize;
    LPWSTR pszPayloadRoot;
    IUnknown* pComponentLock;
} CSI_COMPONENT_PAYLOAD_INFORMATION;

struct __declspec(uuid("465F1EC1-7F1D-4A85-A30B-AE1090F212DB")) ICSIStore
    : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
    virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

    virtual HRESULT STDMETHODCALLTYPE BeginTransaction(
        ULONG, _GUID const&, LPCTSTR client_id, _Out_ ICSITransaction**) = 0;
    virtual HRESULT STDMETHODCALLTYPE CancelPendingTransaction(ULONG,
        _GUID const&,
        LPCTSTR,
        _Out_ ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE BeginRepairTransaction(
        ULONG, _Out_ ICSIRepairTransaction**, _Out_ ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        CancelPendingRepairTransaction(ULONG, _Out_ ULONG*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComponentManifests(
        DWORD dwFlags, UINT64 cnt, IDefinitionIdentity* def_idents[],
        _GUID const&, _Out_ IUnknown* manifests[]) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComponentInstalledVersions(
        ULONG dwFlags, UINT64 cnt_idents, _Out_ IDefinitionIdentity* def_idents[],
        _Out_ ULONG* cnt_versions, _Out_ COMPONENT_VERSION versions[]) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComponentInformation(
        ULONG dwFlags, ICSISTORE_GET_COMPONENT_INFORMATION_CLASS dwClass,
        IDefinitionIdentity*, UINT64 cbSize, _Out_ void*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReplaceMacros(ULONG, IDefinitionIdentity*,
        LPCTSTR, _Out_ LPTSTR*) = 0;
    virtual HRESULT STDMETHODCALLTYPE
        EnumPendingTransactions(ULONG, _GUID const&, _Out_ IUnknown**) = 0;
    virtual HRESULT STDMETHODCALLTYPE CancelPendingTransactions(
        ULONG, UINT64, _Out_ LPCTSTR const*, _Out_ ULONG*) = 0;
};

#pragma endregion
