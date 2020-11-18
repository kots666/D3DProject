#include "VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 device) :
	CResources(device), m_VB(nullptr), m_IB(nullptr),
	m_vtxCnt(0), m_triCnt(0), m_vtxSize(0), m_FVF(0),
	m_idxSize(0), m_format(D3DFMT_INDEX16)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) :
	CResources(rhs), m_VB(rhs.m_VB), m_IB(rhs.m_IB),
	m_vtxCnt(rhs.m_vtxCnt), m_triCnt(rhs.m_triCnt),
	m_vtxSize(rhs.m_vtxSize), m_FVF(rhs.m_FVF),
	m_idxSize(rhs.m_idxSize), m_format(rhs.m_format)
{
	SafeAddRef(m_VB);
	SafeAddRef(m_IB);
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready()
{
	if (FAILED(m_device->CreateVertexBuffer(
		m_vtxSize * m_vtxCnt,
		0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
		m_FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL
	)))
	{
		MSG_BOX("Fail to create vertex buffer");
		return E_FAIL;
	}

	if (FAILED(m_device->CreateIndexBuffer(
		m_idxSize * m_triCnt,
		0,
		m_format,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL
	)))
	{
		MSG_BOX("Fail to create index buffer");
		return E_FAIL;
	}

	return S_OK;
}

void CVIBuffer::Render()
{
	// �׷��� ����̽� ��ġ���� ���� ���� �׸����� ���۸� ��ũ�����ִ� �Լ�
	m_device->SetStreamSource(0, m_VB, 0, m_vtxSize);
	// 1���� : �� �� ���Կ� ������ ���ΰ�
	// 2���� : � ���� �Ѱ��� ���ΰ�
	// 3���� : ��𼭺��� �׸� ���ΰ�
	// 4���� : � ������ ǥ���� ���ΰ�
	m_device->SetIndices(m_IB);
	m_device->SetFVF(m_FVF);
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vtxCnt, 0, m_triCnt);
}

void CVIBuffer::Free()
{
	SafeRelease(m_VB);
	SafeRelease(m_IB);

	CResources::Free();
}

