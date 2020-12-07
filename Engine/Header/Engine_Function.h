#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	DWORD SafeAddRef(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (nullptr != pointer)
			dwRefCnt = pointer->AddRef();

		return dwRefCnt;
	}

	template <typename T>
	DWORD SafeRelease(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (nullptr != pointer)
		{
			dwRefCnt = pointer->Release();
			if (dwRefCnt == 0)
				pointer = nullptr;
		}
		return dwRefCnt;
	}

	template <typename T>
	void SafeSingleDestory(T& pointer)
	{
		if (nullptr != pointer)
		{
			pointer->DestroyInstance();
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeDelete(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeDeleteArray(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////
	/////////////////////////////////Functor(함수 객체)//////////////////////////

	class CTagFinder
	{
	public:
		explicit CTagFinder(const _tchar* pTag)
			: m_pTargetTag(pTag) 
		{		
		}
		~CTagFinder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& instance)
		{
			_ulong dwRefCnt = 0;

			if (nullptr != instance)
			{
				dwRefCnt = instance->Release();

				if (0 == dwRefCnt)
					instance = nullptr;
			}
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			if (nullptr != Pair.second)
			{
				dwRefCnt = Pair.second->Release();

				if (0 == dwRefCnt)
					Pair.second = nullptr;
			}
		}
	};
}


#endif
