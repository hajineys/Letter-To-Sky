// LetterToSky.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "framework.h"
#include "GameApp.h"

// 메모리 릭 확인용
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 디버그 빌드의 경우 실행시점 메모리 점검 기능을 켠다
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	/// 메모리 릭이 발생할때 사용!
	//_CrtSetBreakAlloc(1120063);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 애플리케이션 생성
	GameApp* pGameApp = new GameApp();

	// 애플리케이션 초기화를 수행합니다:
	if (!pGameApp->Initialize(hInstance))
	{
		return false;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LETTERTOSKY));

	// 애플리케이션 실행
	pGameApp->Run();

	// 애플리케이션 종료 및 메모리 해제
	pGameApp->Finalize();
	delete pGameApp;
	pGameApp = nullptr;

	return true;
}