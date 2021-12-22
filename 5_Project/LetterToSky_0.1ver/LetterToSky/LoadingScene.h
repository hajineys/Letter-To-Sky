#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

/// <summary>
/// Loading Scene
/// 
/// 2021. 08. 01 Hacgeum
/// </summary>
class LoadingScene : public IScene
{
public:
	LoadingScene();
	virtual ~LoadingScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;

	bool m_NextScene;
};