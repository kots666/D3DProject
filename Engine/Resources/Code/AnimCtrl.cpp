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
		rhs.m_animCtrl->GetMaxNumAnimationOutputs(), // ���� �� ���� ��ü���� �����ǰ� �ִ� �ִϸ��̼��� ����
		rhs.m_animCtrl->GetMaxNumAnimationSets(), // ���� ������ �ִϸ��̼��� �ִ� ����(��Դ� 1�� ���ڿ� ����)
		rhs.m_animCtrl->GetMaxNumTracks(), // �ִϸ��̼��� ����ϱ� ���� �ִ� Ʈ���� ����(��κ� �� ���� ���, ���� ���� ����ؾ� �� �� ���)
		rhs.m_animCtrl->GetMaxNumEvents(), // �ִϸ��̼� ��� �� ����Ǵ� ��Ư�� �̺�Ʈ(�츮�� ��� ����)
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

	// �ε��� ���� �ش��ϴ� �ִϸ��̼� ���� ���� �Լ�
	m_animCtrl->GetAnimationSet(index, &animSet);

	// m_animCtrl->GetAnimationSetByName()

	m_period = animSet->GetPeriod(); // �ִϸ��̼� ���� ��� �ð��� ��ȯ�ϴ� �Լ�


	// newƮ���� �ִϸ��̼� �� ����
	m_animCtrl->SetTrackAnimationSet(m_newTrack, animSet);

	// �ִϸ��̼� ���� �ȿ� ���ԵǾ� �ִ� �̺�Ʈ ������ �����ϴ� �Լ�(�̺�Ʈ�� ���� ������ ó��)
	// : �츮�� �̺�Ʈ�� ����� �� ���� ��Ȳ�ε�, ������ ���ԵǾ� �ִ� ��� ���� �ִϸ��̼��� ���� ���� �ʴ� ������ �߻�
	m_animCtrl->UnkeyAllTrackEvents(m_currentTrack);
	m_animCtrl->UnkeyAllTrackEvents(m_newTrack);

	_double transitionTime = 0.01;

	// ���� ������ Ʈ���� ��� �Ǵ� ���� ��Ű�� ���� �Լ�(3���� : �������� ���� Ʈ���� ������ ���ΰ�)
	m_animCtrl->KeyTrackEnable(m_currentTrack, FALSE, m_accTime + transitionTime);

	// ���ڰ����� ������ Ʈ���� ���õ� �ִϸ��̼� ���� � �ӵ��� ������ ������ �����ϴ� �Լ�(�ӵ��� ��� ���� 1)
	m_animCtrl->KeyTrackSpeed(m_currentTrack, 0.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);

	// ���ڰ����� ������ Ʈ���� ����ġ�� �����ϴ� �Լ�
	m_animCtrl->KeyTrackWeight(m_currentTrack, 0.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);


	// New Ʈ���� Ȱ��ȭ�� �����ϴ� �Լ�
	m_animCtrl->SetTrackEnable(m_newTrack, TRUE);
	m_animCtrl->KeyTrackSpeed(m_newTrack, 1.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);
	m_animCtrl->KeyTrackWeight(m_newTrack, 1.f, m_accTime, transitionTime, D3DXTRANSITION_LINEAR);

	m_animCtrl->ResetTime(); // AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð��� �ʱ�ȭ�ϴ� �Լ�
	m_accTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ������ ������ �߱� ������ ��� ������ 0��(0�� ��ġ�� �ʱ�ȭ)�� �����ϵ��� �����ϴ� �Լ�
	m_animCtrl->SetTrackPosition(m_newTrack, 0.0);

	m_oldAnimIdx = index;
	m_currentTrack = m_newTrack;

	*blendTime = transitionTime;

	return true;
}

void CAnimCtrl::PlayAnimation(const _float & deltaTime)
{
	m_animCtrl->AdvanceTime(deltaTime, NULL);	// 2���� : �ִϸ��̼� ����� ���� ���峪, ����Ʈ�� ���� ����, ������ �Ⱦ�.
												// AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð� ���� ����
	m_accTime += deltaTime;
}

_bool CAnimCtrl::IsAnimationSetEnd()
{
	// Ʈ�������� �����ͼ� ���� ��� ��ġ�� �� �������� 0.1�� �� ������ �����ٸ� �ִϸ��̼��� �������� �˷��ش�.
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
