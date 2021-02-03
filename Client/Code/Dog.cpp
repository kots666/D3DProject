#include "stdafx.h"
#include "Dog.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "HPUI.h"
#include "Texture.h"

CDog::CDog(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle) :
	Engine::CGameObject(device),
	m_startPosition(pos),
	m_startAngle(angle)
{

}

CDog::~CDog()
{
}

HRESULT CDog::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	D3DXMatrixRotationY(&m_reviseMat, D3DXToRadian(-90.f));

	m_transCom->SetPos(m_startPosition);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(m_startAngle));

	m_meshCom->SetBoneName("Bip001");

	CColliderManager::GetInstance()->AddObject(OBJ_ENEMY, this);

	m_isInterval = false;
	m_isHit = false;
	m_isAttack = false;
	m_isDeadAnim = false;
	m_isDissolve = false;

	m_dissolveAmount = 0.f;

	m_intervalTime = 0.f;

	CHPUIManager::GetInstance()->Create(m_device, this);

	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	m_naviMeshCom->UpdateCurrentIndex(&pos);

	return S_OK;
}

_int CDog::Update(const _float& deltaTime)
{
	if (!m_isDissolve)
	{
		CalcState(deltaTime);
		MonsterAI(deltaTime);
		CalcAttack(deltaTime);

		m_meshCom->PlayAnimation(deltaTime);
		m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);

		_vec3 movePos;

		if (m_meshCom->CanCalcMovePos("RootBone", movePos))
		{
			movePos.y = 0;
			movePos.z = 0;

			_vec3 nowPos;
			_vec3 nowDir;
			m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
			m_transCom->GetInfo(Engine::INFO_LOOK, &nowDir);

			nowDir *= movePos.x;

			_vec3 moveDist = nowPos + nowDir;

			_vec3 movePos = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &nowDir);

			m_transCom->SetPos(movePos);
		}

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
	}
	else if(m_isDissolve)
	{
		m_dissolveAmount += deltaTime;
		if (1.f < m_dissolveAmount)
			m_isDead = true;
	}

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	m_naviMeshCom->UpdateCurrentIndex(&pos);

	return 0;
}

void CDog::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect, 4);
	
	effect->End();

	Engine::SafeRelease(effect);
}

_bool CDog::AttackColliderOverlapped(Engine::CGameObject * target)
{
	if (IsInCollideList(target)) return false;

	m_collideList.emplace_back(target);

	return true;
}

void CDog::HitColliderOverlapped(Engine::CGameObject * causer)
{
	_float randNum = ((rand() % 5) + 8) * 0.1f;
	_int fixDamage = causer->GetATK() * randNum;
	m_hp -= fixDamage;

	Engine::CTransform* causerTrans = dynamic_cast<Engine::CTransform*>(causer->GetComponent(L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == causerTrans) return;

	_vec3 targetPos;
	causerTrans->GetInfo(Engine::INFO_POS, &targetPos);

	LookAtTarget(targetPos);

	_vec3 myPos;
	m_transCom->GetInfo(Engine::INFO_POS, &myPos);

	CFontManager::GetInstance()->ActiveNumber(fixDamage, myPos);

	if (m_hp > 0)
	{
		DoKnockBack(targetPos, myPos);
		DoHit();
	}
	else
	{
		DoDeadAnim();
	}

	_vec3 spawnPos = targetPos - myPos;
	spawnPos *= 0.6f;

	spawnPos.y += 1.f;

	spawnPos += myPos;

	CHitManager::GetInstance()->SpawnHitEffect(spawnPos);
	CHitManager::GetInstance()->SpawnHitSlash(spawnPos);
}

HRESULT CDog::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Dog"));
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

	// NormalTexture
	component = m_normalTex = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_Dog_Normal"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NormalTexture", component);

	m_meshCom->AddNormalTexture(m_normalTex);

	// DissolveTexture
	component = m_dissolveTex = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Dissolve"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", component);
	
	return S_OK;
}

HRESULT CDog::LoadCollider()
{
	m_hitCollider.reserve(10);
	m_attackCollider.reserve(10);

	HANDLE hFile = CreateFile(L"../Data/Dog_Collider.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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
		if (4 > cnt)
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

HRESULT CDog::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	m_transCom->GetWorldMatrix(&matWorld);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	m_dissolveTex->SetTexture(effect, "g_DissolveTexture");

	effect->SetFloat("g_DissolveAmount", m_dissolveAmount);

	return S_OK;
}

void CDog::CalcState(const _float & deltaTime)
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
			DisableAttackCollider();
			m_isAttack = false;
			m_isInterval = true;
			m_intervalTime = 1.5f;
			DoIdle();
		}
	}

	if (m_isDeadAnim)
	{
		if (m_meshCom->IsAnimationSetEnd())
			m_isDissolve = true;
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

void CDog::MonsterAI(const _float& deltaTime)
{
	if (m_isInterval || m_isHit || m_isAttack || m_isDeadAnim) return;

	Engine::CTransform* playerTrans = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTrans) return;

	_vec3 targetPos, pos;
	playerTrans->GetInfo(Engine::INFO_POS, &targetPos);
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	_float distance = D3DXVec3Length(&(targetPos - pos));

	if (4.f >= distance)
	{
		DoAttack(targetPos, pos);
	}
	else if (10.f >= distance)
	{
		DoChase(targetPos, pos, deltaTime);
	}
	else
	{
		DoIdle();
	}
}

void CDog::DoChase(const _vec3 & target, const _vec3 & myPos, const _float & deltaTime)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		LookAtTarget(target);

		_vec3 dir = target - myPos;
		D3DXVec3Normalize(&dir, &dir);

		//dir *= deltaTime * m_speed;

		_vec3 finalPos = m_naviMeshCom->MoveOnNaviMesh(&myPos, &(dir * deltaTime * m_speed));

		m_transCom->SetPos(finalPos);

		m_meshCom->SetAnimation(4, 0.15f, 0.01f, false);
	}
}

void CDog::DoAttack(const _vec3 & target, const _vec3 & myPos)
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		CSoundManager::PlayOverlapSound(L"Dog_Attack.ogg", SoundChannel::MONSTER, 0.1f, 0.1f);
		
		m_accTime = 0.f;
		CalcFrameTime(m_attackStartTime, 10.f);
		CalcFrameTime(m_attackEndTime, 26.f);

		LookAtTarget(target);
		m_meshCom->SetAnimation(1, 0.001f, 0.03f, true);
		m_isAttack = true;
	}
}

void CDog::DoIdle()
{
	if (!m_isHit && !m_isAttack && !m_isDeadAnim)
	{
		m_meshCom->SetAnimation(0, 0.15f, 0.1f, true);
	}
}

void CDog::DoHit()
{
	if (!m_isDeadAnim)
	{
		CSoundManager::PlayOverlapSound(L"Dog_Damage.ogg", SoundChannel::MONSTER, 0.1f, 0.1f);

		m_meshCom->ResetAnimation();

		m_isAttack = false;
		m_isHit = true;

		m_meshCom->SetAnimation(3, 0.015f, 0.1f, true);
	}
}

void CDog::DoDeadAnim()
{
	if (!m_isDeadAnim)
	{
		CSoundManager::PlayOverlapSound(L"Dog_Die.ogg", SoundChannel::MONSTER, 0.1f, 0.1f);

		DisableHitCollider();
		m_isDeadAnim = true;
		m_isHit = false;
		m_isAttack = false;

		m_meshCom->SetAnimation(2, 0.015f, 0.1f, false);
	}
}

void CDog::DoKnockBack(const _vec3 & targetPos, const _vec3 & myPos)
{
	_vec3 backDir = myPos - targetPos;
	backDir.y = 0.f;
	D3DXVec3Normalize(&backDir, &backDir);

	_vec3 finalPos = m_naviMeshCom->MoveOnNaviMesh(&myPos, &(backDir * 1.f));
	m_transCom->SetPosAfterUpdate(&finalPos);
}

void CDog::LookAtTarget(const _vec3& targetPos)
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

void CDog::CalcFrameTime(_float & outTime, const _float & frame)
{
	outTime = (frame / 30.f);
}

void CDog::CalcAttack(const _float& deltaTime)
{
	if (m_isAttack)
	{
		m_accTime += deltaTime;

		if (m_accTime > m_attackStartTime)
		{
			EnableAttackCollider();
		}
		
		if (m_accTime > m_attackEndTime)
		{
			DisableAttackCollider();
		}
	}
}

CDog* CDog::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle)
{
	CDog* instance = new CDog(device, pos, angle);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CDog::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_ENEMY, this);
	CSpawnManager::GetInstance()->DeleteObject(SPAWNTYPE::DOG, this);

	Engine::CGameObject::Free();
}