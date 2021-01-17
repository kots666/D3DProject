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
	// m_effect ���� �ְ�, m_errMsg ���� ���� ��� : ���̴� �ڵ尡 ���������� �ۼ���
	// m_effect ���� ����, m_errMsg ���� �ִ� ��� : ���̴� �ڵ� ���°� ��������, ������ ���
	// m_effect ���� �ְ�, m_errMsg ���� �ִ� ��� : �����ϴ� ���� ������ ������ ��� �ִ� ���


	if (FAILED(D3DXCreateEffectFromFile(m_device, 
		filePath,
		NULL, // ��ũ���� �ּ� (���̴� ���ο� �����ϴ�)
		NULL,  // ���� ������ �ּ�(���̴� ���ο� �����ϴ�)
		D3DXSHADER_DEBUG, // ���̴��� ������ϰڴٴ� �ǹ�(�ܺ� ���̺귯���� �̿��� ���� ����Ͽ� �־���� ����)
		NULL,		// ���� ���ҽ� ������ ��� ������ ���ΰ�(Null�� ��� �⺻������ ��ġ�� ������ ��)
		&m_effect, 
		&m_errMsg))) // ������� �Ұ��������� ���� �Ǵ� ��� ���� ���ڿ� ������ �����صδ� �뵵
	{
		MessageBoxA(NULL, (char*)m_errMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	// ��� ��Ȳ
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