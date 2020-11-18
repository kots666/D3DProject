#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
	Free();
}

HRESULT CProtoMgr::Ready(const _tchar * protoTag, CComponent * comp)
{
	CComponent* tmpComp = Find(protoTag);

	if (nullptr != tmpComp) return E_FAIL;

	m_compMap.emplace(protoTag, comp);

	return S_OK;
}

CComponent * CProtoMgr::Clone(const _tchar * protoTag)
{
	CComponent* comp = Find(protoTag);

	if (nullptr == comp) return nullptr;

	return comp->Clone();
}

CComponent * CProtoMgr::Find(const _tchar * protoTag)
{
	auto iter = find_if(m_compMap.begin(), m_compMap.end(), CTagFinder(protoTag));

	if (m_compMap.end() == iter) return nullptr;

	return iter->second;
}

void CProtoMgr::Free()
{
	for_each(m_compMap.begin(), m_compMap.end(), CDeleteMap());
	m_compMap.clear();
}
