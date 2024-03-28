#include "uihandler.h"

ULONG UIHandler::AddRef() {
	return InterlockedIncrement(&dwRef);
}

HRESULT UIHandler::QueryInterface(REFIID riid, LPVOID* ppvObject) {
	*ppvObject = NULL;

	if (riid == __uuidof(ICbsUIHandler) || riid == __uuidof(IUnknown)) {
		*ppvObject = this;
		this->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG UIHandler::Release() {
	ULONG decRef = InterlockedDecrement(&dwRef);

	if (!decRef) {
		delete this;
	}

	return decRef;
}

HRESULT UIHandler::Initiate(IEnumCbsUpdate* updates, int* pUnk) {
	return S_OK;
}

HRESULT UIHandler::Error(HRESULT hr, PCWSTR str, int* pUnk) {
	ConLog(" ERROR: %s [HR = %08x]", str, hr);
	return S_OK;
}

HRESULT UIHandler::ResolveSource(PCWSTR str1, ICbsIdentity* pIdent, PCWSTR str2, PWSTR* strUnk, int* pUnk) {
	if (g_options.debug) {
		PWSTR identStr;
		pIdent->SaveAsStringId(&identStr);
		ConLog("Resolve Source: (pkg = \"%s\") \"%s\" \"%s\"", identStr, str1, str2);
	}

	return S_OK;
}

HRESULT UIHandler::EnteringStage(UINT unk1, CbsOperationStage stage, int unk2, int unk3) {
	ConLog("Entering Stage: %s", CbsOpStageAsStr(stage));
	return S_OK;
}

HRESULT UIHandler::Progress(CbsInstallState state, UINT cur, UINT total, int* pUnk) {
	printf("\rProgress: %.1f%%", 100.0 * cur / total);
	return S_OK;
}

HRESULT UIHandler::ProgressEx(CbsInstallState state, UINT cur, UINT total, UINT unk, int* pUnk) {
	return Progress(state, cur, total, pUnk);
}

HRESULT UIHandler::Terminate() {
	return S_OK;
}