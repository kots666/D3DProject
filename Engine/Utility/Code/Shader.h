#ifndef Shader_h__
#define Shader_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader :public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	LPD3DXEFFECT GetEffectHandle() { return m_effect; }

public:
	HRESULT Ready(const _tchar* filePath);

private:
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXEFFECT m_effect; // 쉐이더를 담당하는 컴객체
	LPD3DXBUFFER m_errMsg; // 컴파일 시점에 에러메세지를 저장하기 위한 버퍼 공간

public:
	static CShader* Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath);
	virtual CComponent* Clone();
	virtual void Free();

};


END
#endif // Shader_h__
