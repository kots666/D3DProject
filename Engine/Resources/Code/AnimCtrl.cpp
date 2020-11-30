#include "AnimCtrl.h"

USING(Engine)

CAnimCtrl::CAnimCtrl(LPD3DXANIMATIONCONTROLLER animCtrl) :
	m_animCtrl(animCtrl),
	m_currentTrack(0),
	m_newTrack(1),
	m_accTime(0.f),
	m_oldAnimIdx(999),
	m_period(0.0)
{
	SafeAddRef(m_animCtrl);
}

CAnimCtrl::CAnimCtrl(const CAnimCtrl & rhs) :
	m_currentTrack(rhs.m_currentTrack),
	m_newTrack(rhs.m_newTrack),
	m_accTime(rhs.m_accTime),
	m_oldAnimIdx(rhs.m_oldAnimIdx),
	m_period(rhs.m_period)
{
	rhs.m_animCtrl->CloneAnimationController(
		rhs.m_animCtrl->GetMaxNumAnimationOutputs(), // 복제 시 원본 객체에서 제공되고 있는 애니메이션의 개수
		rhs.m_animCtrl->GetMaxNumAnimationSets(), // 구동 가능한 애니메이션의 최대 개수(대게는 1번 인자와 같음)
		rhs.m_animCtrl->GetMaxNumTracks(), // 애니메이션을 재생하기 위한 최대 트랙의 개수(대부분 한 개를 사용, 정말 많이 사용해야 두 개 사용)
		rhs.m_animCtrl->GetMaxNumEvents(), // 애니메이션 재생 시 적용되는 독특한 이벤트(우리는 사용 못함)
		&m_animCtrl);
}

CAnimCtrl::~CAnimCtrl()
{
}

HRESULT CAnimCtrl::Ready()
{
	return S_OK;
}

_bool CAnimCtrl::IsAnimationSetChange(const _uint & index, _double* blendTime)
{
	if (m_oldAnimIdx == index) return false;

	m_newTrack = m_currentTrack ^ 1;

	LPD3DXANIMATIONSET animSet = nullptr;

	// 인덱스 값에 해당하는 애니메이션 셋을 얻어는 함수
	m_animCtrl->GetAnimationSet(index, &animSet);

	// m_animCtrl->GetAnimationSetByName()

	m_period = animSet->GetPeriod(); // 애니메이션 셋의 재생 시간을 반환하는 함수


	// new트랙에 애니메이션 셋 세팅
	m_animCtrl->SetTrackAnimationSet(m_newTrack, animSet);

	// 애니메이션 정보 안에 삽입되어 있는 이벤트 정보를 해제하는 함수(이벤트가 없는 것으로 처리)
	// : 우리가 이벤트를 사용할 수 없는 상황인데, 정보가 삽입되어 있는 경우 가끔 애니메이션이 보간 되지 않는 문제가 발생
	m_animCtrl->UnkeyAllTrackEvents(m_currentTrack);
	m_animCtrl->UnkeyAllTrackEvents(m_newTrack);

	_double transitionTime = 0.01;

	// 현재 설정된 트랙을 재생 또는 종료 시키기 위한 함수(3인자 : 언제부터 현재 트랙을 해제할 것인가)
	m_animCtrl->KeyTrackEnable(m_currentTrack, FALSE, m_accTime + transitionTime);

	// 인자값으로 들어오는 트랙에 세팅된 애니메이션 셋을 어떤 속도로 움직일 것인지 설정하는 함수(속도의 상수 값은 1)
	m_animCtrl->KeyTrackSpeed(m_currentTrack, 0.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);

	// 인자값으로 들어오는 트랙의 가중치를 설정하는 함수
	m_animCtrl->KeyTrackWeight(m_currentTrack, 0.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);


	// New 트랙의 활성화를 지시하는 함수
	m_animCtrl->SetTrackEnable(m_newTrack, TRUE);
	m_animCtrl->KeyTrackSpeed(m_newTrack, 1.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);
	m_animCtrl->KeyTrackWeight(m_newTrack, 1.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);

	m_animCtrl->ResetTime(); // AdvanceTime 호출 시 내부적으로 누적되던 시간을 초기화하는 함수
	m_accTime = 0.f;

	// 기존의 재생 중이던 트랙에서 새로운 트랙으로 변경을 했기 때문에 재생 시점을 0초(0의 위치로 초기화)로 시작하도록 지시하는 함수
	m_animCtrl->SetTrackPosition(m_newTrack, 0.0);

	m_oldAnimIdx = index;
	m_currentTrack = m_newTrack;

	*blendTime = transitionTime;

	return true;
}

void CAnimCtrl::PlayAnimation(const _float & deltaTime)
{
	m_animCtrl->AdvanceTime(deltaTime, NULL);	// 2인자 : 애니메이션 재생에 따라 사운드나, 이펙트를 구동 가능, 하지만 안씀.
												// AdvanceTime 호출 시 내부적으로 누적되는 시간 값이 있음
	m_accTime += deltaTime;
}

_bool CAnimCtrl::IsAnimationSetEnd()
{
	// 트랙정보를 가져와서 현재 재생 위치가 끝 지점에서 0.1을 뺀 지점을 지났다면 애니메이션이 끝났음을 알려준다.
	D3DXTRACK_DESC trackInfo;
	ZeroMemory(&trackInfo, sizeof(D3DXTRACK_DESC));

	m_animCtrl->GetTrackDesc(m_currentTrack, &trackInfo);

	if (trackInfo.Position >= m_period - 0.03)
		return true;

	return false;
}

CAnimCtrl * CAnimCtrl::Create(LPD3DXANIMATIONCONTROLLER animCtrl)
{
	CAnimCtrl* instance = new CAnimCtrl(animCtrl);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CAnimCtrl * CAnimCtrl::Create(const CAnimCtrl & rhs)
{
	CAnimCtrl* instance = new CAnimCtrl(rhs);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

void CAnimCtrl::Free()
{
	SafeRelease(m_animCtrl);
}
