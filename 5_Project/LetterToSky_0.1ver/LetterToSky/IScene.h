#pragma once
#include <windows.h>
#include <map>
#include <string>

class DXDevice;
class DXFactory;
class ResourceManager;
class SceneManager;
class GameTimer;
class D3Camera;
class DXRenderer;
class BoxCollider;

/// <summary>
/// �� �������̽�
/// 
/// ������ �θ� Ŭ����
/// 2021. 07. 19 Hacgeum
/// </summary>
class IScene
{
public:
	IScene() {}
	virtual ~IScene() {}

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) {};
	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager) {};
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) abstract;
	virtual void Render(DXRenderer* pRenderer) abstract;
	virtual void Finalize() abstract;
	virtual void ChangeSceneState(SceneManager* pSceneManager) abstract;

	virtual BoxCollider* GetFootStepStair() { return nullptr; };

protected:
	POINT m_MousePos = { 0, };
};