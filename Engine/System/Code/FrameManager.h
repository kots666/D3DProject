#ifndef FrameManager_h__
#define FrameManager_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameManager : public CBase
{
	DECLARE_SINGLETON(CFrameManager)
private:
	explicit CFrameManager();
	virtual ~CFrameManager();

public:
	_bool IsPermitCall(const _tchar* frameTag, const _float& deltaTime);

public:
	HRESULT Ready(const _tchar* frameTag, const _float& callLimit);

private:
	CFrame* FindFrame(const _tchar* frameTag);

private:
	map<const _tchar*, CFrame*> m_frameMap;

public:
	virtual void Free();
};

END

#endif