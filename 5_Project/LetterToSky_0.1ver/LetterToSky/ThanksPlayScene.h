#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

/// <summary>
/// 2021. 08. 20 Hacgeum
/// </summary>
class ThanksPlayScene : public IScene
{
public:
	ThanksPlayScene();
	virtual ~ThanksPlayScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;
};