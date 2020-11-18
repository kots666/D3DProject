#ifndef Pipeline_h__
#define Pipeline_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline : public CBase
{
private:
	explicit CPipeline();
	virtual ~CPipeline();

public:
	static void MakeTransformMatrix(_matrix* out, 
									const _vec3* right,
									const _vec3* up,
									const _vec3* look,
									const _vec3* pos);
	
	static void MyRotationX(_vec3* out, _vec3* in, _float angle);
	static void MyRotationY(_vec3* out, _vec3* in, _float angle);
	static void MyRotationZ(_vec3* out, _vec3* in, _float angle);

	static void MakeViewSpaceMatrix(_matrix* out, const _vec3* eye, const _vec3* at, const _vec3* up);
	static void MakePerspectiveMatrix(_matrix* out, const _float& fovY, const _float& aspect, const _float& nearZ, const _float& farZ);

};

END

#endif
