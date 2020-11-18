// ResourcesMgr
// Get
// Set
// General

HRESULT Reserve_ContainerSize(const _ushort& size)
{
	return CResourcesMgr::GetInstance()->Reserve(size);
}
HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 device, const _ushort& index, const _tchar* bufferTag, BUFFERID ID, _ulong cntX, _ulong cntZ, _ulong interval)
{
	return CResourcesMgr::GetInstance()->Ready(device, index, bufferTag, ID, cntX, cntZ, interval);
}
void Render_Buffer(const _ushort& index, const _tchar* bufferTag)
{
	CResourcesMgr::GetInstance()->Render(index, bufferTag);
}

// Release Resources
void Release_Resoures()
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}
