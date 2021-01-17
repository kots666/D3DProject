#include "stdafx.h"
#include "DynamicCamera.h"
#include "TerrainTex.h"
#include "MainFrm.h"
#include "MainView.h"
#include "Terrain.h"
#include "NaviMesh.h"
#include "NaviCell.h"
#include "SheetView.h"


CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 device) :
	Engine::CCamera(device)
{

}

CDynamicCamera::~CDynamicCamera()
{

}

HRESULT CDynamicCamera::Ready(const _vec3* eye, const _vec3* at, const _vec3* up, const _float& fovY, const _float& aspect, const _float& nearZ, const _float& farZ)
{
	m_eye = *eye;
	m_at = *at;
	m_up = *up;
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready(), E_FAIL);

	ShowCursor(false);

	m_mainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == m_mainFrame) return;

	m_toolView = dynamic_cast<CMainView*>(m_mainFrame->m_mainSplitter.GetPane(0, 1));
	if (nullptr == m_toolView) return;

	m_hWnd = m_toolView->GetSafeHwnd();

	FixMouse();
	
	return S_OK;
}

_int CDynamicCamera::Update(const _float& deltaTime)
{
	CSheetView* sheetView = dynamic_cast<CSheetView*>((m_mainFrame)->m_mainSplitter.GetPane(0, 0));
	if (nullptr != sheetView)
		m_canPickNaviMesh = sheetView->m_selectSheet->m_mapToolPage->m_isFocus;

	KeyInput(deltaTime);

	if (m_isFix)
	{
		MoveMouse();
		FixMouse();
	}
	else
	{
		if (Engine::GetDIKeyDownState(VK_LBUTTON) && m_canPickNaviMesh)
			MousePicking();
	}
	
	_int ret = Engine::CCamera::Update(deltaTime);
	
	return ret;
}

void CDynamicCamera::KeyInput(const _float& deltaTime)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		_vec3 look;
		memcpy(look, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&look, &look) * m_speed * deltaTime;

		m_eye += length;
		m_at += length;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		_vec3 look;
		memcpy(look, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&look, &look) * m_speed * deltaTime;

		m_eye -= length;
		m_at -= length;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		_vec3 right;
		memcpy(right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&right, &right) * m_speed * deltaTime;

		m_eye -= length;
		m_at -= length;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		_vec3 right;
		memcpy(right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&right, &right) * m_speed * deltaTime;

		m_eye += length;
		m_at += length;
	}

	// 마우스 전환
	if (Engine::GetDIKeyDownState(VK_LCONTROL))
	{
		m_canClick ^= true;
		m_isFix ^= true;

		if (m_isFix) ShowCursor(false);
		else ShowCursor(true);
	}
}

void CDynamicCamera::MoveMouse()
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long mouseMove = 0;
	// 마우스를 상하로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_Y))
	{
		_vec3 right;
		memcpy(&right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 look = m_at - m_eye;
		_matrix matRot;
		_float degree = D3DXToDegree(m_fovY);
		_float yGap = WINCY / degree;

		D3DXMatrixRotationAxis(&matRot, &right, D3DXToRadian(mouseMove / yGap));

		D3DXVec3TransformNormal(&look, &look, &matRot);

		m_at = m_eye + look;
	}

	// 마우스를 좌우로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_X))
	{
		_vec3 up = { 0.f, 1.f, 0.f };

		_vec3 look = m_at - m_eye;
		_matrix matRot;
		_float degree = D3DXToDegree(m_fovY);
		_float xGap = WINCX / degree;

		D3DXMatrixRotationAxis(&matRot, &up, D3DXToRadian(mouseMove / xGap));

		D3DXVec3TransformNormal(&look, &look, &matRot);

		m_at = m_eye + look;
	}
}

void CDynamicCamera::FixMouse()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(m_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MousePicking()
{
	POINT mouse{ 0 };

	GetCursorPos(&mouse);
	ScreenToClient(m_hWnd, &mouse);

	if (0 < mouse.x && mouse.x < WINCX && 0 < mouse.y && mouse.y < WINCY)
	{
		CalcRay();
		switch (m_pickType)
		{
		case 0: TerrainPicking(); break;
		case 1: NaviColliderPicking(); break;
		}
	}
}

void CDynamicCamera::CalcRay()
{
	POINT mouse{ 0 };

	GetCursorPos(&mouse);
	ScreenToClient(m_hWnd, &mouse);

	_vec3 mousePos;

	D3DVIEWPORT9 viewPort;
	ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));
	m_device->GetViewport(&viewPort);

	// 뷰포트 -> 투영

	mousePos.x = (mouse.x / (viewPort.Width * 0.5f)) - 1.f;
	mousePos.y = (mouse.y / -(viewPort.Height * 0.5f)) + 1.f;
	mousePos.z = 0.f;

	// L * W * V * P * (P^-1)
	// L * W * V

	// 투영 -> 뷰 스페이스
	_matrix	matProj;
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&mousePos, &mousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3 rayPos, rayDir;

	rayPos = _vec3(0.f, 0.f, 0.f);
	rayDir = mousePos - rayPos;

	D3DXVec3TransformCoord(&rayPos, &rayPos, &matView);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matView);

	m_rayPos = rayPos;
	m_rayDir = rayDir;
}

void CDynamicCamera::TerrainPicking()
{
	_vec3 rayPos = m_rayPos;
	_vec3 rayDir = m_rayDir;

	Engine::CTransform* terrainTransCom = dynamic_cast<CTransform*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK(terrainTransCom);

	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(terrainBufferCom);

	_vec3 pickPos = { 0.f, 0.f, 0.f };

	_float zeroValue = (-rayPos.y) / rayDir.y;

	pickPos.x = rayPos.x + rayDir.x * zeroValue;
	pickPos.z = rayPos.z + rayDir.z * zeroValue;

	// 월드 -> 로컬
	//_matrix	matWorld, invWorld;
	//terrainTransCom->GetWorldMatrix(&matWorld);
	//D3DXMatrixInverse(&invWorld, NULL, &matWorld);

	//D3DXVec3TransformCoord(&rayPos, &rayPos, &invWorld);
	//D3DXVec3TransformNormal(&rayDir, &rayDir, &invWorld);

	//_ulong vtxCntX = terrainBufferCom->GetVtxCntX();
	//_ulong vtxCntZ = terrainBufferCom->GetVtxCntZ();

	//const _vec3* terrainVtxPos = terrainBufferCom->GetVtxPos();

	//_ulong vtxIdx[3];
	//_float uValue, vValue, dist;
	//_float minDist = 99999.f;

	//for (_ulong i = 0; i < vtxCntZ - 1; ++i)
	//{
	//	for (_ulong j = 0; j < vtxCntX - 1; ++j)
	//	{
	//		_ulong index = i * vtxCntX + j;

	//		// 오른쪽 위
	//		vtxIdx[0] = index + vtxCntX;
	//		vtxIdx[1] = index + vtxCntX + 1;
	//		vtxIdx[2] = index + 1;

	//		// V1 + U(V2 - V1) + V(V3 - V1)
	//		if (D3DXIntersectTri(&terrainVtxPos[vtxIdx[0]],
	//			&terrainVtxPos[vtxIdx[1]],
	//			&terrainVtxPos[vtxIdx[2]], &rayPos, &rayDir, &uValue, &vValue, &dist))
	//		{
	//			if (minDist > dist)
	//			{
	//				minDist = dist;
	//				pickPos = terrainVtxPos[vtxIdx[0]] + uValue * (terrainVtxPos[vtxIdx[1]] - terrainVtxPos[vtxIdx[0]]) + vValue * (terrainVtxPos[vtxIdx[2]] - terrainVtxPos[vtxIdx[0]]);
	//			}
	//			//pickPos = rayPos + (rayDir * dist);
	//		}

	//		// 왼쪽 아래
	//		vtxIdx[0] = index + vtxCntX;
	//		vtxIdx[1] = index + 1;
	//		vtxIdx[2] = index;

	//		// V1 + U(V2 - V1) + V(V3 - V1)
	//		if (D3DXIntersectTri(&terrainVtxPos[vtxIdx[0]],
	//			&terrainVtxPos[vtxIdx[1]],
	//			&terrainVtxPos[vtxIdx[2]], &rayPos, &rayDir, &uValue, &vValue, &dist))
	//		{
	//			if (minDist > dist)
	//			{
	//				minDist = dist;
	//				pickPos = terrainVtxPos[vtxIdx[0]] + uValue * (terrainVtxPos[vtxIdx[1]] - terrainVtxPos[vtxIdx[0]]) + vValue * (terrainVtxPos[vtxIdx[2]] - terrainVtxPos[vtxIdx[0]]);
	//			}
	//		}
	//	}
	//}

	//D3DXVec3TransformCoord(&pickPos, &pickPos, &matWorld);

	if (0 == m_transType) FindNearlyPoint(&pickPos);

	CNaviMesh::GetInstance()->AddPos(pickPos);
}

void CDynamicCamera::FindNearlyPoint(_vec3 * outPos)
{
	_vec3 pos = *outPos;
	_vec3 minPos;
	_float min = 9999.f;
	_bool isCollide = false;

	vector<CNaviCell*>* cellVec = CNaviMesh::GetInstance()->GetCellList();

	for (auto cell : *cellVec)
	{
		_vec3* vertices = cell->GetAllPos();

		for (_int i = 0; i < 3; ++i)
		{
			_float dist = D3DXVec3Length(&(pos - vertices[i]));

			if (2.f >= dist)
			{
				if (min > dist)
				{
					minPos = vertices[i];
					min = dist;
					isCollide = true;
				}
			}
		}
	}
	if (isCollide)
		*outPos = minPos;
}

void CDynamicCamera::NaviColliderPicking()
{
	vector<CNaviCell*>* cellList = CNaviMesh::GetInstance()->GetCellList();
	_int size = cellList->size();

	for (_int i = 0; i < size; ++i)
	{
		_vec3* pos = (*cellList)[i]->GetAllPos();
		for (_int j = 0; j < 3; ++j)
		{
			if (IsCollideToSphere(m_rayPos, m_rayDir, pos[j], 0.2f))
			{
				// 처음 만난 애
				CMainFrame* main = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
				if (nullptr == main) return;

				CSheetView* selectView = dynamic_cast<CSheetView*>(main->m_mainSplitter.GetPane(0, 0));
				if (nullptr == selectView) return;

				selectView->m_selectSheet->m_mapToolPage->SelectPickedVertex(i, j);
			}
		}
	}
}

_bool CDynamicCamera::IsCollideToSphere(const _vec3 & rayPos, const _vec3 & rayDir, const _vec3 & centerPos, const _float & radius)
{
	_matrix invMat;
	D3DXMatrixIdentity(&invMat);

	invMat.m[3][0] = -centerPos.x;
	invMat.m[3][1] = -centerPos.y;
	invMat.m[3][2] = -centerPos.z;

	_vec3 pos, dir;
	D3DXVec3TransformCoord(&pos, &rayPos, &invMat);
	D3DXVec3TransformNormal(&dir, &rayDir, &invMat);

	float vv = D3DXVec3Dot(&dir, &dir);
	float qv = D3DXVec3Dot(&pos, &dir);
	float qq = D3DXVec3Dot(&pos, &pos);
	float rr = radius * radius;

	return (qv * qv) - vv * (qq - rr) >= 0;
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 device, const _vec3 * eye, const _vec3 * at, const _vec3 * up, const _float & fovY, const _float & aspect, const _float & nearZ, const _float & farZ)
{
	CDynamicCamera*	instance = new CDynamicCamera(device);

	if (FAILED(instance->Ready(eye, at, up, fovY, aspect, nearZ, farZ)))
		SafeRelease(instance);

	return instance;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}