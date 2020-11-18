#include "Management.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

CManagement::~CManagement()
{
	Free();
}

CScene * CManagement::GetScene(const _tchar * sceneName)
{
	auto iter = m_sceneMap.find(sceneName);

	if (m_sceneMap.end() == iter) return nullptr;

	return iter->second;
}

void CManagement::Free()
{
	for_each(m_sceneMap.begin(), m_sceneMap.end(), CDeleteMap());
	m_sceneMap.clear();
}
