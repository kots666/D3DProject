#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// ID_DYNAMIC : 실시간 업데이트가 필요한 컴포넌트
	enum COMPONENTID	{ ID_DYNAMIC, ID_STATIC, ID_END };
	enum INFO			{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION		{ ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum RESOURCETYPE	{ RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_NORMAL, RESOURCE_END	};

	enum BUFFERID		{ BUFFER_TRICOL, BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_TRAIL, BUFFER_PTTEX, BUFFER_INRCTEX };
	enum TEXTURETYPE	{ TEX_NORMAL, TEX_CUBE, TEX_END };
	
	enum RENDERID		{ RENDER_SKYBOX, RENDER_PRIORITY, RENDER_SHADOWDEPTH, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
	enum OBJCOLTYPE		{ OBJCOL_PLAYER, OBJCOL_ENEMY, OBJCOL_END};

	enum COLLTYPE		{ COL_FALSE, COL_TRUE, COL_END };
	enum MESHTYPE		{ TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI };

	enum ANGLE			{ ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END };
	enum MOUSEKEYSTATE	{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

}

#endif
