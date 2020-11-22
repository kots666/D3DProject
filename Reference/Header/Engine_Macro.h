#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
	#define			USING(NAMESPACE)	using namespace NAMESPACE;

	#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), __T("System Message"), MB_OK)
	#define			TAGMSG_BOX(_tag,_message)	MessageBoxW(NULL, _message, _tag, MB_OK)

	#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
	#define			END						}

	#ifdef ENGINE_EXPORTS
	#define ENGINE_DLL _declspec(dllexport)
	#else
	#define ENGINE_DLL _declspec(dllimport)
	#endif // ENGINE_EXPORTS


	#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){__asm { int 3 };return;}}

	#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){__asm { int 3 };return _return;}}

	#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

	#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


	#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

	#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

	#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

	#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}



	#define NO_COPY(CLASSNAME)							\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

	#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_instance;					\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

	#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_instance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_instance) {					\
				m_instance = new CLASSNAME;				\
			}											\
			return m_instance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_instance)	{					\
				delete m_instance;						\
				m_instance = NULL;						\
			}											\
		}
}

#endif
