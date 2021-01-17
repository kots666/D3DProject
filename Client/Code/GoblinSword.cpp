#include "stdafx.h"
#include "GoblinSword.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "HPUI.h"

CGoblinSword::CGoblinSword(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle) :
	Engine::CGameObject(device),
	m_startPosition(pos),
	m_angle(angle)
{

}

CGoblinSword::~CGoblinSword()
{
}

HRESULT CGoblinSword::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	D3DXMatrixRotationY(&m_reviseMat, D3DXToRadian(-90.f));

	m_transCom->SetPos(m_startPosition);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->SetRotation(Engine::ROT_Y, m_angle);

	m_meshCom->SetBoneName("Bip001");

	CColliderManager::GetInstance()->AddObject(OBJ_ENEMY, this);

	m_isInterval = false;
	m_isHit = false;
	m_isAttack = false;
	m_isDeadAnim = false;

	m_intervalTime = 0.f;

	return S_OK;
}

_int CGoblinSword::Update(const _float& deltaTime)
{
	CalcState(deltaTime);
	MonsterAI(deltaTime);

	m_meshCom->PlayAnimation(deltaTime);
	m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);

	_vec3 movePos;

	if (m_meshCom->CanCalcMovePos("RootBone", movePos))
	{
		//D3DXVec3TransformNormal(&movePos, &movePos, &m_yScaleRotMat);
		//movePos.x = 0;
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

void CGoblinSword::Render()
{
	m_transCom->SetTransform(m_device);
	m_meshCom->Render();

//#ifdef _DEBUG
//	for (auto& elem : m_hitCollider)
//	{
//		elem->Render();
//	}
//
//	for (auto& elem : m_attackCollider)
//	{
//		elem->Render();
//	}
//#endif
}

_bool CGoblinSword::AttackColliderOverlapped(Engine::CGameObject * target)
{
	if (IsInCollideList(target)) return false;

	m_collideList.emplace_back(target);

	return true;
}

void CGoblinSword::HitColliderOverlapped(Engine::CGameObject * causer)
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

HRESULT CGoblinSword::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Goblin_Sword"));
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

	return S_OK;
}

HRESULT CGoblinSword::LoadCollider()
{
	m_hitCollider.reserve(1);
	m_attackCollider.reserve(1);

	HANDLE hFile = CreateFile(L"../Data/Goblin_Sword_Collider.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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
		if (0 == cnt)
			m_hitCollider.emplace_back(newSphere);
		else
			m_attackCollider.emplace_back(newSphere);

		++cnt;
	}
	CloseHandle(hFile);

	for (auto& elem : m_hitCollider)
		elem->SetCanCollide(true);

	for (auto& elem : m_hitCollider)
		elem->UpdateByBone(m_transCom->GetWorldMatrix());

	for (auto& elem : m_attackCollider)
		elem->UpdateByBone(m_transCom->GetWorldMatrix());

	return S_OK;
}

void CGoblinSword::CalcState(const _float & deltaTime)
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

		if (0.f >= m_intervalTime)
		{
			m_isInterval = false;
			m_intervalTime = 0.f;
		}
	}
}

void CGoblinSword::MonsterAI(const _float& deltaTime)
{
	if (m_isInterval || m_isHit || m_isAttack || m_isDeadAnim) return;

	Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTrans) return;

	_vec3 targetPos, pos;
	playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	_float distance = D3DXVec3Length(&(targetPos - pos));

	if (2.f >= distance)
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

void CGoblinSword::DoChase(const _vec3 & target, const _vec3 & myPos, const _float & deltaTime)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		LookAtTarget(target);

		_vec3 dir = target - myPos;
		D3DXVec3Normalize(&dir, &dir);

		dir *= deltaTime * m_speed;

		m_transCom->MovePos(&dir);

		m_meshCom->SetAnimation(3, 0.15f, 0.1f, false);
	}
}

void CGoblinSword::DoAttack(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		LookAtTarget(target);
		m_meshCom->SetAnimation(0, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CGoblinSword::DoIdle()
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		m_meshCom->SetAnimation(2, 0.15f, 0.1f, false);
	}
}

void CGoblinSword::DoHit()
{
	if (!m_isDeadAnim)
	{
		if (m_isHit)
			m_meshCom->ResetAnimation();

		m_isAttack = false;
		m_isHit = true;

		m_meshCom->SetAnimation(7, 0.015f, 0.1f, true);
	}
}

void CGoblinSword::DoDeadAnim()
{
	if (!m_isDeadAnim)
	{
		m_isDeadAnim = true;
		m_isHit = false;
		m_isAttack = false;

		m_meshCom->SetAnimation(1, 0.015f, 0.1f, false);
	}
}

void CGoblinSword::LookAtTarget(const _vec3& targetPos)
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

CGoblinSword* CGoblinSword::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle)
{
	CGoblinSword* instance = new CGoblinSword(device, pos, angle);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CGoblinSword::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_ENEMY, this);
	CSpawnManager::GetInstance()->DeleteObject(SPAWNTYPE::GOBLIN_SWORD, this);

	Engine::CGameObject::Free();
}