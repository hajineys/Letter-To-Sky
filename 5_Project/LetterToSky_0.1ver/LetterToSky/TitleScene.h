#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

#include <windows.h>

class IDXObjectBase;

/// <summary>
/// Title Scene
/// 
/// 2021. 08. 01 Hacgeum
/// </summary>
class TitleScene : public IScene
{
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;

	bool m_NextScene;

	static bool m_OnceCreateAll;
};