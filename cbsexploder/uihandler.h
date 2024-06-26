#include "CbsApi.h"
#include "logging.h"

class UIHandler : public ICbsUIHandler {
private:
	ULONG dwRef = 0;

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override;
	virtual ULONG STDMETHODCALLTYPE Release(void) override;

	virtual HRESULT STDMETHODCALLTYPE Initiate(IEnumCbsUpdate*, int*) override;
	virtual HRESULT STDMETHODCALLTYPE Terminate(void) override;
	virtual HRESULT STDMETHODCALLTYPE Error(HRESULT, PCWSTR, int*) override;
	virtual HRESULT STDMETHODCALLTYPE ResolveSource(PCWSTR, ICbsIdentity*, PCWSTR, PWSTR*, int*) override;
	virtual HRESULT STDMETHODCALLTYPE Progress(CbsInstallState, UINT, UINT, int*) override;
	virtual HRESULT STDMETHODCALLTYPE EnteringStage(UINT, CbsOperationStage, int, int) override;
	virtual HRESULT STDMETHODCALLTYPE ProgressEx(CbsInstallState, UINT, UINT, UINT, int*) override;

	UIHandler() = default;
	virtual ~UIHandler() = default;
};