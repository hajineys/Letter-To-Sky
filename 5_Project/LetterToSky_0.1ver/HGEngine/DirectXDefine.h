#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")		// adapter info

// Effect
#ifdef _DEBUG
	#pragma comment( lib, "../lib/Effects11d.lib" )
#else
	#pragma comment( lib, "../lib/Effects11.lib" )
#endif

// FXC : warning X4717: Effects deprecated for D3DCompiler_47
// -> 하위호환성이 지원이 되지 않아 발생하는 오류

// Font
#ifdef _DEBUG
	#pragma comment( lib, "../DXTK/Lib/DirectXTK.lib" )
#else
	#pragma comment( lib, "../DXTK/Lib/DirectXTKr.lib" )
#endif

// ASE Parser
#ifdef _DEBUG
#pragma comment( lib, "../lib/ASEParser_x86d.lib" )
#else
#pragma comment( lib, "../lib/ASEParser_x86.lib" )
#endif

// Fmod (Sound)
#ifdef _WIN32
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodL_vc.lib")
#endif

// Direct3D 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#include <Windows.h>
#include <stdlib.h>
#include <cassert>		// assert
#include <fstream>		// ifstream
#include <vector>
#include <float.h>
#include <cmath>
using namespace std;

//---------------------------------------------------------------------------------------
// Simple d3d error checker for book demos.
//---------------------------------------------------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
	{														\
		HRESULT hr = (x);									\
		if(FAILED(hr))										\
		{													\
			LPWSTR output;									\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |			\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
				NULL,										\
				hr,											\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,							\
				0,											\
				NULL);										\
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}													\
	}
#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SAFE_RELEASE(x) { if(x){ x->Release(); x = 0; } }	// 예전 스타일

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------
#define SafeDelete(x) { delete x; x = 0; }

inline void ____error(LPCWSTR message)
{
	MessageBox(NULL, message, L"에러!", MB_OK);
}

#include <atltrace.h>
#define TRACE ATLTRACE
