#pragma once



#ifdef ENGINE_EXPORTS
#define ENGINE_DLL	_declspec(dllexport)
#else
#define ENGINE_DLL	_declspec(dllimport)
#endif // DEBUG


#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

#define GetSingle(SINGLETON) SINGLETON::GetInstance()

#define DIS_Up				0b00000001
#define DIS_Down			0b00000110
#define DIS_DoubleDown		0b00000100
#define DIS_Press			0b11111111

// ??
#define COLOR_GRAY		(D3DCOLOR_ARGB(255, 124, 124, 124))
#define COLOR_BLUE      (D3DCOLOR_XRGB(0, 0, 255))
#define COLOR_WHITE     (D3DCOLOR_XRGB(255, 255, 255))
#define COLOR_BLACK     (D3DCOLOR_XRGB(0, 0, 0))
#define COLOR_RED		(D3DCOLOR_XRGB(255, 0, 0))
#define COLOR_GREEN		(D3DCOLOR_XRGB(0, 255, 0))
#define COLOR_YELLOW    (D3DCOLOR_XRGB(255, 255, 0))
#define COLOR_CYAN      (D3DCOLOR_XRGB(0, 255, 255))
#define COLOR_MAGENTA   (D3DCOLOR_XRGB(255, 0, 255))

#define BEGIN(p)	namespace p {
#define END			}

 

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);


//?̱???////////////////////////////////////////////////////////////////////////
#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static _ulong DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = nullptr;	\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(nullptr == m_pInstance) {				\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		_ulong CLASSNAME::DestroyInstance( void ) {		\
			_ulong dwRefCnt = 0;						\
			if(nullptr != m_pInstance)	{				\
				dwRefCnt = m_pInstance->Release();		\
				m_pInstance = nullptr;					\
			}											\
			return dwRefCnt;							\
		}


#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();


// ?????? üũ ??ũ??
// {__asm { int 3; }; ?ڵ? ???????? Break ????Ʈ ?ɱ? 86??Ʈ ????
// __debugbreak(); ?ڵ? ???????? Break ????Ʈ ?ɱ? 64??Ʈ ????

// NULL üũ1 / ?⺻ 
#define NULL_CHECK( _ptr) \
if( _ptr == 0){__debugbreak(); return;}


// NULL üũ2 / ?? ???? 
#define NULL_CHECK_BREAK( _ptr) \
if( _ptr == 0){__debugbreak();}

// NULL üũ3 / ???? ???? 
#define NULL_CHECK_RETURN( _ptr, _return) \
if( _ptr == 0){__debugbreak(); return _return;}

// NULL üũ4 / ?޼????ڽ?
#define NULL_CHECK_MSG( _ptr, _message) \
if( _ptr == 0){MessageBox(NULL, _message, L"System Message", MB_OK);__debugbreak();}

// NULL üũ4 / ?޼????ڽ? / ???? ????
#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message ) \
if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__debugbreak();return _return;}

// IF üũ / TRUE
#define IF_CHECK_TRUE( _IF) \
if(_IF){__debugbreak();}

// IF üũ / FALSE
#define IF_CHECK_FALSE( _IF) \
if(_IF){__debugbreak();}


//#define IF_CHECK_MESSAGE( _IF, _message) \
if( _IF){MessageBox(NULL, _message, L"System Message", MB_OK);__debugbreak();};


// HRESULT ????üũ

// HRESULT ????üũ1 / ?⺻
#define FAILED_CHECK(_hr) \
if(((HRESULT)(_hr)) < 0) { MSGBOX("Fail"); __debugbreak();\
return E_FAIL;}

// HRESULT ????üũ2 / ?븮??
#define FAILED_CHECK_NONERETURN(_hr) \
if(((HRESULT)(_hr)) < 0) { MSGBOX("Fail"); __debugbreak();}

// HRESULT ????üũ3 / ???? ????
#define FAILED_CHECK_RETURN(_hr, _return) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, L"Failed", L"System Error",MB_OK); __debugbreak();\
return _return;}

// HRESULT ????üũ4 / ?޼??? ????
#define FAILED_CHECK_MSG( _hr, _message) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, _message, L"System Message",MB_OK); __debugbreak();return E_FAIL;}

// HRESULT ????üũ5 / ???? ?޼??? ????
#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message) if( ((HRESULT)(_hr)) < 0 ) \
{ MessageBox(NULL, _message, L"System Message"sp,MB_OK); __debugbreak();return _return;}


