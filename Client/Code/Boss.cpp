#include "stdafx.h"
#include "Boss.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Texture.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle) :
	Engine::CGameObject(device),
	m_startPosition(pos),
	m_angle(angle)
{

}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready()
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
	m_phase = 1;

	m_hp = 200;
	m_maxHp = 200;

	m_intervalTime = 0.f;

	DoIdle();

	return S_OK;
}

_int CBoss::Update(const _float& deltaTime)
{
	CalcState(deltaTime);
	MonsterAI(deltaTime);

	m_meshCom->PlayAnimation(deltaTime);
	m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);

	_vec3 movePos;

	if (m_meshCom->CanCalcMovePos("Bip001", movePos))
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

void CBoss::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	_int passIndex[3] = { 4, 4, 4 };

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect, passIndex);

	effect->End();

	Engine::SafeRelease(effect);
}

_bool CBoss::AttackColliderOverlapped(Engine::CGameObject * target)
{
	if (IsInCollideList(target)) return false;

	m_collideList.emplace_back(target);

	return true;
}

void CBoss::HitColliderOverlapped(Engine::CGameObject * causer)
{
	m_hp -= 20;

	if (m_hp > 0)
	{
		for (auto& elem : m_hitCollider)
			elem->SetIsCollide(true);

		DisableAttackCollider();
		DoKnockUp();
	}
	else
	{
		DisableAttackCollider();
		DoDeadAnim();
	}

	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{

		Engine::CTransform* causerTrans = dynamic_cast<Engine::CTransform*>(causer->GetComponent(L"Com_Transform", Engine::ID_DYNAMIC));
		if (nullptr == causerTrans) return;

		_vec3 pos;
		causerTrans->GetInfo(Engine::INFO_POS, &pos);

		LookAtTarget(pos);
	}
}

HRESULT CBoss::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Boss"));
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

	// weapon - body - hair
	// weapon
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_Boss_Weapon"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_WeaponNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	// body
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_Boss_Body"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_BodyNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	// hair
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_Boss_Hair"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_HairNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	return S_OK;
}

HRESULT CBoss::LoadCollider()
{
	m_hitCollider.reserve(1);
	m_attackCollider.reserve(1);

	HANDLE hFile = CreateFile(L"../Data/Boss_Collider.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

HRESULT CBoss::SetUpConstantTable(LPD3DXEFFECT & effect)
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

void CBoss::CalcState(const _float & deltaTime)
{
	_bool isIdle = false;

	if (1 == m_phase)
	{
		if (m_hp <= m_maxHp / 2)
		{
			m_phase = 2;
		}
	}

	if (m_isHit)
	{
		if (m_meshCom->IsAnimationSetEnd())
		{
			m_isHit = false;
			m_isInterval = true;
			m_intervalTime = 1.f;
			isIdle = true;
		}
	}

	if (m_isAttack)
	{
		if (m_meshCom->IsAnimationSetEnd())
		{
			DisableAttackCollider();
			m_isAttack = false;
			m_isInterval = true;
			m_intervalTime = 1.f;
			isIdle = true;
		}
	}

	if (isIdle)
	{
		switch (m_phase)
		{
		case 1: DoIdle(); break;
		case 2: DoRageIdle(); break;
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

void CBoss::MonsterAI(const _float& deltaTime)
{
	if (m_isInterval || m_isHit || m_isAttack || m_isDeadAnim) return;

	switch (m_phase)
	{
	case 1: DoPhase1(deltaTime); break;
	case 2: DoPhase2(deltaTime); break;
	}
}

void CBoss::DoPhase1(const _float & deltaTime)
{
	Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTrans) return;

	_vec3 targetPos, pos;
	playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	_float distance = D3DXVec3Length(&(targetPos - pos));

	if (1.f >= distance)
	{
		DoAttack5(targetPos, pos);
	}
	else if (2.f >= distance)
	{
		DoAttack2(targetPos, pos);
	}
	else if (8.f <= distance && distance <= 10.f)
	{
		_int randNum = rand() % 2;

		if (randNum)
			DoAttack3(targetPos, pos);
		else
			DoAttack1(targetPos, pos);
	}
	else if (20.f >= distance)
	{
		DoChase(targetPos, pos, deltaTime);
	}
	else
	{
		DoIdle();
	}
}

void CBoss::DoPhase2(const _float & deltaTime)
{
	Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTrans) return;

	_vec3 targetPos, pos;
	playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	_float distance = D3DXVec3Length(&(targetPos - pos));

	if (5.f >= distance)
	{
		DoRageAttack1(targetPos, pos);
	}
	/*else if (2.f >= distance)
	{
		DoRageAttack2(targetPos, pos);
	}*/
	else if (8.f <= distance && distance <= 10.f)
	{
		_int randNum = rand() % 2;

		if (randNum)
			DoRageAttack2(targetPos, pos);
		else
			DoRageAttack3(targetPos, pos);
	}
	else if (20.f >= distance)
	{
		DoChase(targetPos, pos, deltaTime);
	}
	else
	{
		DoRageIdle();
	}
}

void CBoss::DoChase(const _vec3 & target, const _vec3 & myPos, const _float & deltaTime)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		LookAtTarget(target);

		_vec3 dir = target - myPos;
		D3DXVec3Normalize(&dir, &dir);

		dir *= deltaTime * m_speed;

		m_transCom->MovePos(&dir);

		m_meshCom->SetAnimation(20, 0.15f, 0.1f, false);
	}
}

void CBoss::DoAttack1(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(0, 0.01f, 0.05f, true);
		m_isAttack = true;
	}
}

void CBoss::DoAttack2(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(1, 0.01f, 0.05f, true);
		m_isAttack = true;
	}
}

void CBoss::DoAttack3(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(2, 0.01f, 0.05f, true);
		m_isAttack = true;
	}
}

void CBoss::DoAttack4(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(3, 0.01f, 0.05f, true);
		m_isAttack = true;
	}
}

void CBoss::DoAttack5(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(4, 0.01f, 0.05f, true);
		m_isAttack = true;
	}
}

void CBoss::DoRageAttack1(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(12, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CBoss::DoRageAttack2(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(13, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CBoss::DoRageAttack3(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(14, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CBoss::DoRageAttack4(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(15, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CBoss::DoRageAttack5(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		EnableAttackCollider();

		LookAtTarget(target);
		m_meshCom->SetAnimation(16, 0.01f, 0.1f, true);
		m_isAttack = true;
	}
}

void CBoss::DoIdle()
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		m_meshCom->SetAnimation(11, 0.15f, 0.1f, false);
	}
}

void CBoss::DoRageIdle()
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		m_meshCom->SetAnimation(17, 0.15f, 0.1f, false);
	}
}

void CBoss::DoKnockBack()
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

void CBoss::DoKnockUp()
{
	if (!m_isDeadAnim && !m_isAttack)
	{
		if (m_isHit)
			m_meshCom->ResetAnimation();

		m_isAttack = false;
		m_isHit = true;

		m_meshCom->SetAnimation(9, 0.015f, 0.1f, true);
	}
}

void CBoss::DoDeadAnim()
{
	if (!m_isDeadAnim)
	{
		m_isDeadAnim = true;
		m_isHit = false;
		m_isAttack = false;
		DisableHitCollider();

		m_meshCom->SetAnimation(6, 0.015f, 0.1f, false);
	}
}

void CBoss::LookAtTarget(const _vec3& targetPos)
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

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle)
{
	CBoss* instance = new CBoss(device, pos, angle);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CBoss::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_ENEMY, this);
	CSpawnManager::GetInstance()->DeleteObject(SPAWNTYPE::BOSS, this);

	Engine::CGameObject::Free();
}