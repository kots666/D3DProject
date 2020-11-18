#ifndef ProtoMgr_h__
#define ProtoMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();

public:
	HRESULT Ready(const _tchar* protoTag, CComponent* comp);
	CComponent* Clone(const _tchar* protoTag);

private:
	CComponent* Find(const _tchar* protoTag);

private:
	map<const _tchar*, CComponent*> m_compMap;

private:
	virtual void Free();
};

END

#endif