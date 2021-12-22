#pragma once
#include "IScene.h"
#include <vector>
using namespace std;

class IDXObjectBase;

/// <summary>
/// Intro cut Scene
/// 
/// 2021. 08. 01 Hacgeum
/// </summary>
class IntroCutScene : public IScene
{
public:
	IntroCutScene();
	virtual ~IntroCutScene();

	virtual void Initialize(DXDevice* pDevice, DXFactory* pFactory) override;
	virtual void Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera) override;
	virtual void Render(DXRenderer* pRenderer) override;
	virtual void Finalize() override;
	virtual void ChangeSceneState(SceneManager* pSceneManager) override;

private:
	vector<IDXObjectBase*> m_pUI;
	
	bool m_PostOfficeOn;
	int m_NowTextIndex;

	bool m_NextScene;
};