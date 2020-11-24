#include "Component.h"

USING(Engine)

CComponent::CComponent() :
	m_isClone(false)
{
}

CComponent::CComponent(const CComponent & rhs) :
	m_isClone(true)
{
}

CComponent::~CComponent()
{
}

_int CComponent::Update(const _float & deltaTime)
{
	return 0;
}