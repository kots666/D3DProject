#include "stdafx.h"
#include "Player.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "TerrainTex.h"
#include "DynamicCamera.h"
#include "TrailRect.h"
#include "Texture.h"
#include "SwordTrail.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device),
	m_dir(0.f, 0.f, 0.f),
	m_yRotAngle(0.f)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	m_camera = dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"));

	D3DXMatrixRotationY(&m_reviseMat, D3DXToRadian(-90.f));

	m_transCom->SetPos(12.f, 0.3f, 4.f);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->Update(0.1f);
	
	m_naviMeshCom->SetNaviIndex(0);

	// Player
	m_meshCom->SetAnimation(0, 0.15f, 0.1f, false);
	m_meshCom->SetIsRootMotion(false);
	m_meshCom->SetBoneName("Bip001");

	m_speed = 10.f;
	m_animationSpeed = 1.f;

	CColliderManager::GetInstance()->AddObject(OBJ_PLAYER, this);

	m_state = STATE_IDLE;

	return S_OK;
}

_int CPlayer::Update(const _float& deltaTime)
{
	//SetUpOnTerrain();
	CalcComboTime(deltaTime);
	KeyInput(deltaTime);
	UpdateAnimMatrices();

	m_meshCom->PlayAnimation(deltaTime, m_animationSpeed);
	m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);
	
	_vec3 movePos;
	if (m_meshCom->CanCalcMovePos("Bip001", movePos) /*&& 0 < m_comboIndex*/)
	{
		D3DXVec3TransformNormal(&movePos, &movePos, &m_yScaleRotMat);
		movePos.y = 0;
		_vec3 nowPos;
		_vec3 nowDir;
		m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
		m_transCom->GetInfo(Engine::INFO_LOOK, &nowDir);

		_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(movePos));

		m_transCom->SetPos(moveDist);
	}

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	Engine::CGameObject::Update(deltaTime);

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

	/*for (auto iter = m_trailVec.begin(); iter != m_trailVec.end();)
	{
		(*iter)->DecreaseLifeTime(deltaTime);

		if (0.f >= (*iter)->GetLifeTime())
		{
			Engine::SafeRelease(*iter);
			iter = m_trailVec.erase(iter);
		}
		else
			++iter;
	}*/

	if (m_state & STATE_ATTACK)
	{
		RecordPos();
	}

	m_swordTrail->Update(deltaTime);
	m_swordTrail->SetWorldMat(*m_transCom->GetWorldMatrix());

	/*_vec3 curPos;
	m_transCom->GetInfo(Engine::INFO_POS, &curPos);

	cout << curPos.x << "\t" << curPos.y << "\t" << curPos.z << endl;*/

	return 0;
}

void CPlayer::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	_uint maxPass = 0;

	effect->Begin(&maxPass, 0);
	effect->BeginPass(0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	m_meshCom->Render(effect);

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

_bool CPlayer::AttackColliderOverlapped(Engine::CGameObject * target)
{
	if (IsInCollideList(target)) return false;

	m_collideList.emplace_back(target);

	//cout << "Attack" << endl;

	return true;
}

void CPlayer::HitColliderOverlapped(Engine::CGameObject * causer)
{
	if (m_hp > 0)
	{
		m_hp -= 10;
	}

	for (auto& elem : m_hitCollider)
		elem->SetIsCollide(true);

	//cout << "Hit" << endl;
}

HRESULT CPlayer::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Player"));
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

	// Calculator
	component = m_calcCom = dynamic_cast<Engine::CCalculator*>(Engine::CloneComp(L"Proto_Calculator"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Calculator", component);

	component = m_trailTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Trail"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	m_swordTrail = CSwordTrail::Create(m_device);
	NULL_CHECK_RETURN(m_swordTrail, E_FAIL);

	return S_OK;
}

HRESULT CPlayer::SetUpConstantTable(LPD3DXEFFECT & effect)
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

HRESULT CPlayer::LoadCollider()
{
	m_hitCollider.reserve(1);
	m_attackCollider.reserve(6);

	HANDLE hFile = CreateFile(L"../Data/Gladiator_Collider.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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
	{
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
		elem->SetCanCollide(true);
	}

	for (auto& elem : m_attackCollider)
		elem->UpdateByBone(m_transCom->GetWorldMatrix());

	return S_OK;
}

void CPlayer::KeyInput(const _float& deltaTime)
{
	if(nullptr == m_camera)
		m_camera = dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"));

	m_transCom->GetInfo(Engine::INFO_LOOK, &m_dir);

	_bool isAnyKeyDown = false;

	if (m_meshCom->IsAnimationSetEnd())
	{
		DoIdle();
	}

	if ((m_state == STATE_IDLE) && nullptr != m_camera)
	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			isAnyKeyDown = true;
			m_speed = 10.f;

			_vec3 originDir = { 0.f, 0.f, 1.f };
			_vec3 nowPos, nowDir;

			_float camYDegree = m_camera->GetYDegree();

			_matrix rotY;

			// ÁÂ»ó
			if (GetAsyncKeyState('A') & 0x8000)
			{
				m_speed /= sqrtf(2.f);
				camYDegree -= 45.f;
			}
			// ¿ì»ó
			else if (GetAsyncKeyState('D') & 0x8000)
			{
				m_speed /= sqrtf(2.f);
				camYDegree += 45.f;
			}

			D3DXMatrixRotationY(&rotY, D3DXToRadian(camYDegree));
			D3DXVec3TransformNormal(&nowDir, &originDir, &rotY);

			m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
			D3DXVec3Normalize(&nowDir, &nowDir);

			_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(nowDir * deltaTime * m_speed));
			m_transCom->SetPos(moveDist);
			
			m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(camYDegree));
			m_yRotAngle = camYDegree;

			// Player
			m_meshCom->SetAnimation(1, 0.15f, 0.01f, false);
		}

		else if (GetAsyncKeyState('S') & 0x8000)
		{
			isAnyKeyDown = true;
			m_speed = 10.f;

			_vec3 originDir = { 0.f, 0.f, 1.f };
			_vec3 nowPos, nowDir;

			_float camYDegree = m_camera->GetYDegree() - 180.f;

			_matrix rotY;

			// ÁÂÇÏ
			if (GetAsyncKeyState('A') & 0x8000)
			{
				m_speed /= sqrtf(2.f);
				camYDegree += 45.f;
			}
			// ¿ìÇÏ
			else if (GetAsyncKeyState('D') & 0x8000)
			{
				m_speed /= sqrtf(2.f);
				camYDegree -= 45.f;
			}

			D3DXMatrixRotationY(&rotY, D3DXToRadian(camYDegree));
			D3DXVec3TransformNormal(&nowDir, &originDir, &rotY);

			m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
			D3DXVec3Normalize(&nowDir, &nowDir);

			_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(nowDir * deltaTime * m_speed));
			m_transCom->SetPos(moveDist);

			m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(camYDegree));
			m_yRotAngle = camYDegree;

			// Player
			m_meshCom->SetAnimation(1, 0.15f, 0.01f, false);
		}

		else if (GetAsyncKeyState('A') & 0x8000)
		{
			isAnyKeyDown = true;
			m_speed = 10.f;

			_vec3 originDir = { 0.f, 0.f, 1.f };
			_vec3 nowPos, nowDir;

			_float camYDegree = m_camera->GetYDegree() - 90.f;

			_matrix rotY;
			D3DXMatrixRotationY(&rotY, D3DXToRadian(camYDegree));
			D3DXVec3TransformNormal(&nowDir, &originDir, &rotY);

			m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
			D3DXVec3Normalize(&nowDir, &nowDir);

			_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(nowDir * deltaTime * m_speed));
			m_transCom->SetPos(moveDist);

			m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(camYDegree));
			m_yRotAngle = camYDegree;

			// Player
			m_meshCom->SetAnimation(1, 0.15f, 0.01f, false);
		}

		else if (GetAsyncKeyState('D') & 0x8000)
		{
			isAnyKeyDown = true;
			m_speed = 10.f;

			_vec3 originDir = { 0.f, 0.f, 1.f };
			_vec3 nowPos, nowDir;

			_float camYDegree = m_camera->GetYDegree() + 90.f;

			_matrix rotY;
			D3DXMatrixRotationY(&rotY, D3DXToRadian(camYDegree));
			D3DXVec3TransformNormal(&nowDir, &originDir, &rotY);

			m_transCom->GetInfo(Engine::INFO_POS, &nowPos);
			D3DXVec3Normalize(&nowDir, &nowDir);

			_vec3 moveDist = m_naviMeshCom->MoveOnNaviMesh(&nowPos, &(nowDir * deltaTime * m_speed));
			m_transCom->SetPos(moveDist);

			m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(camYDegree));
			m_yRotAngle = camYDegree;

			// Player
			m_meshCom->SetAnimation(1, 0.15f, 0.01f, false);
		}
	}

	if (Engine::GetDIMouseState(Engine::DIM_LB) & 0x80)
	{
		DoAttack();
	}

	if (Engine::GetDIKeyDownState(VK_RBUTTON))
	{
		CSpawnManager::GetInstance()->Spawn(SPAWNTYPE::DOG, { 10.f, 0.f, 10.f });
		//_vec3 pickPos = PickUpOnTerrain();
		//m_transCom->MoveToPickPos(&pickPos, m_speed, deltaTime);
	}

	if (!isAnyKeyDown && (m_state == STATE_IDLE))
	{
		m_meshCom->SetAnimation(0, 0.15f, 0.1f, false);
		m_comboIndex = 0;
	}
}

void CPlayer::SetUpOnTerrain()
{
	_vec3 position;
	m_transCom->GetInfo(Engine::INFO_POS, &position);

	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(terrainBufferCom);

	_float height = m_calcCom->ComputeHeightOnTerrain(&position, terrainBufferCom->GetVtxPos(), VTXCNTX, VTXCNTZ, VTXITV);

	m_transCom->SetPos(position.x, height, position.z);
}

_vec3 CPlayer::PickUpOnTerrain()
{
	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	if (nullptr == terrainBufferCom)
		return _vec3(0.f, 0.f, 0.f);

	Engine::CTransform* terrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == terrainTransformCom)
		return _vec3(0.f, 0.f, 0.f);

	return m_calcCom->PickingOnTerrain(g_hWnd, terrainBufferCom, terrainTransformCom);
}

void CPlayer::UpdateAnimMatrices()
{
	D3DXQUATERNION qt;
	_vec3 up = { 0.f, 1.f, 0.f };

	D3DXQuaternionRotationAxis(&qt, &up, D3DXToRadian(m_yRotAngle - 90.f));
	D3DXMatrixRotationQuaternion(&m_yRotMat, &qt);

	D3DXMatrixScaling(&m_yScaleRotMat, 0.01f, 0.01f, 0.01f);

	m_yScaleRotMat *= m_yRotMat;
}

void CPlayer::CalcComboTime(const _float& deltaTime)
{
	if (m_isCombo)
	{
		m_accTime += deltaTime;
	}
}

void CPlayer::DoIdle()
{
	// Player
	m_meshCom->SetAnimation(0, 0.15f, 0.1f, false);
	m_animationSpeed = 1.f;
	m_comboIndex = 0;
	m_isCombo = false;
	m_accTime = 0.f;
	m_comboTime = 0.f;

	m_state = STATE_IDLE;

	for (auto& elem : m_attackCollider)
		elem->SetCanCollide(false);

	m_collideList.clear();
}

void CPlayer::DoAttack()
{
	if (m_comboTime <= m_accTime)
	{
		m_state |= STATE_ATTACK;

		++m_comboIndex;

		switch (m_comboIndex)
		{
		case 1:
			m_meshCom->SetAnimation(2, 0.015f, 0.03f, true);
			m_isCombo = true;
			m_animationSpeed = 1.5f;
			m_accTime = 0.f;
			m_comboTime = 0.73f / m_animationSpeed;
			break;

		case 2:
			m_meshCom->SetAnimation(3, 0.015f, 0.03f, true);
			m_isCombo = true;
			m_animationSpeed = 1.5f;
			m_accTime = 0.f;
			m_comboTime = 0.73f / m_animationSpeed;
			break;

		case 3:
			m_meshCom->SetAnimation(4, 0.01f, 0.03f, true);
			m_isCombo = true;
			m_animationSpeed = 1.5f;
			m_accTime = 0.f;
			m_comboTime = 1.46f / m_animationSpeed;
			break;

		case 4:
			m_meshCom->SetAnimation(5, 0.01f, 0.04f, true);
			m_isCombo = false;
			m_animationSpeed = 1.5f;
			m_accTime = 0.f;
			m_comboTime = 0.f;
			break;
		}

		_vec3 originDir = { 0.f, 0.f, 1.f };
		_vec3 nowPos, nowDir;

		_float camYDegree = m_camera->GetYDegree();

		_matrix rotY;
		D3DXMatrixRotationY(&rotY, D3DXToRadian(camYDegree));
		D3DXVec3TransformNormal(&nowDir, &originDir, &rotY);

		m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(camYDegree));
		m_yRotAngle = camYDegree;

		for (auto& elem : m_attackCollider)
			elem->SetCanCollide(true);

		m_collideList.clear();
	}
}

void CPlayer::RecordPos()
{
	_vec3 endOffset = { 0.f, -175.f, 0.f };
	_vec3 lowOffset = { 0.f, -140.f, 0.f };

	auto frame = m_meshCom->GetCloneFrameByName("Bone_R_Weapon");
	_matrix frameMat = frame->combinedTransformationMatrix;

	D3DXVec3TransformCoord(&endOffset, &endOffset, &frameMat);
	D3DXVec3TransformCoord(&lowOffset, &lowOffset, &frameMat);

	m_swordTrail->AddPos(endOffset, lowOffset);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 device)
{
	CPlayer* instance = new CPlayer(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CPlayer::Free()
{
	Engine::SafeRelease(m_swordTrail);

	Engine::CGameObject::Free();
}