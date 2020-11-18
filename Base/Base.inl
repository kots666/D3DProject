CBase::CBase() : m_refCnt(0)
{

}

CBase::~CBase()
{

}

unsigned long CBase::AddRef()
{
	return m_refCnt++;
}

unsigned long CBase::Release()
{
	if (0 == m_refCnt)
	{
		Free();

		delete this;

		return 0;
	}

	return m_refCnt--;
}