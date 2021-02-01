#include "stdafx.h"
#include "Loading.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_isFinish(false)
{
	m_device->AddRef();
}

CLoading::~CLoading()
{
}

_uint	CALLBACK CLoading::ThreadMain(void* arg)
{
	CLoading* loading = (CLoading*)arg;

	_uint flag = 0;

	EnterCriticalSection(loading->GetCRT());

	switch (loading->GetLoadingID())
	{
	case LOADING_STAGE:
		flag = loading->LoadingForStage();		
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(loading->GetCRT());
	_endthreadex(0);

	return flag;
}

HRESULT CLoading::Ready(LOADINGID loadID)
{
	InitializeCriticalSection(&m_crt);

	m_thread = (HANDLE)_beginthreadex(NULL, 0, ThreadMain, this, 0, NULL);

	m_loadingID = loadID;

	return S_OK;
}

_uint CLoading::LoadingForStage()
{
	m_nowLoad = 0;

	Engine::CComponent* comp = nullptr;

	comp = Engine::CCalculator::Create(m_device);
	NULL_CHECK_RETURN(comp, E_FAIL);
	Engine::ReadyProto(L"Proto_Calculator", comp);

	/*comp = Engine::COptimization::Create(m_device);
	NULL_CHECK_RETURN(comp, E_FAIL);
	Engine::ReadyProto(L"Proto_Optimization", comp);*/

	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device, Engine::RESOURCE_STAGE, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX, VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device,
		Engine::RESOURCE_STATIC,
		L"Buffer_CrossTex",
		Engine::BUFFER_CROSSTEX), E_FAIL);

	// buffer
	/*FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device,
												Engine::RESOURCE_STATIC,
												L"Buffer_TerrainTex",
												Engine::BUFFER_TERRAINTEX,
												VTXCNTX,
												VTXCNTZ,
												VTXITV),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device,
												Engine::RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);*/
	_int bufferCnt = 2;
	m_nowLoad += bufferCnt;
	
	// ===========================================ÅØ½ºÃÄ============================

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Terrain/Grass_%d.tga", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_HPBar",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/UI/HPBar.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_HPFrame",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/UI/HPFrame.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_HPBar2",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/UI/HPBar2.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_PlayerIcon",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/UI/PlayerIcon.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Trail",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/Trail.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Dissolve",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/Dissolve.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_CircleMask",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/CircleMask.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Distortion",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/Distortion.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Number",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Combo/%d.tga", 10), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_HitEffect",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/HitEffect.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_HitSlash",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Effect/HitSlash3.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Ilust",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Quest/Ilust%d.tga", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Talk",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Quest/%d.png", 5), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_Interaction",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Quest/Interaction.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Texture_TerrainFlat",
		Engine::TEX_NORMAL,
		L"../Resource/Texture/Terrain/LV_B2_Prototype_Floor02_D.tga"), E_FAIL);

	_int textureCnt = 14;
	m_nowLoad += textureCnt;

	LoadingForDynamicMeshNormalTextures();
	LoadingForStaticMeshNormalTextures();

	// ================================= DynamicMesh Load

	// Gladiator
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Gladiator/",
		L"Gladiator.X"), E_FAIL);

	// Dog
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Dog",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Dog/",
		L"Dog.X"), E_FAIL);

	// Goblin_Sword
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Goblin_Sword",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Goblin/Sword/",
		L"Goblin_Sword.X"), E_FAIL);

	// Goblin_Magician
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Goblin_Magician",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Goblin/Magician/",
		L"Goblin_Magician.X"), E_FAIL);

	// Minotauros
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Minotauros",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Minotauros/",
		L"Minotauros.X"), E_FAIL);

	// Boss
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Boss",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/Boss/",
		L"Boss.X"), E_FAIL);

	// NPC
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_NPC",
		Engine::TYPE_DYNAMIC,
		L"../Resource/Mesh/DynamicMesh/NPC/",
		L"Miracle.X"), E_FAIL);

	_int dynamicMeshLoad = 7;

	m_nowLoad += dynamicMeshLoad;

	// ================================================= Static Mesh ===================

	if(FAILED(LoadingForStaticMeshes()))
		return -1;

	FAILED_CHECK_RETURN(Engine::ReadyMesh(
		m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::TYPE_NAVI,
		NULL,
		NULL),
		E_FAIL);

	m_nowLoad += 1;

	m_isFinish = true;

	return 0;
}

HRESULT CLoading::LoadingForDynamicMeshNormalTextures()
{
	// Player
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Player_Hair",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Gladiator/PC_Gl_Sargon_Hair_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Player_Upper",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Gladiator/PC_Gl_Sargon_Body_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Player_Weapon",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Gladiator/PC_Gl_Sargon_WP01_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Player_Face",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Gladiator/CH_PC_Gladiator_Face_N.tga"), E_FAIL);

	// GoblinSword
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Goblin_Sword_Normal",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Goblin/Sword/CH_Mob_Goblin_N.tga"), E_FAIL);

	// GoblinMagician
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Goblin_Magician_Normal",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Goblin/Magician/CH_Mob_Goblin_Magic_N.tga"), E_FAIL);

	// Dog
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Dog_Normal",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Dog/CH_Mob_Dog_N.tga"), E_FAIL);

	// Minotauros
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Minotauros_Normal",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Minotauros/Mob_Minotaur_New_N.tga"), E_FAIL);

	// Boss
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Boss_Weapon",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Boss/Boss_Apocalypse_Weapon_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Boss_Body",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Boss/Boss_Apocalypse_Body_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_Boss_Hair",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/Boss/Boss_Apocalypse_Hair_N.tga"), E_FAIL);

	// NPC
	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_NPC_Body",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/NPC/CH_NPC_Miracle_Body_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_NPC_Hair",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/NPC/CH_NPC_Miracle_Hair_N.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(
		m_device,
		Engine::RESOURCE_NORMAL,
		L"Texture_NPC_Cloth",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/DynamicMesh/NPC/CH_NPC_Miracle_Cloth_N.tga"), E_FAIL);

	_int dynamicNormalTex = 14;

	m_nowLoad += dynamicNormalTex;

	return S_OK;
}

HRESULT CLoading::LoadingForStaticMeshNormalTextures()
{
	/*
	CartCrash - Y
	CatFish - Y
	Gate - Y, B
	House - Y, B
	Props - Y
	Props2 - Y
	Props3 - Y, Y
	Tree(KHJ) - Y
	Tree(n) - Y
	Tree(CWH) - B
	VillageFloor - B, Y
	Well - Y, B, Y
	*/
	_bool singleYellow[1] = { false };
	_bool yellowBlue[2] = { false, true };
	_bool doubleYellow[2] = { false, false };
	_bool blueYellow[2] = { true, false };
	_bool yellowBlueYellow[3] = { false, true, true };

	// Anvil
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Anvil",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Blocks01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Blocks01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// Blocks02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Blocks02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// Brazier
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Brazier",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Broom
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Broom",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// CartCrash01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_CartCrash01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/CartCrash/WoodenGate_N.tga", 1, singleYellow), E_FAIL);

	// CatFish01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_CatFish01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/CatFish/catfish_N.tga", 1, singleYellow), E_FAIL);

	// CircleBlock01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_CircleBlock01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// CircleBlock02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_CircleBlock02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// DistantHouse01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_DistantHouse01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// DistantHouse02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_DistantHouse02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// EachBlock
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_EachBlock",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// Fence01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Fence01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Fence02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Fence02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Fence03
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Fence03",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// FishCart1
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_FishCart1",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// FloorCircle01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_FloorCircle01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/FloorCircle/LV_B2_Prototype_Wall01_N.tga"), E_FAIL);

	// FloorFlat01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_FloorFlat01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/FloorFlat/LV_B2_Prototype_Floor02_N.tga"), E_FAIL);

	// FloorFlat02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_FloorFlat02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/FloorFlat/LV_B2_Prototype_Floor02_N.tga"), E_FAIL);

	// Fruit
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Fruit",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Gate01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Gate01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Gate/Gate_N_0.tga", 1, singleYellow), E_FAIL);

	// Gate02 roof¸¸
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Gate02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Gate/Gate_N_%d.tga", 2, yellowBlue), E_FAIL);

	// Gibbet
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Gibbet",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// House01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House02b
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Houe02b",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House03
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House03",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House03b
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House03b",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House03c
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House03c",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House04
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House04",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House04b
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House04b",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House05
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House05",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House06
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House06",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House07
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House07",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House08
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House08",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// House09
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_House09",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_%d.tga", 2, yellowBlue), E_FAIL);

	// HouseWall house¸¸
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_HouseWall",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/House/House_N_0.tga", 1, singleYellow), E_FAIL);

	// Manger
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Manger",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props3/Props_N_%d.tga", 2, doubleYellow), E_FAIL);

	// NoticeBoard
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_NoticeBoard",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// OwlStatue
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_OwlStatue",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Pack01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Pack01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Pack02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Pack02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Pumpkin
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Pumpkin",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Ramp01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Ramp01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Ramp02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Ramp02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// ShopStand01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_ShopStand01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props3/Props_N_%d.tga", 2, doubleYellow), E_FAIL);

	// ShopStand02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_ShopStand02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props3/Props_N_%d.tga", 2, doubleYellow), E_FAIL);

	// Sign01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Sign01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Sign02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Sign02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props2/FieldVillage_Props02_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Table01
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Table01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Tree01 KHJ
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Tree01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Tree/FieldVillage_Tree01_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Tree02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Tree02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Tree/FieldVillage_tree01_N.tga", 1, singleYellow), E_FAIL);

	// Tree03
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Tree03",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Tree/FieldVillage_tree01_N.tga", 1, singleYellow), E_FAIL);

	// Tree04 KHJ
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Tree04",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Tree/FieldVillage_Tree01_N_KHJ.tga", 1, singleYellow), E_FAIL);

	// Trees CWH
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Trees",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Tree/BG_FallTree_N_CWH.tga"), E_FAIL);

	// VillageCircle01 ³«¿±±æ + µ¹
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageCircle01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_%d.tga", 2, blueYellow), E_FAIL);

	// VillageCircle02 ³«¿±±æ¸¸
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageCircle02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageCircle03 ³«¿±±æ + µ¹
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageCircle03",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_%d.tga", 2, blueYellow), E_FAIL);

	// VillageCircle04 µ¹´óÀÌ
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageCircle04",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Block/GargoyleCastle_CircleBlock_N_KSV.tga"), E_FAIL);

	// VillageFloor01 ³«¿±±æ
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor01",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor02
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor02",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor03
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor03",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor04
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor04",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor05
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor05",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor06
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor06",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor07
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor07",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// VillageFloor08
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_VillageFloor08",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/VillageFloor/VillageFloor_N_0.tga"), E_FAIL);

	// Well - props - roof - house
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Well",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Well/Well_N_%d.tga", 3, yellowBlueYellow), E_FAIL);

	// Wheel
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
		Engine::RESOURCE_NORMAL,
		L"Mesh_Wheel",
		Engine::TEX_NORMAL,
		L"../Resource/Mesh/StaticMesh/Props/FieldVillage_Props_N_KHJ.tga", 1, singleYellow), E_FAIL);

	_int staticNormalTex = 69;

	m_nowLoad += staticNormalTex;

	return S_OK;
}

HRESULT CLoading::LoadingForStaticMeshes()
{
	// Anvil
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Anvil",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Anvil.X"), E_FAIL);

	// Blocks01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Blocks01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"Blocks01.X"), E_FAIL);

	// Blocks02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Blocks02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"Blocks02.X"), E_FAIL);

	// Brazier
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Brazier",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Brazier.X"), E_FAIL);

	// Broom
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Broom",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Broom.X"), E_FAIL);

	// CartCrash01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_CartCrash01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/CartCrash/",
		L"CartCrash01.X"), E_FAIL);

	// CatFish01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_CatFish01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/CatFish/",
		L"CatFish01.X"), E_FAIL);

	// CircleBlock01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_CircleBlock01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"CircleBlock01.X"), E_FAIL);

	// CircleBlock02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_CircleBlock02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"CircleBlock02.X"), E_FAIL);

	// CircleCap ³ë¸» X
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_CircleCap",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/CircleCap/",
		L"CircleCap.X"), E_FAIL);

	// DistantHouse01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_DistantHouse01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"DistantHouse01.X"), E_FAIL);

	// DistantHouse02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_DistantHouse02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"DistantHouse02.X"), E_FAIL);

	// EachBlock
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_EachBlock",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"EachBlock.X"), E_FAIL);

	// Fence01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Fence01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Fence01.X"), E_FAIL);

	// Fence02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Fence02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Fence02.X"), E_FAIL);

	// Fence03
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Fence03",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Fence03.X"), E_FAIL);

	// FishCart1
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_FishCart1",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"FishCart1.X"), E_FAIL);

	// FloorCircle01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_FloorCircle01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/FloorCircle/",
		L"FloorCircle01.X"), E_FAIL);

	// FloorFlat01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_FloorFlat01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/FloorFlat/",
		L"FloorFlat01.X"), E_FAIL);

	// FloorFlat02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_FloorFlat02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/FloorFlat/",
		L"FloorFlat02.X"), E_FAIL);

	// Fruit
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Fruit",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Fruit.X"), E_FAIL);

	// Gate01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Gate01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Gate/",
		L"Gate01.X"), E_FAIL);

	// Gate02 roof¸¸
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Gate02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Gate/",
		L"Gate02.X"), E_FAIL);

	// Gibbet
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Gibbet",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Gibbet.X"), E_FAIL);

	// House01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House01.X"), E_FAIL);

	// House02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House02.X"), E_FAIL);

	// House02b
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Houe02b",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House02b.X"), E_FAIL);

	// House03
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House03",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House03.X"), E_FAIL);

	// House03b
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House03b",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House03b.X"), E_FAIL);

	// House03c
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House03c",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House03c.X"), E_FAIL);

	// House04
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House04",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House04.X"), E_FAIL);

	// House04b
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House04b",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House04b.X"), E_FAIL);

	// House05
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House05",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House05.X"), E_FAIL);

	// House06
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House06",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House06.X"), E_FAIL);

	// House07
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House07",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House07.X"), E_FAIL);

	// House08
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House08",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House08.X"), E_FAIL);

	// House09
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_House09",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"House09.X"), E_FAIL);

	// HouseWall house¸¸
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_HouseWall",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/House/",
		L"HouseWall.X"), E_FAIL);

	// Manger
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Manger",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props3/",
		L"Manger.X"), E_FAIL);

	// NoticeBoard
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_NoticeBoard",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"NoticeBoard.X"), E_FAIL);

	// OwlStatue
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_OwlStatue",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"OwlStatue.X"), E_FAIL);

	// Pack01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Pack01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"Pack01.X"), E_FAIL);

	// Pack02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Pack02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"Pack02.X"), E_FAIL);

	// Projectile_Head
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Projectile_Head",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Projectile/",
		L"Head_Projectile.X"), E_FAIL);

	// Projectile_Tail
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Projectile_Tail",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Projectile/",
		L"Projectile1.X"), E_FAIL);

	// Pumpkin
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Pumpkin",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"Pumpkin.X"), E_FAIL);

	// Ramp01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Ramp01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Ramp01.X"), E_FAIL);

	// Ramp02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Ramp02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Ramp02.X"), E_FAIL);

	// ShopStand01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_ShopStand01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props3/",
		L"ShopStand01.X"), E_FAIL);

	// ShopStand02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_ShopStand02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props3/",
		L"ShopStand02.X"), E_FAIL);

	// Sign01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Sign01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"Sign01.X"), E_FAIL);

	// Sign02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Sign02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props2/",
		L"Sign02.X"), E_FAIL);

	// SkyBox
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_SkyBox",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/SkyBox/",
		L"SkyBox.X"), E_FAIL);

	// SkyBox2
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_SkyBox2",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/SkyBox/",
		L"SkyBox2.X"), E_FAIL);

	// Table01
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Table01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Table01.X"), E_FAIL);

	// Tree01 KHJ
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Tree01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Tree/",
		L"Tree01.X"), E_FAIL);

	// Tree02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Tree02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Tree/",
		L"Tree02.X"), E_FAIL);

	// Tree03
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Tree03",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Tree/",
		L"Tree03.X"), E_FAIL);

	// Tree04 KHJ
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Tree04",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Tree/",
		L"Tree04.X"), E_FAIL);

	// Trees CWH
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Trees",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Tree/",
		L"Trees.X"), E_FAIL);

	// VillageCircle01 ³«¿±±æ + µ¹
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageCircle01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageCircle01.X"), E_FAIL);

	// VillageCircle02 ³«¿±±æ¸¸
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageCircle02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageCircle02.X"), E_FAIL);

	// VillageCircle03 ³«¿±±æ + µ¹
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageCircle03",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageCircle03.X"), E_FAIL);

	// VillageCircle04 µ¹´óÀÌ
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageCircle04",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Block/",
		L"VillageCircle04.X"), E_FAIL);

	// VillageFloor01 ³«¿±±æ
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor01",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor01.X"), E_FAIL);

	// VillageFloor02
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor02",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor02.X"), E_FAIL);

	// VillageFloor03
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor03",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor03.X"), E_FAIL);

	// VillageFloor04
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor04",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor04.X"), E_FAIL);

	// VillageFloor05
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor05",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor05.X"), E_FAIL);

	// VillageFloor06
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor06",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor06.X"), E_FAIL);

	// VillageFloor07
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor07",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor07.X"), E_FAIL);

	// VillageFloor08
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_VillageFloor08",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/VillageFloor/",
		L"VillageFloor08.X"), E_FAIL);

	// Well - props - roof - house
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Well",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Well/",
		L"Well.X"), E_FAIL);

	// Wheel
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
		Engine::RESOURCE_STAGE,
		L"Mesh_Wheel",
		Engine::TYPE_STATIC,
		L"../Resource/Mesh/StaticMesh/Props/",
		L"Wheel.X"), E_FAIL);

	_int staticMeshLoad = 74;

	m_nowLoad += staticMeshLoad;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 device, LOADINGID loadID)
{
	CLoading* instance = new CLoading(device);

	if (FAILED(instance->Ready(loadID)))
		SafeRelease(instance);

	return instance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_thread, INFINITE);
	CloseHandle(m_thread);

	DeleteCriticalSection(&m_crt);

	Engine::SafeRelease(m_device);
}

