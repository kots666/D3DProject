#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_effect(nullptr),
	m_errMsg(nullptr)
{
	SafeAddRef(m_device);
}

Engine::CShader::CShader(const CShader& rhs) :
	m_device(rhs.m_device),
	m_effect(rhs.m_effect),
	m_errMsg(rhs.m_errMsg)
{
	SafeAddRef(m_device);
	SafeAddRef(m_effect);
	SafeAddRef(m_errMsg);
}

Engine::CShader::~CShader()
{

}

HRESULT Engine::CShader::Ready(const _tchar* filePath)
{
	// m_effect 값이 있고, m_errMsg 값이 없는 경우 : 쉐이더 코드가 정상적으로 작성됨
	// m_effect 값이 없고, m_errMsg 값이 있는 경우 : 쉐이더 코드 상태가 비정상적, 에러인 경우
	// m_effect 값이 있고, m_errMsg 값도 있는 경우 : 동작하는 것은 문제가 없으나 경고가 있는 경우


	if (FAILED(D3DXCreateEffectFromFile(m_device, 
		filePath,
		NULL, // 매크로의 주소 (쉐이더 내부에 지원하는)
		NULL,  // 포함 파일의 주소(쉐이더 내부에 지원하는)
		D3DXSHADER_DEBUG, // 쉐이더를 디버깅하겠다는 의미(외부 라이브러리를 이용할 것을 대비하여 넣어놓은 인자)
		NULL,		// 각종 리소스 정보를 어디에 저장할 것인가(Null인 경우 기본값으로 장치에 저장이 됨)
		&m_effect, 
		&m_errMsg))) // 디버깅은 불가능하지만 에러 또는 경고에 대핸 문자열 정보를 저장해두는 용도
	{
		MessageBoxA(NULL, (char*)m_errMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	// 경고 상황
	else if (nullptr != m_errMsg)
	{
		MessageBoxA(NULL, (char*)m_errMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}

	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath)
{
	CShader* instance = new CShader(device);

	if (FAILED(instance->Ready(filePath)))
		SafeRelease(instance);

	return instance;
}

Engine::CComponent* Engine::CShader::Clone()
{
	return new CShader(*this);
}

void Engine::CShader::Free()
{
	SafeRelease(m_errMsg);
	SafeRelease(m_effect);
	SafeRelease(m_device);
}