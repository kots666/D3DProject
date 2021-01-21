#include "stdafx.h"
#include "GoblinMagician.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "HPUI.h"
#include "Texture.h"

CGoblinMagician::CGoblinMagician(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle) :
	Engine::CGameObject(device),
	m_startPosition(pos),
	m_angle(angle)
{

}

CGoblinMagician::~CGoblinMagician()
{
}

HRESULT CGoblinMagician::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	D3DXMatrixRotationY(&m_reviseMat, D3DXToRadian(-90.f));

	m_transCom->SetPos(m_startPosition);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(m_angle));

	m_meshCom->SetBoneName("Bip001");

	CColliderManager::GetInstance()->AddObject(OBJ_ENEMY, this);

	m_isInterval = false;
	m_isHit = false;
	m_isAttack = false;
	m_isDeadAnim = false;
	m_isSpawn = false;
	m_isSpawned = false;

	m_intervalTime = 0.f;

	return S_OK;
}

_int CGoblinMagician::Update(const _float& deltaTime)
{
	CalcState(deltaTime);
	MonsterAI(deltaTime);

	m_meshCom->PlayAnimation(deltaTime);
	m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);

	_vec3 movePos;

	if (m_meshCom->CanCalcMovePos("RootBone", movePos))
	{
		//D3DXVec3TransformNormal(&movePos, &movePos, &m_yScaleRotMat);
		movePos.y = 0;
		movePos.z = 0;

		_vec3 nowPos;
		_vec3 nowDir;
		m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
		m_transCom->GetInfo(Engine::INFO_LOOK, &nowDir);

		nowDir *= movePos.x;

		//_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(movePos));
		_vec3 moveDist = nowPos + nowDir;

		m_transCom->SetPos(moveDist);

		//cout << movePos.x << endl;
	}

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	for (auto& elem : m_hitCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	for (auto& elem : m_attackCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	return 0;
}

void CGoblinMagician::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect, 1);

	effect->End();

	Engine::SafeRelease(effect);
}

_bool CGoblinMagician::AttackColliderOverlapped(Engine::CGameObject * target)
{
	if (IsInCollideList(target)) return false;

	m_collideList.emplace_back(target);

	return true;
}

void CGoblinMagician::HitColliderOverlapped(Engine::CGameObject * causer)
{
	m_hp -= 20;

	if (m_hp > 0)
	{
		for (auto& elem : m_hitCollider)
			elem->SetIsCollide(true);
		DoHit();
	}
	else
	{
		DoDeadAnim();
	}

	Engine::CTransform* causerTrans = dynamic_cast<Engine::CTransform*>(causer->GetComponent(L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == causerTrans) return;

	_vec3 pos;
	causerTrans->GetInfo(Engine::INFO_POS, &pos);

	LookAtTarget(pos);
}

HRESULT CGoblinMagician::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Goblin_Magician"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);

	// NaviMesh
	component = m_naviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NaviMesh", component);

	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	// Normal Texture
	component = m_normalTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_Goblin_Magician_Normal"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_meshCom->AddNormalTexture(m_normalTexCom);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NormalTexture", component);

	return S_OK;
}

HRESULT CGoblinMagician::LoadCollider()
{
	m_hitCollider.reserve(1);

	HANDLE hFile = CreateFile(L"../Data/Goblin_Magician_Collider.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	_int cnt = 0;

	Engine::CComponent* component = nullptr;

	while (true)
	{
		_tchar boneName[50] = L"";

		_vec3 offset;
		_float radius;

		ReadFile(hFile, boneName, sizeof(boneName), &dwByte, nullptr);
		ReadFile(hFile, &offset, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &radius, sizeof(_float), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		char tmpName[50] = "";
		WideCharToMultiByte(CP_ACP, 0, boneName, 50, tmpName, 50, NULL, NULL);

		const Engine::D3DXFRAME_EX* foundBone = m_meshCom->GetCloneFrameByName(tmpName);
		Engine::D3DXFRAME_EX* findBone = const_cast<Engine::D3DXFRAME_EX*>(foundBone);

		_tchar* comName = L"Com_Collider";

		_tchar* myName = new _tchar[50];
		_tchar numCnt[10] = L"";
		ZeroMemory(myName, sizeof(_tchar) * 50);
		lstrcpy(myName, comName);
		wsprintf(numCnt, L"%d", cnt);
		lstrcat(myName, numCnt);

		Engine::CSphereCollider* newSphere = Engine::CSphereCollider::Create(m_device, myName, &findBone->combinedTransformationMatrix, offset, radius);
		component = newSphere;
		NULL_CHECK_RETURN(component, E_FAIL);

		m_hitCollider.emplace_back(newSphere);

		++cnt;
	}
	CloseHandle(hFile);

	for (auto& elem : m_hitCollider)
		elem->SetCanCollide(true);

	for (auto& elem : m_hitCollider)
		elem->UpdateByBone(m_transCom->GetWorldMatrix());

	return S_OK;
}

HRESULT CGoblinMagician::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	m_transCom->GetWorldMatrix(&matWorld);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

void CGoblinMagician::CalcState(const _float & deltaTime)
{
	if (m_isHit)
	{
		if (m_meshCom->IsAnimationSetEnd())
		{
			m_isHit = false;
			m_isInterval = true;
			m_intervalTime = 0.5f;
			DoIdle();
		}
	}

	if (m_isAttack)
	{
		if (m_meshCom->IsAnimationSetEnd())
		{
			m_isAttack = false;
			m_isInterval = true;
			m_intervalTime = 2.f;
			DoIdle();
		}
	}

	if (m_isSpawn)
	{
		if (m_meshCom->IsAnimationSetEnd())
		{
			SpawnGoblinSword();

			m_isSpawn = false;
			m_isInterval = true;
			m_intervalTime = 0.5f;

			DoIdle();
		}
	}

	if (m_isDeadAnim)
	{
		if (m_meshCom->IsAnimationSetEnd())
			m_isDead = true;
	}

	if (m_isInterval)
	{
		m_intervalTime -= deltaTime;

		if (0.f > m_intervalTime)
		{
			m_isInterval = false;
			m_intervalTime = 0.f;
		}
	}
}

void CGoblinMagician::MonsterAI(const _float& deltaTime)
{
	if (m_isInterval || m_isHit || m_isAttack || m_isDeadAnim) return;

	Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTrans) return;

	_vec3 targetPos, pos;
	playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	_float distance = D3DXVec3Length(&(targetPos - pos));

	if (!m_isSpawned)
	{
		if (30.f >= distance)
			DoSpawn(targetPos, pos);
		else if (!m_isSpawn)
			DoIdle();
	}
	else
	{
		if (20.f >= distance)
		{
			DoAttack(targetPos, pos);
		}
		else if (30.f >= distance)
		{
			DoChase(targetPos, pos, deltaTime);
		}
		else
		{
			DoIdle();
		}
	}
}

void CGoblinMagician::DoChase(const _vec3 & target, const _vec3 & myPos, const _float & deltaTime)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		LookAtTarget(target);

		_vec3 dir = target - myPos;
		D3DXVec3Normalize(&dir, &dir);

		dir *= deltaTime * m_speed;

		m_transCom->MovePos(&dir);

		m_meshCom->SetAnimation(5, 0.15f, 0.1f, false);
	}
}

void CGoblinMagician::DoAttack(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		// 24ÇÁ·¹ÀÓ
		SpawnFireBall((target + _vec3(0.f, 1.f, 0.f)), myPos);
		LookAtTarget(target);
		m_meshCom->SetAnimation(0, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CGoblinMagician::DoSpawn(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isSpawn)
	{
		LookAtTarget(target);
		m_meshCom->SetAnimation(1, 0.01f, 0.01f, true);

		m_isSpawn = true;
	}
}

void CGoblinMagician::DoIdle()
{
	if (!m_isHit  && !m_isAttack && !m_isDeadAnim)
	{
		m_meshCom->SetAnimation(4, 0.15f, 0.1f, false);
	}
}

void CGoblinMagician::DoHit()
{
	if (!m_isDeadAnim && !m_isSpawn)
	{
		if (m_isHit)
			m_meshCom->ResetAnimation();

		m_isAttack = false;
		m_isHit = true;

		m_meshCom->SetAnimation(3, 0.015f, 0.1f, true);
	}
}

void CGoblinMagician::DoDeadAnim()
{
	if (!m_isDeadAnim)
	{
		m_isDeadAnim = true;
		m_isHit = false;
		m_isAttack = false;
		DisableHitCollider();

		m_meshCom->SetAnimation(2, 0.015f, 0.1f, false);
	}
}

void CGoblinMagician::LookAtTarget(const _vec3& targetPos)
{
	_vec3 myPos;
	m_transCom->GetInfo(Engine::INFO_POS, &myPos);

	_vec3 targetDir = targetPos - myPos;
	targetDir.y = 0.f;
	D3DXVec3Normalize(&targetDir, &targetDir);

	_vec3 curDir = { 0.f, 0.f, 1.f };

	_float dot = D3DXVec3Dot(&curDir, &targetDir);
	_float cosAngle = acosf(dot);

	_float angle = (curDir.x * targetDir.z - curDir.z * targetDir.x > 0.f) ? cosAngle : -cosAngle;
	_float degree = D3DXToDegree(angle);

	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(-degree));
}

void CGoblinMagician::SpawnFireBall(const _vec3& targetPos, const _vec3 & myPos)
{
	CSpawnManager::GetInstance()->Spawn(SPAWNTYPE::FIREBALL, myPos + _vec3(0.f, 2.f, 0.f), 0.f, &targetPos);
}

void CGoblinMagician::SpawnGoblinSword()
{
	if (!m_isSpawned)
	{
		Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		if (nullptr == playerTrans) return;

		_vec3 targetPos, myPos, myRight;
		playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
		m_transCom->GetInfo(Engine::INFO_POS, &myPos);
		m_transCom->GetInfo(Engine::INFO_RIGHT, &myRight);

		_vec3 tarDir = targetPos - myPos;
		D3DXVec3Normalize(&tarDir, &tarDir);

		_vec3 pos = myPos + (5.f * tarDir);
		_vec3 pos2 = myPos + (5.f * myRight);
		_vec3 pos3 = myPos - (5.f * myRight);

		CSpawnManager::GetInstance()->Spawn(SPAWNTYPE::GOBLIN_SWORD, pos, m_angle);
		CSpawnManager::GetInstance()->Spawn(SPAWNTYPE::GOBLIN_SWORD, pos2, m_angle);
		CSpawnManager::GetInstance()->Spawn(SPAWNTYPE::GOBLIN_SWORD, pos3, m_angle);

		m_isSpawned = true;
	}
}

CGoblinMagician* CGoblinMagician::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle)
{
	CGoblinMagician* instance = new CGoblinMagician(device, pos, angle);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CGoblinMagician::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_ENEMY, this);
	CSpawnManager::GetInstance()->DeleteObject(SPAWNTYPE::GOBLIN_MAGICIAN, this);

	Engine::CGameObject::Free();
}